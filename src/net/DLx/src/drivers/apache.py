# SPDX-License-Identifier: GPL-3.0
# DLX
#
# Bulk download tool
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.
#
# This software is free and open source. Licensed under the GNU general
# public license version 3.0 as published by the Free Software Foundation.

import requests
from bs4 import BeautifulSoup
import urllib.parse

def fetch_files_recursive(base_url, current_path="", visited=None):
    if visited is None:
        visited = set()

    url = urllib.parse.urljoin(base_url, current_path)
    normalized_path = urllib.parse.urljoin(base_url, current_path).rstrip("/")

    if normalized_path in visited:
        return []

    visited.add(normalized_path)
    print(f"Fetching index of {url}")

    response = requests.get(url)
    if response.status_code != 200:
        print(f"Failed to retrieve {url}. Status code: {response.status_code}")
        return []

    soup = BeautifulSoup(response.content, "html.parser")
    files = []

    for item in soup.find_all("a"):
        href = item.get("href")

        if href is None or href.startswith("#") or href.startswith("?") or href.startswith("../") or href.startswith("mailto:") or href.startswith("javascript:") or href.startswith("ftp://") or href.endswith("/."):
            continue

        full_href = urllib.parse.urljoin(url, href)

        parsed_href = urllib.parse.urlparse(full_href)
        relative_path = parsed_href.path[len(urllib.parse.urlparse(base_url).path) :].lstrip("/")

        if href.endswith("/"):
            files.extend(fetch_files_recursive(base_url, relative_path, visited))
        else:
            files.append((full_href, relative_path))

    return files

class Driver:
    def fetch(base_url, output):
        result = fetch_files_recursive(base_url)
        out = open(output, "w")
        for full_url, relative_path in result:
            out.write(f"{full_url}\t{relative_path}\n")
