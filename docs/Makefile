# Sphinx + Breathe doc build (run from repo root: make -C docs html)
SPHINXOPTS  ?=
SPHINXBUILD ?= sphinx-build
SOURCEDIR   = .
BUILDDIR    = _build
DOXYFILE    = Doxyfile

# Run Doxygen to generate XML for Breathe, then Sphinx
# Requires: pip install -r docs/requirements.txt (sphinx, breathe, myst-parser, sphinx-rtd-theme)
html:
	@python -c "import breathe" 2>/dev/null || { echo "Error: missing 'breathe'. Run: pip install -r docs/requirements.txt"; exit 1; }
	@echo "Running Doxygen (from repo root)..."
	cd .. && doxygen docs/$(DOXYFILE)
	$(SPHINXBUILD) -b html $(SPHINXOPTS) $(SOURCEDIR) $(BUILDDIR)/html
	@echo "Open $(BUILDDIR)/html/index.html"

clean:
	rm -rf $(BUILDDIR) doxygen

.PHONY: html clean
