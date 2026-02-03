#!/usr/bin/env python3

from flask import Flask, request, Response
from subprocess import Popen, PIPE
import logging
import os
import datetime
import threading
import queue

app = Flask(__name__)
logging.basicConfig(level=logging.INFO)

PRINTER = "/dev/usb/lp0"
JOB_DIR = "./data/jobs"

# --- sanity checks ---------------------------------------------------------

if not os.path.exists(PRINTER):
    raise RuntimeError(f"Printer device {PRINTER} does not exist")

os.makedirs(JOB_DIR, exist_ok=True)

# --- job queue -------------------------------------------------------------

print_queue = queue.Queue()

def printer_worker():
    while True:
        job_path = print_queue.get()
        try:
            logging.info("printing %s", job_path)
            with open(job_path, "rb") as infile, open(PRINTER, "wb") as outfile:
                proc = Popen(
                    ["./build/escpos"],
                    stdin=infile,
                    stdout=outfile,
                    stderr=PIPE,
                    close_fds=True,
                )
                _, stderr = proc.communicate()
                if proc.returncode != 0:
                    logging.error("print failed: %s", stderr.decode())
        finally:
            print_queue.task_done()

threading.Thread(target=printer_worker, daemon=True).start()

# --- routes ----------------------------------------------------------------

@app.route("/print/<print_id>", methods=["POST"])
def print_escpos(print_id):
    job_path = f"{JOB_DIR}/{print_id}.epml"
    with open(job_path, "wb") as f:
        f.write(request.data)

    print_queue.put(job_path)
    return Response("queued\n", status=202)


@app.route("/print/submit_job", methods=["POST"])
def submit_print_job():
    escpos_data = request.form.get("escpos_data", "")
    copies = int(request.form.get("copies", "1"))

    job_id = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    job_path = f"{JOB_DIR}/{job_id}.epml"

    with open(job_path, "w") as f:
        f.write(escpos_data)

    for _ in range(copies):
        print_queue.put(job_path)

    return Response("queued\n", status=202)


@app.route("/", methods=["GET"])
def index():
    return """
    <h1>ESC/POS Print Server</h1>
    <form action="/print/submit_job" method="post">
        <textarea name="escpos_data" rows="10" cols="50"></textarea><br>
        <input type="number" name="copies" value="1" min="1" max="10"><br>
        <input type="submit">
    </form>
    """

# --- main ------------------------------------------------------------------

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80, debug=False, use_reloader=False)
