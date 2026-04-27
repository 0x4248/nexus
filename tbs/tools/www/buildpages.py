#!/usr/bin/env python3
import html
import os
import shutil

try:
    import markdown
except ImportError:
    os.system("pip3 install markdown")
    import markdown

from markdown.core import Markdown

ROOT = os.getcwd()
SRC_DIR = ROOT
OUT_DIR = os.path.join(ROOT, "pages")

IGNORE_DIRS = [".git", "build", "data", "tmp", "bin", "__pycache__", "pages"]
LOGS_DIR = os.path.join(OUT_DIR, "logs")

TEMPLATE = """<!doctype html>
<html>
    <head>
        <title>{title}</title>
        <link rel="stylesheet" href="/nexus/misc/www/css/basic_mono.css"/>
        <link rel="stylesheet" href="/nexus/misc/www/css/Atlas/main.css"/>
        <style>
        .ln {{
            display: inline-block;
            opacity: 0.5;
            margin-right: 10px;
            text-align: right;
            width: 3em;
            user-select: none;
        }}
        body.show-lines .ln {{
            display: inline-block;
        }}
        body.hide-lines .ln {{
            display: none;
        }}
        </style>

        <script>
        function toggleLineNumbers() {{
            document.body.classList.toggle("show-lines");
            document.body.classList.toggle("hide-lines");
        }}
        </script>
    </head>
    <body class="show-lines">
        <pre>
<h1>{title}</h1>
{breadcrumbs}
{stats}{toolbar}
{separator}
{content}
<div class="separator">[{endtag}]</div>
(C) 2025 0x4248
(C) 2025 4248 Media and 4248 Systems, All part of 0x4248
See LICENCE files for more information. Not all files are by 0x4248 always check Licencing.
        </pre>
    </body>
</html>
"""


def ensure_dir(path):
    os.makedirs(path, exist_ok=True)


def write_file(path, content):
    ensure_dir(os.path.dirname(path))
    with open(path, "w", encoding="utf-8") as f:
        f.write(content)


def run_cmd(cmd):
    return os.popen(cmd).read().strip()


def write_log(name, content):
    ensure_dir(LOGS_DIR)
    with open(os.path.join(LOGS_DIR, name), "w", encoding="utf-8") as f:
        f.write(content)


def compute_breadcrumbs(rel_path):
    if not rel_path:
        return "Home"

    parts = rel_path.split("/")
    crumbs = ['<a href="/index.html">Home</a>']

    accum = ""
    for p in parts:
        accum = os.path.join(accum, p)
        crumbs.append(f'<a href="/Nexus/{accum}/index.html">{p}</a>')

    return " / ".join(crumbs)


def is_binary(filepath, sample_size=2048):
    try:
        with open(filepath, "rb") as f:
            chunk = f.read(sample_size)
            if not chunk:
                return False
            if b"\0" in chunk:
                return True

            try:
                chunk.decode("utf-8")
                return False
            except UnicodeDecodeError:
                return True
    except Exception:
        return True


def make_file_page(src_path, out_path_rel):
    filename = os.path.basename(src_path)
    rel_path_dir = os.path.dirname(out_path_rel)
    breadcrumbs = compute_breadcrumbs(rel_path_dir)
    filesize = os.path.getsize(src_path)

    repo = "0x4248/Nexus"
    repo_path = out_path_rel.replace(".html", "")
    github_url = f"https://github.com/{repo}/blob/main/{repo_path}"
    github_raw = f"https://raw.githubusercontent.com/{repo}/main/{repo_path}"
    github_proxy_raw = f"https://proxy.0x4248.dev/https://raw.githubusercontent.com/{repo}/main/{repo_path}"
    github_search = f"https://github.com/search?q=repo%3A0x4248%2FNexus++path%3A**%2F{filename}&type=code"
    ext = os.path.splitext(filename)[1].lower()

    if ext in [".md", ".markdown", ".html", ".htm"]:
        with open(src_path, "r", encoding="utf-8", errors="replace") as f:
            raw = f.read()

        if ext in [".md", ".markdown"]:
            rendered = markdown.markdown(raw)
        else:
            rendered = raw

        stats = f"Rendered HTML view | Size: {filesize} bytes\n"
        toolbar = (
            f'<a href="{filename}" download>[Download]</a> '
            f'<a href="{github_url}">[Show on GitHub]</a> '
            f'<a href="{github_search}">[Search similar files]</a> '
            f'<a href="{github_raw}">[Raw]</a> '
            f'<a href="{github_proxy_raw}">[Raw (proxy)]</a> '
        )
        content = f"<div class='rendered-html'>{rendered}</div>"

    elif is_binary(src_path):
        stats = f"Size: {filesize} bytes\n"
        toolbar = (
            f'<a href="{filename}" download>[Download]</a> '
            f'<a href="{github_url}">[Show on GitHub]</a> '
            f'<a href="{github_search}">[Search similar files]</a> '
            f'<a href="{github_raw}">[Raw]</a> '
            f'<a href="{github_proxy_raw}">[Raw (proxy)]</a> '
        )
        content = (
            "<p style='text-align:center;'>Binary file cannot be displayed.</p>\n"
            f'<a style="text-align:center;display:block;" href="{filename}" download>[Download]</a>\n'
        )

    else:
        with open(src_path, "r", encoding="utf-8", errors="replace") as f:
            text = f.read()

        escaped = html.escape(text)
        lines = escaped.split("\n")

        numbered = "\n".join(
            f'<span class="ln">{i + 1}</span>{line}' for i, line in enumerate(lines)
        )

        stats = f"Lines: {text.count('\\n') + 1} | Size: {filesize} bytes\n"
        toolbar = (
            f'<a href="{filename}" download>[Download]</a> '
            f'<a href="{github_url}">[Show on GitHub]</a> '
            f'<a href="{github_search}">[Search similar files]</a> '
            f'<a href="{github_raw}">[Raw]</a> '
            f'<a href="{github_proxy_raw}">[Raw (proxy)]</a> '
        )
        content = numbered

    page = TEMPLATE.format(
        title="Atlas - " + filename,
        breadcrumbs=breadcrumbs,
        stats=stats,
        toolbar=toolbar,
        separator='<div class="separator">[FILE BEGIN]</div>',
        endtag="FILE END",
        content=content,
    )

    write_file(os.path.join(OUT_DIR, out_path_rel), page)


def make_directory_index(dir_path, rel_path):
    src_index = os.path.join(SRC_DIR, rel_path, "index.html")
    if os.path.exists(src_index):
        dst_index = os.path.join(OUT_DIR, rel_path, "index.html")
        ensure_dir(os.path.dirname(dst_index))
        shutil.copy2(src_index, dst_index)
        return

    entries = sorted(os.listdir(dir_path))

    intro = ""

    readme_paths = [
        os.path.join(dir_path, "README.txt"),
        os.path.join(dir_path, "README"),
        os.path.join(dir_path, "README.html"),
        os.path.join(dir_path, "README.md"),
    ]
    for readme_path in readme_paths:
        if rel_path == "":
            with open("pages/README.txt", "r", encoding="utf-8", errors="replace") as f:
                intro_text = f.read()
                intro = intro_text + "\n\n<hr>\n\n"
        elif os.path.exists(readme_path):
            with open(readme_path, "r", encoding="utf-8", errors="replace") as f:
                if readme_path.endswith(".html"):
                    intro_text = f.read()
                elif readme_path.endswith(".md"):
                    intro_text = markdown.markdown(f.read())
                else:
                    intro_text = html.escape(f.read())
                intro = intro_text + "\n\n<hr>\n\n"

    lines = []
    if rel_path != "":
        lines.append('* <a href="../">../</a>')

    for name in entries:
        if any(name.startswith(d) for d in IGNORE_DIRS):
            continue

        full = os.path.join(dir_path, name)
        if os.path.isdir(full):
            lines.append(f'* <a href="{name}/index.html">{name}/</a>')
        else:
            lines.append(f'* <a href="{name}.html">{name}</a>')

    content = intro + "\n".join(lines)

    breadcrumbs = compute_breadcrumbs(rel_path)
    title_path = "Home" if rel_path == "" else rel_path

    page = TEMPLATE.format(
        title="Atlas - " + title_path,
        breadcrumbs=breadcrumbs,
        stats="",
        toolbar="",
        separator='<div class="separator">[DIR BEGIN]</div>',
        endtag="DIR END",
        content=content,
    )

    write_file(os.path.join(OUT_DIR, rel_path, "index.html"), page)


def write_commit_page(commit, outdir):
    # Commit metadata
    meta = run_cmd(f"git show -s --format='%H%n%an%n%ae%n%at%n%s' {commit}").split("\n")
    full_hash, author, author_email, timestamp, subject = meta
    timestamp = int(timestamp)

    # Diff
    diff = run_cmd(f"git show --color=never {commit}")
    diff_html = f"<pre>{html.escape(diff)}</pre>"

    page = TEMPLATE.format(
        title=f"Commit {full_hash}",
        breadcrumbs="Commits",
        stats="",
        toolbar="",
        separator='<div class="separator">[COMMIT BEGIN]</div>',
        endtag="COMMIT END",
        content=f"""
{diff_html}
""",
    )

    write_file(os.path.join(outdir, f"{commit}.html"), page)


def write_commit_index(outdir):
    commits = run_cmd("git log --after='2025-12-09' --format='%H%x00%at%x00%an%x00%s'").split("\n")

    lines = []
    for entry in commits:
        if not entry.strip():
            continue

        h, ts, author, subj = entry.split("\x00", 3)
        when = int(ts)

        lines.append(
            f"* {when} <a href='{h}.html'>{html.escape(subj)}</a> - {html.escape(author)}"
        )

    content = "\n".join(lines)

    page = TEMPLATE.format(
        title="Commit Log",
        breadcrumbs="Commits",
        stats="",
        toolbar="",
        separator='<div class="separator">[LOG BEGIN]</div>',
        endtag="LOG END",
        content=content,
    )

    write_file(os.path.join(outdir, "index.html"), page)


def generate_lore_like_log():
    lore_dir = os.path.join(OUT_DIR, "lore")
    ensure_dir(lore_dir)

    # Get commit list in reverse chronological
    commits = run_cmd("git log --format='%H'").splitlines()

    for c in commits:
        write_commit_page(c, lore_dir)

    write_commit_index(lore_dir)


def run_prebuild_commands():
    print("Running prebuild commands...")
    os.system("git clone https://github.com/0x4248/JunkDrawer ext/JunkDrawer")
    os.system("git submodule init")
    os.system("git submodule update --recursive --remote")
    print("Prebuild complete.\n")


def generate_logs():
    print("Generating logs...")

    ensure_dir(LOGS_DIR)
    generate_lore_like_log()

    gitlog = run_cmd("git log --stat --decorate --color=never")
    write_log("gitlog.txt", gitlog)

    gitlog_short = run_cmd("git log --oneline --decorate --color=never")
    write_log("gitlog_short.txt", gitlog_short)

    gitstats = run_cmd("git shortlog -sn --all --no-merges")
    write_log("gitstats.txt", gitstats)

    branch = run_cmd("git rev-parse --abbrev-ref HEAD")
    write_log("branch.txt", branch + "\n")

    last_commit = run_cmd("git log -1 --pretty=fuller --color=never")
    write_log("last_commit.txt", last_commit)

    submodules = run_cmd("git submodule status --recursive")
    write_log("submodules.txt", submodules)

    tree = run_cmd("git ls-tree -r --name-only HEAD")
    write_log("gittree.txt", tree)

    tree = run_cmd("git ls-tree -l -r HEAD")
    write_log("gittreelong.txt", tree)

    tree = run_cmd("tree")
    write_log("tree.txt", tree)

    print("Logs generated.\n")


def copy_repo_to_pages():
    print("Copying repository to /pages...")

    for dirpath, dirs, files in os.walk(SRC_DIR):
        if dirpath.startswith(OUT_DIR):
            continue

        dirs[:] = [d for d in dirs if d not in IGNORE_DIRS]

        rel = os.path.relpath(dirpath, SRC_DIR)
        if rel == ".":
            rel = ""

        for file in files:
            if file.startswith(".") or file == "buildpages.py":
                continue

            src_file = os.path.join(dirpath, file)
            dst_file = os.path.join(OUT_DIR, rel, file)

            ensure_dir(os.path.dirname(dst_file))
            shutil.copy2(src_file, dst_file)

    print("Copy complete.\n")


def generate_all_pages():
    print("Generating index and file pages...")

    for dirpath, dirs, files in os.walk(OUT_DIR):
        rel = os.path.relpath(dirpath, OUT_DIR)
        if rel == ".":
            rel = ""

        if rel.startswith("lore"):
            continue
        # Directory index
        make_directory_index(dirpath, rel)

        # File pages
        for file in files:
            if file.startswith("."):
                continue

            src_path = os.path.join(dirpath, file)

            if file == "index.html":
                continue

            out_rel = os.path.join(rel, file) + ".html"
            make_file_page(src_path, out_rel)

    print("HTML generation complete.\n")


def main():
    if os.path.exists(OUT_DIR):
        shutil.rmtree(OUT_DIR)
    ensure_dir(OUT_DIR)

    shutil.copy("doc/Atlas/welcome.txt", "pages/README.txt")

    run_prebuild_commands()
    copy_repo_to_pages()
    generate_logs()
    generate_all_pages()


if __name__ == "__main__":
    main()
