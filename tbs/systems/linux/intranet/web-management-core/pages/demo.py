from fastapi import APIRouter, Request, Form
from fastapi.responses import RedirectResponse
from core import page as p

router = APIRouter()

@router.get("/")
async def home(request: Request):
    return p.menu(
        request,
        title="PAGE: DEMO",
        entries=[
            ("MESSAGE", "/message"),
            ("FORM", "/form"),
        ],
    )

@router.get("/message")
async def msg(request: Request):
    return p.message(request, "MESSAGE", "Hello from server", "/")

@router.get("/form")
async def form_page(request: Request):
    return p.form(
        request,
        title="DATA FORM",
        action="/form",
        fields=[
            {"name": "summary"},
            {"name": "details", "type": "textarea"},
        ],
    )

@router.post("/form")
async def form_submit(request: Request, summary: str = Form(...), details: str = Form(...)):
    if not summary:
        return p.form(
            request,
            title="DATA FORM",
            action="/form",
            fields=[
                {"name": "summary"},
                {"name": "details", "type": "textarea"},
            ],
            error="summary required",
        )
    return p.message(
        request,
        "FORM SUBMITTED",
        f"Summary: {summary}\nDetails: {details}",
        "/",
    )
 