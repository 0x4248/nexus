import json
import os
import time
import threading
import uuid
from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

DATA_DIR = "/var/lib/pulsewatch"
os.makedirs(DATA_DIR, exist_ok=True)

lock = threading.Lock()

def load_json(path):
    if not os.path.exists(path):
        return {}
    with open(path, "r") as f:
        return json.load(f)


def store_stats(host, payload):
    path = os.path.join(DATA_DIR, f"{host}.json")
    ts = str(int(time.time()))

    with lock:
        data = {}
        if os.path.exists(path):
            with open(path, "r") as f:
                data = json.load(f)

        data[ts] = payload

        with open(path, "w") as f:
            json.dump(data, f)

def store_ping(host, payload):
    path = os.path.join(DATA_DIR, f"{host}_pings.json")
    ts = str(int(time.time()))

    with lock:
        data = {}
        if os.path.exists(path):
            with open(path, "r") as f:
                data = json.load(f)

        data[ts] = payload

        with open(path, "w") as f:
            json.dump(data, f)


class Handler(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path != "/ingest":
            self.send_error(404)
            return

        length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(length)

        try:
            payload = json.loads(body)
        except json.JSONDecodeError:
            self.send_error(400)
            return

        host = payload.get("hostname") or self.client_address[0]

        if payload.get("event") in ("ping_received", "ping_rtt"):
            store_ping(host, payload)
        else:
            store_stats(host, payload)

        self.send_response(200)
        self.end_headers()
        self.wfile.write(b"OK")

    def do_GET(self):
        parsed = urlparse(self.path)

        # --- ping endpoint (unchanged) ---
        if parsed.path == "/ping":
            qs = parse_qs(parsed.query)
            ts = qs.get("ts", [None])[0]
            ping_id = qs.get("id", [None])[0]

            if ts is None or ping_id is None:
                self.send_error(400)
                return

            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps({
                "ts": ts,
                "ping_id": ping_id
            }).encode())
            return

        # --- list hosts ---
        if parsed.path == "/data/hosts":
            hosts = []
            for f in os.listdir(DATA_DIR):
                if f.endswith(".json") and not f.endswith("_pings.json"):
                    hosts.append(f.replace(".json", ""))

            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps(hosts).encode())
            return

        # --- per-host data ---
        if parsed.path.startswith("/data/host/"):
            parts = parsed.path.split("/")
            if len(parts) < 4:
                self.send_error(404)
                return

            host = parts[3]
            is_ping = len(parts) == 5 and parts[4] == "pings"

            fname = (
                f"{host}_pings.json"
                if is_ping
                else f"{host}.json"
            )

            path = os.path.join(DATA_DIR, fname)
            data = load_json(path)

            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(json.dumps(data).encode())
            return

        self.send_error(404)


    def log_message(self, *_):
        pass

if __name__ == "__main__":
    server = ThreadingHTTPServer(("0.0.0.0", 9000), Handler)
    server.serve_forever()
