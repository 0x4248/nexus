#!/usr/bin/env python3
import curses
import subprocess
import sys
from pathlib import Path

failedTargets = []


def load_targets(path):
    out = []
    for line in Path(path).read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split("\t")
        if len(parts) != 3:
            continue
        name, args, directory = parts
        args = None if args == "NULL" else args
        out.append((name, args, directory))
    return out


def build_target(name, args, directory):
    try:
        cmd = ["make"]
        if args:
            cmd.extend(args.split())
        subprocess.run(cmd, cwd=directory, check=True)
    except subprocess.CalledProcessError:
        failedTargets.append(directory)


def choose_target(stdscr, targets):
    curses.curs_set(0)
    idx = 0
    help_msg = "[↑/k] up   [↓/j] down   [Enter] build   [q] quit"

    while True:
        stdscr.clear()
        stdscr.addstr(0, 0, help_msg)

        for i, (name, _, _) in enumerate(targets):
            marker = "> " if i == idx else "  "
            stdscr.addstr(i + 2, 0, f"{marker}{name}")

        stdscr.refresh()

        key = stdscr.getch()
        if key in (curses.KEY_UP, ord("k")):
            idx = (idx - 1) % len(targets)
        elif key in (curses.KEY_DOWN, ord("j")):
            idx = (idx + 1) % len(targets)
        elif key in (curses.KEY_ENTER, 10, 13):
            return targets[idx]
        elif key in (ord("q"), 27):
            return None


def main():
    targets = load_targets("tools/reflists/makelist.ref")
    argOverride = ""
    if len(sys.argv) >= 2:
        name = sys.argv[1]
        if len(sys.argv) == 3:
            argOverride = sys.argv[2]
        if name == "ALL":
            for t_name, args, directory in targets:
                print(f"==> {t_name}")
                if argOverride == "":
                    build_target(name, args, directory)
                else:
                    build_target(name, argOverride, directory)
            return

        for t_name, args, directory in targets:
            if t_name == name:
                if argOverride == "":
                    build_target(name, args, directory)
                else:
                    build_target(name, argOverride, directory)
                return

        print(f"error: no target named '{name}'")
        sys.exit(1)

    # menu mode
    chosen = curses.wrapper(choose_target, targets)
    if chosen:
        name, args, directory = chosen
        build_target(name, args, directory)


if __name__ == "__main__":
    main()
    if len(failedTargets) != 0:
        print("----------[FAIL LOG]----------")
        print("The following failed:")
        for target in failedTargets:
            print("[X] " + target)
        exit(1)
