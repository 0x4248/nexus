import time
import requests
import collector
import sys
import uuid
import socket

SERVER = sys.argv[1] if len(sys.argv) > 1 else "http://localhost:9000"
HOSTNAME = socket.gethostname()

def send_stats():
    requests.post(
        f"{SERVER}/ingest",
        json=collector.collect(),
        timeout=2
    )

def ping():
    ping_id = str(uuid.uuid4())
    t0 = time.time_ns()

    r = requests.get(
        f"{SERVER}/ping",
        params={"ts": t0, "id": ping_id},
        timeout=2
    )

    t1 = time.time_ns()
    r.raise_for_status()

    rtt_ms = (t1 - t0) / 1_000_000

    # agent-side receipt
    requests.post(
        f"{SERVER}/ingest",
        json={
            "hostname": HOSTNAME,
            "event": "ping_received",
            "ping_id": ping_id,
        },
        timeout=2
    )

    # controller-side RTT
    requests.post(
        f"{SERVER}/ingest",
        json={
            "hostname": HOSTNAME,
            "event": "ping_rtt",
            "ping_id": ping_id,
            "rtt_ms": round(rtt_ms, 3),
        },
        timeout=2
    )

if __name__ == "__main__":
    while True:
        try:
            send_stats()
            ping_id, rtt = ping()

            # store RTT on controller side
            requests.post(
                f"{SERVER}/ingest",
                json={
                    "hostname": HOSTNAME,
                    "event": "ping_rtt",
                    "ping_id": ping_id,
                    "rtt_ms": round(rtt, 3),
                },
                timeout=2
            )

        except Exception:
            pass

        time.sleep(5)
