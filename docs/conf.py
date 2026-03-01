# ACC documentation - Sphinx + Breathe (C++ API from Doxygen XML)
# i18n: English = default (.md); Chinese = *_zh.md. Two builds via READTHEDOCS_LANGUAGE.

import os

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
base_exclude = [
    "_build",
    "Thumbs.db",
    ".DS_Store",
    "DIRECTORY_STRUCTURE_en.md",
    "PROJECT_PLAN_en.md",
    "TODO_en.md",
    "capability-radar-roadmap.md",
]

# Dual-file i18n: English .md + Chinese _zh.md. RtD sets READTHEDOCS_LANGUAGE for zh project.
# RtD uses lowercase with hyphen (e.g. zh-cn); accept zh-cn, zh_CN, zh.
build_lang = (os.environ.get("READTHEDOCS_LANGUAGE") or "en").strip().lower().replace("_", "-")
if build_lang in ("zh-cn", "zh") or build_lang.startswith("zh-"):
    language = "zh_CN"
    # Use master_doc = "index" so Sphinx outputs index.html (required by Read the Docs). index.rst is filled by pre_build copy of index_zh.rst when READTHEDOCS_LANGUAGE=zh-cn.
    master_doc = "index"
    # Exclude English-only .md and index_zh.rst (content is in index.rst after pre_build copy)
    english_docs_with_zh = [
        "GETTING_STARTED.md",
        "PROJECT_PLAN.md",
        "README.md",
    ]
    exclude_patterns = base_exclude + ["index_zh.rst", "index_zh.md"] + english_docs_with_zh
else:
    language = "en"
    master_doc = "index"
    exclude_patterns = base_exclude + ["*_zh.md", "index_zh.rst", "index_zh.md"]

source_suffix = {
    ".rst": "restructuredtext",
    ".md": "markdown",
}

html_theme = "sphinx_rtd_theme"
html_static_path = ["_static"]
html_show_sourcelink = True
# Language switcher only for local preview; RtD provides its own flyout
html_js_files = [] if os.environ.get("READTHEDOCS") == "True" else ["language-switcher.js"]

# Breathe: C++ API from Doxygen XML (generate XML first: doxygen docs/Doxyfile)
breathe_projects = {"acc": "doxygen/xml"}
breathe_default_project = "acc"
breathe_default_members = ("members", "undoc-members", "protected-members")

# MyST
myst_enable_extensions = ["deflist", "tasklist"]
