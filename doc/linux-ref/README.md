# 4248's Linux Reference Manual

Minimal Pandoc generator with Linux-kernel-style Make targets:

- `make htmldocs`
- `make pdfdocs`

## Prerequisites

- `pandoc`
- For PDF output: Pandoc uses a PDF engine backend (auto-detected: `tectonic`, `xelatex`, `lualatex`, `pdflatex`, `wkhtmltopdf`, `weasyprint`, or `prince`)

## Layout

- `docs/index.md` front matter and introduction
- `docs/chapters/**/*.md` chapter files (recursively discovered, built in lexicographic order)
- `build/html/index.html` generated HTML
- `build/pdf/manual.pdf` generated PDF

## Usage

```bash
make htmldocs
make pdfdocs
make pdfdocs PDF_ENGINE=xelatex
make docs      # both
make cleandocs
```

If your preferred engine is installed, you can force it via `PDF_ENGINE`.

## Add new chapters

Create files under `docs/chapters/` with numeric prefixes for stable ordering, for example:

- `docs/chapters/02-processes-and-scheduling.md`
- `docs/chapters/03-memory-management.md`
- `docs/chapters/bash/10-bash-overview.md`
- `docs/chapters/bash/20-expansions.md`

The `Makefile` auto-discovers these files recursively and preserves subfolders in HTML output, for example `build/html/chapters/bash/10-bash-overview.html`.
