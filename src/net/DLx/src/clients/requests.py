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

class Client:
    def download(self, url, local_path):
        response = requests.get(url)
        if response.status_code == 200:
            with open(local_path, 'wb') as output_file:
                output_file.write(response.content)
        else:
            print(f"Failed to download {url}. Status code: {response.status_code}")
