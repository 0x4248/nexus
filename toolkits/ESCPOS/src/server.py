# python http server that lets you send escpos command via the escpos formatting language
from flask import request, Response
from flask import Flask
from subprocess import Popen, PIPE
import shlex
import logging
import os
import datetime

app = Flask(__name__)
logging.basicConfig(level=logging.DEBUG)
PRINTER = "/dev/usb/lp0" 

STYLE = """
* {
    font-family: monospace;
    background-color: #000;
    color: #fff;
}
a {
    color: #0f0;
}

input, textarea {
    background-color: #222;
    color: #fff;
    border: 1px solid #555;
}

input[type="submit"] {
    background-color: #444;
    color: #fff;
    border: 1px solid #888;
    padding: 5px 10px;
}

"""

def generate_escpos_cmdline(print_id):
    return f"./build/escpos < ./data/jobs/{print_id}.epml > {PRINTER}"

if not os.path.exists(PRINTER):
    logging.error(f"Printer device {PRINTER} does not exist. Please check the printer connection.")
    exit(1)

if not os.path.exists("./data/jobs"):
    os.makedirs("./data/jobs")
else:
    for filename in os.listdir("./data/jobs"):
        file_path = os.path.join("./data/jobs", filename)
        if os.path.isfile(file_path):
            os.remove(file_path)

class JobList:
    def __init__(self):
        self.jobs = []
    
    def add_job(self, job_id, title="New Job"):
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.jobs.append((job_id, title, timestamp))
    
    def get_jobs(self):
        return self.jobs

print_jobs = JobList()

@app.route('/print/<print_id>', methods=['POST'])
def print_escpos(print_id):
    escpos_data = request.data.decode('utf-8')
    job_file_path = f"./data/jobs/{print_id}.epml"
    
    with open(job_file_path, 'w') as job_file:
        job_file.write(escpos_data)
    
    cmdline = generate_escpos_cmdline(print_id)
    logging.debug(f"Executing command: {cmdline}")
    
    process = Popen(shlex.split(cmdline), stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    
    if process.returncode != 0:
        logging.error(f"Error printing job {print_id}: {stderr.decode('utf-8')}")
        return Response(f"Error printing job {print_id}: {stderr.decode('utf-8')}", status=500)
    
    logging.info(f"Successfully printed job {print_id}")
    return Response(f"Successfully printed job {print_id}", status=200)


@app.route('/print/submit_job', methods=['POST'])
def submit_print_job():
    escpos_data = request.form.get('escpos_data', '')
    job_title = request.form.get('job_title', 'New Job')
    print_now = request.form.get('print_now', 'no') == 'yes'
    copies = int(request.form.get('copies', '1'))
    
    job_id = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    job_file_path = f"./data/jobs/{job_id}.epml"
    
    with open(job_file_path, 'w') as job_file:
        job_file.write(escpos_data)
    
    print_jobs.add_job(job_id, job_title)
    
    if print_now:
        for _ in range(copies):
            cmdline = generate_escpos_cmdline(job_id)
            logging.debug(f"Executing command: {cmdline}")
            
            process = Popen(shlex.split(cmdline), stdout=PIPE, stderr=PIPE)
            stdout, stderr = process.communicate()
            
            if process.returncode != 0:
                logging.error(f"Error printing job {job_id}: {stderr.decode('utf-8')}")
                return Response(f"Error printing job {job_id}: {stderr.decode('utf-8')}", status=500)
        
        logging.info(f"Successfully printed job {job_id} ({copies} copies)")
        return Response(f"Successfully printed job {job_id} ({copies} copies)", status=200)
    
    return Response(f"Job {job_id} submitted successfully", status=200)

@app.route('/', methods=['GET'])
def index():
    job_list_html = "<h2>Print Jobs</h2><ul>"
    for job in print_jobs.get_jobs():
        job_id, title, timestamp = job
        job_list_html += f"<li><a href='/print/{job_id}'>{title} - {timestamp}</a></li>"
    job_list_html += "</ul>"

    head_html = f"""
    <head>
    <style>
    {STYLE}
    </style>
    </head>
    """
    form_html = """
    <h1>ESC/POS Print Server</h1>
    <p>Use the buttons below to insert ESC/POS commands into the textarea.</p>
    <a onclick="appendText('[NORMAL]')">[NORMAL]</a>
    <a onclick="appendText('[CENTER]')">[CENTER]</a>
    <a onclick="appendText('[BOLD]')">[BOLD]</a>
    <a onclick="appendText('[CUT]')">[CUT]</a>
    <a onclick="appendText('[INVERT ON]')">[INVERT ON]</a>
    <a onclick="appendText('[INVERT OFF]')">[INVERT OFF]</a>
    <a onclick="appendText('[WIDE]')">[WIDE]</a>
    <a onclick="appendText('[HR]')">[HORIZONTAL RULE]</a>
    <a onclick="appendText('[NL]')">[NEWLINE]</a>
    <br><br>
    <form action="/print/submit_job" method="post">
        <input type="text" name="job_title" placeholder="Job Title"><br>
        <textarea id="escpos_data" name="escpos_data" rows="10" cols="50" placeholder="Enter ESC/POS commands here..."></textarea><br>
        <input type="checkbox" name="print_now" value="yes" checked> Print Now<br>
        <input type="number" name="copies" value="1" min="1" max="10"> Copies<br>
        <input type="submit" value="Send Job to Server">
    </form>
    <script>
    function appendText(text) {
        var textarea = document.getElementById('escpos_data');
        textarea.value += text;
    }
    </script>
    """
    
    return head_html + form_html + job_list_html

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=True)