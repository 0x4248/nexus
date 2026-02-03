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

STYLE = """
* {
    font-family: monospace;
    background-color: #000;
    color: #fff;
}
a {
    color: #0f0;
    cursor: pointer;
    margin-right: 8px;
}

input, textarea {
    background-color: #222;
    color: #fff;
    border: 1px solid #555;
}

input[type="submit"] {
    background-color: #444;
    border: 1px solid #888;
    padding: 5px 10px;
}
"""

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
    return f"""
    <html>
    <head>
        <style>{STYLE}</style>
    </head>
    <body>
        <h1>ESC/POS Print Server</h1>

        <p>Insert ESC/POS formatting tokens:</p>
        <div>
            <a onclick="appendText('[NORMAL]')">[NORMAL]</a>
            <a onclick="appendText('[CENTER]')">[CENTER]</a>
            <a onclick="appendText('[BOLD]')">[BOLD]</a>
            <a onclick="appendText('[CUT]')">[CUT]</a>
            <a onclick="appendText('[INVERT ON]')">[INVERT ON]</a>
            <a onclick="appendText('[INVERT OFF]')">[INVERT OFF]</a>
            <a onclick="appendText('[WIDE]')">[WIDE]</a>
            <a onclick="appendText('[HR]')">[HR]</a>
            <a onclick="appendText('[NL]')">[NL]</a>
        </div>

        <br>

        <form action="/print/submit_job" method="post">
            <textarea id="escpos_data"
                      name="escpos_data"
                      rows="12"
                      cols="60"
                      placeholder="Enter ESC/POS commands here..."></textarea>
            <br><br>
            Copies:
            <input type="number" name="copies" value="1" min="1" max="10">
            <br><br>
            <input type="submit" value="Print">
        </form>

        <script>
        function appendText(text) {{
            var textarea = document.getElementById('escpos_data');
            textarea.value += text;
        }}
        </script>
    </body>
    </html>
    """

# --- main ------------------------------------------------------------------

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80, debug=False, use_reloader=False)
