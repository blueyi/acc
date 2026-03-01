# ACC documentation - Sphinx + Breathe (C++ API from Doxygen XML)
# i18n: English = default (.md); Chinese = *_zh.md when present, else fallback to .md. Two builds via READTHEDOCS_LANGUAGE.

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

# ---------------------------------------------------------------------------
# i18n: auto-scan for *_zh.md; Chinese build uses _zh when present else falls back to .md
# ---------------------------------------------------------------------------
_docs_dir = os.path.dirname(os.path.abspath(__file__))
_base_exclude_set = set(base_exclude)

def _scan_md_and_zh():
    """Return (docs_with_zh, main_doc_order). docs_with_zh = set of base names that have a _zh.md."""
    docs_with_zh = set()
    for f in os.listdir(_docs_dir):
        if not f.endswith(".md") or f in _base_exclude_set:
            continue
        if f.endswith("_zh.md"):
            base = f[:-6]  # strip _zh.md
            docs_with_zh.add(base)
    # Canonical order of main user docs (must match index.rst first toctree; add new docs here)
    main_doc_order = ["GETTING_STARTED", "PROJECT_PLAN", "README"]
    return docs_with_zh, main_doc_order

_docs_with_zh, _main_doc_order = _scan_md_and_zh()

# Chinese toctree: prefer X_zh if exists, else X (fallback to English .md)
i18n_main_toctree = [
    (x + "_zh") if x in _docs_with_zh else x for x in _main_doc_order
]
# Append any _zh-only doc not in main order (optional)
for x in sorted(_docs_with_zh):
    if x not in _main_doc_order:
        i18n_main_toctree.append(x + "_zh")

# Detect Chinese build: READTHEDOCS_LANGUAGE=zh-cn, or on RtD when pre_build already replaced index.rst with index_zh content
build_lang = (os.environ.get("READTHEDOCS_LANGUAGE") or "en").strip().lower().replace("_", "-")
_is_zh = build_lang in ("zh-cn", "zh") or build_lang.startswith("zh-")
if not _is_zh and os.environ.get("READTHEDOCS") == "True":
    _index_path = os.path.join(_docs_dir, "index.rst")
    if os.path.isfile(_index_path):
        try:
            with open(_index_path, "r", encoding="utf-8") as _f:
                _head = _f.read(300)
            if "i18n-toctree" in _head or "ACC 中文" in _head or "用户与开发者" in _head:
                _is_zh = True
        except Exception:
            pass
if _is_zh:
    language = "zh_CN"
    master_doc = "index"
    # Exclude only English .md that have a _zh counterpart; keep .md without _zh as fallback
    exclude_en_with_zh = [x + ".md" for x in _docs_with_zh]
    exclude_patterns = (
        base_exclude
        + ["index_zh.rst", "index_zh.md"]
        + exclude_en_with_zh
    )
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


# ---------------------------------------------------------------------------
# Custom directive: i18n-toctree (for index_zh.rst) uses i18n_main_toctree
# ---------------------------------------------------------------------------
def _setup_i18n_toctree(app):
    from docutils.parsers.rst import Directive
    from docutils.parsers.rst import directives
    from sphinx.addnodes import toctree as toctree_node

    class I18nToctreeDirective(Directive):
        has_content = False
        optional_arguments = 0
        required_arguments = 0
        option_spec = {
            "caption": directives.unchanged,
            "maxdepth": directives.unchanged,
        }

        def run(self):
            env = self.state.document.settings.env
            raw = getattr(env.config, "i18n_main_toctree", [])
            entries = [e for e in raw if e]
            caption = self.options.get("caption", "")
            maxdepth_str = self.options.get("maxdepth", "2")
            try:
                maxdepth = int(maxdepth_str)
            except ValueError:
                maxdepth = 2
            node = toctree_node()
            node["entries"] = [(e, "") for e in entries]
            node["includefiles"] = list(entries)
            node["caption"] = caption
            node["maxdepth"] = maxdepth
            node["hidden"] = False
            node["includehidden"] = False
            node["titlesonly"] = False
            node["glob"] = None
            node["numbered"] = 0
            return [node]

    app.add_directive("i18n-toctree", I18nToctreeDirective)


def setup(app):
    app.add_config_value("i18n_main_toctree", i18n_main_toctree, "env")
    _setup_i18n_toctree(app)
