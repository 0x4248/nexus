from fastapi import Request

STYLE = """
<style>
html, body {
  background: #000000;
  color: #ffffff;
  font-family: monospace;
  margin: 0;
  padding: 0.5em;
}

a {
  color: #4da6ff;
  text-decoration: none;
}

a:hover {
  text-decoration: underline;
}

.header {
  margin-bottom: 0.5em;
}

hr {
  border: none;
  border-top: 1px solid #444;
  margin: 0.5em 0;
}

pre {
  white-space: pre-wrap;
}

input, textarea {
  background: #000000;
  color: #ffffff;
  border: 1px solid #444;
  font-family: monospace;
  padding: 0.2em;
}

input[type=submit] {
  width: auto;
}

textarea {
  width: 100%;
  height: 6em;
}

.center {
  text-align: center;
}
</style>
"""

def layout(request: Request, title: str, buttons: list[tuple[str, str]], header_html: str | None, body_html: str) -> str:
    nav = " ".join(f"<a href='{href}'>[{label}]</a>" for label, href in buttons)
    header = header_html if header_html is not None else f"<div class='header'>{nav} {title}</div>"
    return f"""
    <html>
      <head>{STYLE}</head>
      <body>
        {header}
        <hr />
        {body_html}
      </body>
    </html>
    """
