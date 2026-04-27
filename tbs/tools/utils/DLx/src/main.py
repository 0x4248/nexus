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

import os
import importlib

import argparse

def download_files_from_tsv(tsv_path, output_dir, client):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    with open(tsv_path, 'r') as file:
        for line in file:
            full_url, relative_path = line.strip().split('\t')
            local_path = os.path.join(output_dir, relative_path)

            os.makedirs(os.path.dirname(local_path), exist_ok=True)

            print(f"Downloading {full_url} to {local_path}")
            client.download(full_url, local_path)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Fetch or download files from a TSV file')
    subparsers = parser.add_subparsers(dest='command', required=True)

    fetch_parser = subparsers.add_parser('fetch', help='Fetch files from a URL')
    fetch_parser.add_argument('url', help='The URL to fetch files from')
    fetch_parser.add_argument('driver', help='The driver to use for fetching files')

    download_parser = subparsers.add_parser('download', help='Download files from a TSV file')
    download_parser.add_argument('tsv_path', help='The path to the TSV file')
    download_parser.add_argument('output_dir', help='The directory to save the downloaded files')
    download_parser.add_argument('client_backend', help='The client backend to use for downloading files')

    args = parser.parse_args()

    if args.command == 'fetch':
        driver = getattr(importlib.import_module(f'drivers.{args.driver}'), 'Driver')
        driver.fetch(args.url, 'output.tsv')
        exit(0)

    if args.command == 'download':
        client = getattr(importlib.import_module(f'clients.{args.client_backend}'), 'Client')()
        download_files_from_tsv(args.tsv_path, args.output_dir, client)
        exit(0)