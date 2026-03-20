PANDOC ?= pandoc
BUILDDIR = build
CSS = docs/_static/whitepaper.css
TITLE = The 0x4248 Linux Reference Manual
SUBTITLE = A reference for GNU/Linux on x86-64 systems
AUTHOR = 0x4248
BUILD_DATE = $(shell date -u +"%Y-%m-%d")
HTMLDIR = $(BUILDDIR)/html
COVER_ENABLE ?= 1

CHAPTERS = $(sort $(shell find docs/chapters -name '*.md'))

CHAPTER_HTMLS = $(patsubst docs/chapters/%.md,$(HTMLDIR)/chapters/%.html,$(CHAPTERS))
HTML_INDEX = $(HTMLDIR)/index.html

.PHONY: html pdf docs pdfdocs clean

html: $(HTML_INDEX) $(CHAPTER_HTMLS)

pdf: $(BUILDDIR)/manual.pdf

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(HTMLDIR):
	mkdir -p $(HTMLDIR)

$(HTMLDIR)/chapters/%.html: docs/chapters/%.md | $(HTMLDIR)
	mkdir -p $(dir $@)
	$(PANDOC) --standalone --embed-resources --metadata title='$(TITLE)' --metadata subtitle='$(SUBTITLE)' --metadata author='$(AUTHOR)' --metadata date='$(BUILD_DATE)' --css=$(CSS) -o $@ $<

$(HTML_INDEX): $(CHAPTERS) $(CHAPTER_HTMLS) | $(HTMLDIR)
	{ \
		echo '# $(TITLE)'; \
		echo ''; \
		echo '## Contents'; \
		echo ''; \
		for file in $(CHAPTERS); do \
			rel=$${file#docs/chapters/}; \
			link=$${rel%.md}.html; \
			title=$$(grep -m1 '^# ' $$file | sed 's/^# //'); \
			if [ -z "$$title" ]; then title=$$rel; fi; \
			echo "- [$$title](chapters/$$link)"; \
		done; \
	} | $(PANDOC) --standalone --embed-resources --metadata title='$(TITLE)' --metadata subtitle='$(SUBTITLE)' --css=$(CSS) -o $@

$(BUILDDIR)/manual.pdf: $(CHAPTERS) | $(BUILDDIR)
	@# Resolve cover.pdf: prefer cover.html, fall back to cover.pdf in root
	@if [ "$(COVER_ENABLE)" = "1" ] && [ -f cover.html ]; then \
		$(PANDOC) --standalone -V papersize=a4 -V geometry:margin=1in -o $(BUILDDIR)/cover.pdf cover.html; \
	elif [ "$(COVER_ENABLE)" = "1" ] && [ -f cover.pdf ]; then \
		cp cover.pdf $(BUILDDIR)/cover.pdf; \
	fi
	@# Build a before-body snippet that embeds the cover via pdfpages (no pdfunite)
	@if [ -f $(BUILDDIR)/cover.pdf ]; then \
		echo '\includepdf[pages=-]{$(BUILDDIR)/cover.pdf}' > $(BUILDDIR)/cover-include.tex; \
	else \
		printf '' > $(BUILDDIR)/cover-include.tex; \
	fi
	$(PANDOC) --toc --toc-depth=5 --top-level-division=chapter \
		-V documentclass=report \
		-V papersize=a4 \
		-V fontsize=11pt \
		-V geometry:margin=1in \
		-V fontfamily=tgtermes \
		--include-in-header=docs/_static/pdf-header.tex \
		--include-before-body=$(BUILDDIR)/cover-include.tex \
		--metadata title='$(TITLE)' \
		--metadata subtitle='$(SUBTITLE)' \
		--metadata author='$(AUTHOR)' \
		--metadata date='$(BUILD_DATE)' \
		-o $@ $(CHAPTERS)

docs: html

pdfdocs: pdf

clean:
	rm -rf $(BUILDDIR)
