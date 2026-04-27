import os

cwd = os.getcwd()
dirs = []
head = """CompileFlags:
  Add:
"""

with open("tools/reflists/include.ref", "r") as f:
    for line in f.readlines():
        if line.startswith("#"):
            continue
        if line.startswith("\n"):
            continue
        dirs.append('    - "-I' + os.path.join(cwd, line.replace("\n", "")) + '"')

with open(".clangd", "w") as clf:
    clf.write(head)
    for dir in dirs:
        clf.write(dir + "\n")
