# SPDX-License-Identifier: GPL-3.0
# LWPS - Lightweight PHP Server
#
# Main indexer and server.
# main.py
#
# COPYRIGHT NOTICE
# Copyright (C) 2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

import os
import fastapi
import uvicorn
from fastapi.responses import FileResponse
import subprocess


BASE_DIR = os.path.join(os.path.dirname(__file__), '..', 'server')
app = fastapi.FastAPI()

env = os.environ.copy()
env['SERVER_SOFTWARE'] = 'LWPS'
env['DOCUMENT_ROOT'] = BASE_DIR


def convert_php(file_path: str) -> str:
    """Convert a PHP file to HTML using the PHP CLI."""
    try:
        result = subprocess.run(
            ['php', file_path],
            capture_output=True,
            text=True,
            check=True,
            env=env
        )
        return result.stdout
    except subprocess.CalledProcessError as e:
        error_page_path = os.path.join(os.path.dirname(__file__), 'static', '500.html')
        with open(error_page_path, 'r') as f:
            error_page = f.read()
        return error_page.replace('{{error}}', str(e.stderr))
    except FileNotFoundError:
        error_page_path = os.path.join(os.path.dirname(__file__), 'static', '500.html')
        with open(error_page_path, 'r') as f:
            error_page = f.read()
        return error_page.replace('{{error}}', 'PHP CLI not found. Please install PHP.')
    
@app.get("/{full_path:path}")
async def serve_file(full_path: str):
    file_path = os.path.join(BASE_DIR, full_path)
    
    if os.path.isdir(file_path):
        index_php = os.path.join(file_path, 'index.php')
        index_html = os.path.join(file_path, 'index.html')
        if os.path.isfile(index_php):
            file_path = index_php
        elif os.path.isfile(index_html):
            file_path = index_html
        else:
            not_found_page_path = os.path.join(os.path.dirname(__file__), 'static', '404.html')
            return FileResponse(not_found_page_path, status_code=404)
    
    if file_path.endswith('.php') and os.path.isfile(file_path):
        html_content = convert_php(file_path)
        return fastapi.responses.HTMLResponse(content=html_content)
    
    if os.path.isfile(file_path):
        return FileResponse(file_path)
    
    not_found_page_path = os.path.join(os.path.dirname(__file__), 'static', '404.html')
    return FileResponse(not_found_page_path, status_code=404)

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)