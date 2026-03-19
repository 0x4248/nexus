PANDOC      ?= pandoc
SRC_DIR     ?= docs
BUILD_DIR   ?= build
TITLE       ?= 4248's Linux Reference Manual
#PDF_ENGINE  ?= $(shell for e in tectonic xelatex lualatex pdflatex weasyprint prince; do \
#                 command -v $$e >/dev/null 2>&1 && { echo $$e; break; }; done)

PDF_ENGINE  = xelatex
CSS          := $(SRC_DIR)/assets/style.css
FRONT_MATTER := $(SRC_DIR)/index.md
CHAPTERS     := $(sort $(shell find $(SRC_DIR)/chapters -type f -name '*.md'))
BOOK_SOURCES := $(FRONT_MATTER) $(CHAPTERS)

# Per-chapter HTML: docs/chapters/XX-name.md -> build/html/chapters/XX-name.html
CHAPTER_HTMLS := $(patsubst $(SRC_DIR)/chapters/%.md,$(BUILD_DIR)/html/chapters/%.html,$(CHAPTERS))

HTML_INDEX := $(BUILD_DIR)/html/index.html
PDF_OUT    := $(BUILD_DIR)/pdf/manual.pdf

.PHONY: help all docs htmldocs pdfdocs cleandocs clean check-pandoc check-pdf-engine listdocs

# ── Help ─────────────────────────────────────────────────────────────────────
help:
	@echo "Targets:"
	@echo "  make htmldocs   - Build per-chapter HTML manual"
	@echo "  make pdfdocs    - Build PDF manual (new page per chapter)"
	@echo "                    override engine: make pdfdocs PDF_ENGINE=xelatex"
	@echo "  make docs       - Build both"
	@echo "  make listdocs   - Show input source files"
	@echo "  make cleandocs  - Remove generated output"

all: docs
docs: htmldocs pdfdocs

# ── Dependency checks ─────────────────────────────────────────────────────────
check-pandoc:
	@command -v $(PANDOC) >/dev/null 2>&1 || { \
		echo "error: pandoc is not installed"; exit 1; \
	}

check-pdf-engine:
	@test -n "$(PDF_ENGINE)" || { \
		echo "error: no PDF engine found"; \
		echo "install one of: tectonic, xelatex, lualatex, pdflatex, weasyprint, prince"; \
		echo "or specify: make pdfdocs PDF_ENGINE=xelatex"; \
		exit 1; \
	}
	@command -v $(PDF_ENGINE) >/dev/null 2>&1 || { \
		echo "error: PDF engine '$(PDF_ENGINE)' is not installed"; \
		echo "specify another: make pdfdocs PDF_ENGINE=xelatex"; \
		exit 1; \
	}

listdocs:
	@printf '%s\n' $(BOOK_SOURCES)

# ── HTML: one file per chapter ────────────────────────────────────────────────
#
# Each chapter is built as a self-contained HTML file (CSS embedded inline)
# so the output in build/html/chapters/ is portable with no external deps.

$(BUILD_DIR)/html/chapters/%.html: $(SRC_DIR)/chapters/%.md $(CSS) | check-pandoc
	@mkdir -p $(dir $@)
	$(PANDOC) $< \
		--standalone \
		--embed-resources \
		--toc \
		--number-sections \
		--metadata title="$(TITLE)" \
		--css=$(CSS) \
		-o $@

# Index page: intro content + auto-generated chapter listing
$(HTML_INDEX): $(FRONT_MATTER) $(CHAPTERS) $(CSS) | check-pandoc
	@mkdir -p $(dir $@)
	@{ \
		cat $(FRONT_MATTER); \
		echo ""; \
		echo "## Chapters"; \
		for f in $(CHAPTERS); do \
			rel=$${f#$(SRC_DIR)/chapters/}; \
			base=$${rel%.md}; \
			title=$$(grep -m1 '^# ' $$f | sed 's/^# //'); \
			echo "- [$$title](chapters/$$base.html)"; \
		done; \
	} | $(PANDOC) \
		--standalone \
		--embed-resources \
		--metadata title="$(TITLE)" \
		--css=$(CSS) \
		-o $@

htmldocs: $(HTML_INDEX) $(CHAPTER_HTMLS)
	@echo "HTML built: $(BUILD_DIR)/html/"

# ── PDF: new page per chapter ─────────────────────────────────────────────────
#
# --top-level-division=chapter maps each top-level # heading to a LaTeX
# \chapter{}, which starts a new page in the 'report' document class.

$(PDF_OUT): $(BOOK_SOURCES) | check-pandoc check-pdf-engine
	@mkdir -p $(dir $@)
	$(PANDOC) $(BOOK_SOURCES) \
		--toc \
		--number-sections \
		--top-level-division=chapter \
		--metadata title="$(TITLE)" \
		-V documentclass=report \
		-V geometry:margin=1in \
		-V colorlinks=true \
		-V linkcolor=blue \
		--pdf-engine=$(PDF_ENGINE) \
		-o $@

pdfdocs: $(PDF_OUT)
	@echo "PDF built: $(PDF_OUT)"

# ── Cleanup ───────────────────────────────────────────────────────────────────
cleandocs:
	rm -rf $(BUILD_DIR)/html $(BUILD_DIR)/pdf

clean: cleandocs
