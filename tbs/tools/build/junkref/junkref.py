# SPDX-License-Identifier: GPL-3.0
# Junkref
#
# Junkref.py
# Fetch stuff from the Junkdrawer
#
# COPYRIGHT NOTICE
# Copyright (C) 2025-2026 0x4248 and contributors
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the license is not changed.


import os
import shutil
import subprocess
import sys
import zipfile

import requests


def checkDirs():
    try:
        os.chdir("..")
        if not os.path.exists("JunkDrawer"):
            raise FileNotFoundError
        os.chdir("Nexus")
    except FileNotFoundError:
        print("Missing JunkDrawer!")
        print(
            "Your system should look like\n",
            "JunkDrawer\\\n",
            "Nexus\\\n",
            "\ttools\\build\\junkref.py\\\n",
        )
        exit(1)


def readFile(fileName):
    with open(fileName, "r") as file:
        return [
            line.strip()
            for line in file.readlines()
            if line.strip() and not line.startswith("#")
        ]


def fetchFromURL(url, dest):
    response = requests.get(url)
    response.raise_for_status()
    with open(dest, "wb") as f:
        f.write(response.content)


def runCommand(cmd):
    subprocess.run(cmd, shell=True, check=True)


def unzipFile(zipPath, extractTo):
    with zipfile.ZipFile(zipPath, "r") as zip_ref:
        zip_ref.extractall(extractTo)


def printMessage(msg):
    print(msg)


def processLine(line, projDir, mkdirs, fetch, store):
    if line.startswith("-") and line == "-ngit":
        return False

    if line.startswith("@"):
        dir_path = line[1:].strip()
        mkdirs.append(dir_path)
        path = os.path.join(projDir, dir_path)
        if not os.path.exists(path):
            os.makedirs(path)

    elif line.startswith("$"):
        cmd = line[1:].strip()
        runCommand(cmd)

    elif line.startswith("&"):
        zip_file = line[1:].strip()
        unzipFile(os.path.join(projDir, zip_file), projDir)

    elif line.startswith("*"):
        msg = line[1:].strip()
        printMessage(msg)

    elif ">" in line:
        parts = line.split(">")
        left = parts[0].strip().lstrip("/")
        right = parts[1].strip()
        fetch.append(left)
        store.append(right)
        os.chdir("..")
        dest_path = os.path.join(projDir, right)
        if left.startswith("http://") or left.startswith("https://"):
            fetchFromURL(left, dest_path)
        else:
            shutil.copyfile(os.path.join("JunkDrawer", left), dest_path)

    return True


def generateGitignore(projDir, store, mkdirs):
    path = os.path.join(projDir, ".gitignore")
    mode = "a" if os.path.exists(path) else "w"
    with open(path, mode) as f:
        for s in store:
            f.write(s + "\n")
        for d in mkdirs:
            f.write(d + "\n")


def parse(fileName, projDir):
    fetch, store, mkdirs = [], [], []
    gitignore = True
    checkDirs()
    lines = readFile(fileName)
    for line in lines:
        result = processLine(line, projDir, mkdirs, fetch, store)
        if result is False:
            gitignore = False
    if gitignore:
        generateGitignore(projDir, store, mkdirs)


def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} path/to/file/.junkref")
        sys.exit(1)
    junkref_file = sys.argv[1]
    projDir = os.path.join("Nexus/", junkref_file[:-8])
    parse(junkref_file, projDir)


if __name__ == "__main__":
    main()
