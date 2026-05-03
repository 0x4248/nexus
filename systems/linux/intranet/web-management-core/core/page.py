from fastapi import Request
from fastapi.responses import HTMLResponse, RedirectResponse
from core.layout import layout

# ---- Page helpers ----

def page(request: Request, title: str, buttons, body: str, header: str | None = None) -> HTMLResponse:
    return HTMLResponse(layout(request, title, buttons, header, body))

# ---- Page types ----



def menu(request: Request, title: str, entries: list[tuple[str, str]]):
    lines = []
    for label, href in entries:
        lines.append(f"<a href='{href}'>[{label}]</a>")
    body = "<pre>" + "\n".join(lines) + "</pre>"
    return page(request, title, [("BACK", "/")], body)


def message(request: Request, title: str, text: str, ok_href: str = "/"):
    body = f"""
    <div class='center'>
      <pre>{text}</pre>
      <a href='{ok_href}'>[ OK ]</a>
    </div>
    """
    return page(request, title, [("BACK", ok_href)], body)


def form(request: Request, title: str, action: str, fields: list[dict], error: str | None = None):
    rows = []
    for f in fields:
        if f.get("type") == "textarea":
            rows.append(f"<label>{f['name']}</label><br/><textarea name='{f['name']}'></textarea>")
        else:
            rows.append(f"<label>{f['name']}</label><br/><input name='{f['name']}' />")
    err = f"<pre>ERROR: {error}</pre>" if error else ""
    body = f"""
    {err}
    <form method='post' action='{action}'>
      {'<br/>'.join(rows)}
      <br/><input type='submit' value='SUBMIT' />
    </form>
    """
    return page(request, title, [("BACK", "/")], body)


def static_html(request: Request, title: str, html: str):
    return page(request, title, [("BACK", "/")], html)