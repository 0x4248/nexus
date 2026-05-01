import os

import flask

app = flask.Flask(__name__)

TYPEWRITER_CSS = """
pre {
    font-family: "PT Mono", monospace;
    font-size: 11px;
    line-height: 1.4;
    margin: 0;
}

html, body {
    margin: 0;
}
"""


@app.route("/")
def index():
    files = [f for f in os.listdir(".") if f.startswith("TRX-") and f.endswith(".txt")]
    links = [f'<a href="/{f}">{f}</a><br>' for f in files]
    return "".join(links)


@app.route("/<filename>")
def show_trx(filename):
    if not filename.startswith("TRX-") or not filename.endswith(".txt"):
        return "Invalid file", 404
    try:
        with open(filename, "r") as f:
            content = f.read()
        return f"<pre>{content}</pre>"
    except FileNotFoundError:
        return "File not found", 404


if __name__ == "__main__":
    app.run(debug=True)
