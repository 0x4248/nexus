#!/usr/bin/env python3
"""
Felix Menu - a dialog(1)-based launcher.
Requires the `dialog` binary (Debian/Ubuntu).
Usage: ./dialog_menu.py [menu.json]
Menu format: JSON list of items where each item is:
  {"title": "Name", "cmd": "sh command", "help": "text", "submenu": [ ... ]}

Keys:
  Enter  - select item / run command
  Help   - show help text for the highlighted item (if any)
  Esc    - go back / exit
"""

import json
import os
import shutil
import subprocess
import sys
import tempfile

APP_TITLE = 'Felix Menu'
GREEN = '\033[32m'
RED   = '\033[31m'
RESET = '\033[0m'


def setup_theme():
    conf = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'dialog.conf')
    if not os.path.exists(conf):
        sys.exit(f'Error: theme file not found: {conf}')
    os.environ['DIALOGRC'] = conf


def check_dialog():
    if shutil.which('dialog') is None:
        print('dialog binary not found. Install with: sudo apt install dialog')
        sys.exit(1)


def load_menu(path):
    with open(path, 'r', encoding='utf-8') as f:
        return json.load(f)


def item_label(it):
    base = it.get('title') or it.get('label') or '(no title)'
    if 'submenu' in it:
        return base + '  >'
    return base


def dialog_menu(title, items):
    tags = []
    mapping = {}
    for i, it in enumerate(items, 1):
        tag = str(i)
        tags.extend([tag, item_label(it)])
        mapping[tag] = it

    height      = max(10, min(20, len(items) + 6))
    width       = 64
    list_height = min(len(items), height - 6)

    fd, out_path = tempfile.mkstemp(prefix='felix_choice_')
    os.close(fd)

    try:
        cmd = [
            'dialog',
            '--output-fd', '1',
            '--extra-button', '--extra-label', 'Help',
            '--ok-label', 'Select',
            '--cancel-label', 'Back',
            '--menu', title,
            str(height), str(width), str(list_height),
        ] + tags

        with open(out_path, 'w') as out_f, open('/dev/tty', 'r') as tty_in:
            proc = subprocess.run(cmd, stdin=tty_in, stdout=out_f, stderr=None)

        with open(out_path, 'r') as out_f:
            choice = out_f.read().strip()

    finally:
        try:
            os.unlink(out_path)
        except OSError:
            pass

    return proc.returncode, choice, mapping


def run_menu(items, title=APP_TITLE):
    while True:
        rc, choice, mapping = dialog_menu(title, items)

        if rc == 1 or rc == 255:
            return

        sel = mapping.get(choice)
        if sel is None:
            return

        if rc == 3:
            help_text = sel.get('help', '').strip()
            if help_text:
                show_msg(title='Help - ' + (sel.get('title') or ''), text=help_text)
            else:
                show_msg(title='Help', text='No help available for this item.')
            continue

        if 'submenu' in sel:
            run_menu(sel['submenu'], title=sel.get('title', APP_TITLE))
            continue

        if 'cmd' not in sel and 'help' in sel:
            show_msg(
                title=sel.get('title', 'Info'),
                text='Press the Help button to read its description.',
            )
            continue

        cmd_str = sel.get('cmd')
        if cmd_str:
            subprocess.run(['clear'])
            result = subprocess.run(cmd_str, shell=True)
            print()
            if result.returncode == 0:
                print(GREEN + 'Command finished successfully.' + RESET)
            else:
                print(RED + f'Command exited with code {result.returncode}.' + RESET)
            input('\nPress Enter to return to Felix Menu...')


def show_msg(title='Info', text=''):
    with open('/dev/tty', 'r') as tty_in:
        subprocess.run(['dialog', '--msgbox', text, '12', '64'], stdin=tty_in, stderr=None)


def main():
    check_dialog()
    setup_theme()
    path = sys.argv[1] if len(sys.argv) > 1 else 'menu.json'
    if not os.path.exists(path):
        print('menu file not found:', path)
        sys.exit(2)
    try:
        menu = load_menu(path)
    except Exception as e:
        print('Failed to load menu:', e)
        sys.exit(1)
    run_menu(menu, title=APP_TITLE)


if __name__ == '__main__':
    main()