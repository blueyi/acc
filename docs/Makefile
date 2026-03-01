# Sphinx + Breathe doc build (run from repo root: make -C docs html)
SPHINXOPTS  ?=
SPHINXBUILD ?= sphinx-build
SOURCEDIR   = .
BUILDDIR    = _build
DOXYFILE    = Doxyfile

# Run Doxygen to generate XML for Breathe, then Sphinx
# Requires: pip install -r docs/requirements.txt (sphinx, breathe, myst-parser, sphinx-rtd-theme)
# Cleans _build/html before building so output is always up to date.
html:
	@rm -rf $(BUILDDIR)/html
	@python -c "import breathe" 2>/dev/null || { echo "Error: missing 'breathe'. Run: pip install -r docs/requirements.txt"; exit 1; }
	@echo "Running Doxygen (from repo root)..."
	cd .. && doxygen docs/$(DOXYFILE)
	$(SPHINXBUILD) -b html $(SPHINXOPTS) $(SOURCEDIR) $(BUILDDIR)/html
	@echo "Open $(BUILDDIR)/html/index.html"

# Build HTML in Chinese (uses *_zh.md; conf.py uses master_doc=index, so we copy index_zh.rst -> index.rst like RtD)
# Cleans _build/html/zh_CN before building. Restores index.rst after build.
html-zh:
	@rm -rf $(BUILDDIR)/html/zh_CN
	@python -c "import breathe" 2>/dev/null || { echo "Error: missing 'breathe'. Run: pip install -r docs/requirements.txt"; exit 1; }
	@echo "Running Doxygen (from repo root)..."
	cd .. && doxygen docs/$(DOXYFILE)
	cp index_zh.rst index.rst && \
	( READTHEDOCS_LANGUAGE=zh_CN $(SPHINXBUILD) -b html -D language=zh_CN $(SPHINXOPTS) $(SOURCEDIR) $(BUILDDIR)/html/zh_CN; r=$$?; git checkout -- index.rst; exit $$r )
	@echo "Open $(BUILDDIR)/html/zh_CN/index.html"

# Build both English and Chinese (for local preview of both languages)
html-all: html html-zh

clean:
	rm -rf $(BUILDDIR) doxygen

.PHONY: html html-zh html-all clean
