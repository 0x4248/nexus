PANDOC ?= pandoc
BUILDDIR = build
CSS = docs/_static/whitepaper.css
TITLE = The 0x4248 Linux Reference Manual
SUBTITLE = For x86-64 Systems
HTMLDIR = $(BUILDDIR)/html

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
	$(PANDOC) --standalone --embed-resources --metadata title='$(TITLE)' --metadata subtitle='$(SUBTITLE)' --css=$(CSS) -o $@ $<

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
	$(PANDOC) --toc --top-level-division=chapter \
		-V documentclass=report \
		-V papersize=a4 \
		-V fontsize=11pt \
		-V geometry:margin=1in \
		-V fontfamily=tgtermes \
		--metadata title='$(TITLE)' \
		--metadata subtitle='$(SUBTITLE)' \
		-o $(BUILDDIR)/_content.pdf $(CHAPTERS)
	@if [ -f cover.pdf ]; then \
		pdfunite cover.pdf $(BUILDDIR)/_content.pdf $@ && rm $(BUILDDIR)/_content.pdf; \
	else \
		mv $(BUILDDIR)/_content.pdf $@; \
	fi

docs: html

pdfdocs: pdf

clean:
	rm -rf $(BUILDDIR)
