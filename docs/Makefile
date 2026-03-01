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

# Build HTML in Chinese (uses *_zh.md + index_zh.rst; READTHEDOCS_LANGUAGE=zh_CN for conf.py)
# Cleans _build/html/zh_CN before building so output is always up to date.
html-zh:
	@rm -rf $(BUILDDIR)/html/zh_CN
	@python -c "import breathe" 2>/dev/null || { echo "Error: missing 'breathe'. Run: pip install -r docs/requirements.txt"; exit 1; }
	@echo "Running Doxygen (from repo root)..."
	cd .. && doxygen docs/$(DOXYFILE)
	READTHEDOCS_LANGUAGE=zh_CN $(SPHINXBUILD) -b html -D language=zh_CN $(SPHINXOPTS) $(SOURCEDIR) $(BUILDDIR)/html/zh_CN
	@echo '<!DOCTYPE html><html><head><meta charset="utf-8"><meta http-equiv="refresh" content="0;url=index_zh.html"><title>Redirect</title></head><body><p><a href="index_zh.html">ACC 文档（中文）</a></p></body></html>' > $(BUILDDIR)/html/zh_CN/index.html
	@echo "Open $(BUILDDIR)/html/zh_CN/index_zh.html or .../zh_CN/index.html"

# Build both English and Chinese (for local preview of both languages)
html-all: html html-zh

clean:
	rm -rf $(BUILDDIR) doxygen

.PHONY: html html-zh html-all clean
