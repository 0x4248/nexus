import os
import sys


RESET = "\033[0m"
BOLD = "\033[1m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
RED = "\033[31m"
BLUE = "\033[34m"


def use_color() -> bool:
    return sys.stdout.isatty() and os.environ.get("NO_COLOR") is None


def style(text: str, color: str = "", bold: bool = False) -> str:
    if not use_color():
        return text
    prefix = ""
    if bold:
        prefix += BOLD
    if color:
        prefix += color
    return f"{prefix}{text}{RESET}"


def info(message: str) -> None:
    print(f"[{style('>', BLUE)}] {message}")


def ok(message: str) -> None:
    print(f"[{style('+', GREEN)}] {message}")


def warn(message: str) -> None:
    print(f"[{style('!', YELLOW)}] {message}")


def fail(message: str) -> None:
    print(f"[{style('x', RED)}] {message}", file=sys.stderr)
