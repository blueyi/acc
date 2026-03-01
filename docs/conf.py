# ACC documentation - Sphinx + Breathe (C++ API from Doxygen XML)
# Build: from repo root, run doxygen docs/Doxyfile then sphinx-build -b html docs docs/_build/html

project = "ACC"
copyright = "2026, ACC Project"
author = "ACC Project"
release = "0.1.0"
version = "0.1"

extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.viewcode",
    "sphinx.ext.intersphinx",
    "sphinx.ext.todo",
    "breathe",
    "myst_parser",
]

templates_path = ["_templates"]
exclude_patterns = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
    "DIRECTORY_STRUCTURE_en.md",
    "PROJECT_PLAN_en.md",
    "TODO_en.md",
    "capability-radar-roadmap.md",
]
source_suffix = {
    ".rst": "restructuredtext",
    ".md": "markdown",
}

html_theme = "sphinx_rtd_theme"
html_static_path = ["_static"]
html_show_sourcelink = True

# Breathe: C++ API from Doxygen XML (generate XML first: doxygen docs/Doxyfile)
breathe_projects = {"acc": "doxygen/xml"}
breathe_default_project = "acc"
breathe_default_members = ("members", "undoc-members", "protected-members")

# MyST
myst_enable_extensions = ["deflist", "tasklist"]
