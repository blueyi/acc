# ACC documentation - Sphinx + Breathe (C++ API from Doxygen XML)
# i18n: XXX.md = English doc, XXX_zh.md = Chinese doc. Only one → both builds use it; both exist → en uses XXX.md, zh uses XXX_zh.md.

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
# i18n: scan .md — has_en = bases with XXX.md, has_zh = bases with XXX_zh.md.
# Only XXX.md → both builds use it. Only XXX_zh.md → both use it. Both → en=XXX.md, zh=XXX_zh.md.
# ---------------------------------------------------------------------------
_docs_dir = os.path.dirname(os.path.abspath(__file__))
_base_exclude_set = set(base_exclude)

def _scan_md():
    """Return (has_en, has_zh). has_en = set of base names with X.md; has_zh = set with X_zh.md."""
    has_en = set()
    has_zh = set()
    for f in os.listdir(_docs_dir):
        if not f.endswith(".md") or f in _base_exclude_set:
            continue
        if f.endswith("_zh.md"):
            has_zh.add(f[:-6])
        else:
            has_en.add(f[:-3])
    return has_en, has_zh

_has_en, _has_zh = _scan_md()
_both = _has_en & _has_zh
# Canonical order (add new docs here and in index.rst toctree)
_main_doc_order = ["GETTING_STARTED", "PROJECT_PLAN", "README"]

def _toctree_list(use_zh_prefer_zh):
    """use_zh_prefer_zh: True for Chinese build (prefer X_zh), False for English (prefer X)."""
    out = []
    for x in _main_doc_order:
        if use_zh_prefer_zh:
            name = (x + "_zh") if x in _has_zh else (x if x in _has_en else None)
        else:
            name = x if x in _has_en else (x + "_zh") if x in _has_zh else None
        if name:
            out.append(name)
    for x in sorted((_has_en | _has_zh) - set(_main_doc_order)):
        if use_zh_prefer_zh:
            name = (x + "_zh") if x in _has_zh else (x if x in _has_en else None)
        else:
            name = x if x in _has_en else (x + "_zh") if x in _has_zh else None
        if name:
            out.append(name)
    return out

# Detect Chinese build: READTHEDOCS_LANGUAGE=zh-cn, or on RtD when index.rst was overwritten by index_zh
_build_lang = (os.environ.get("READTHEDOCS_LANGUAGE") or "en").strip().lower().replace("_", "-")
_is_zh = _build_lang in ("zh-cn", "zh") or _build_lang.startswith("zh-")
if not _is_zh and os.environ.get("READTHEDOCS") == "True":
    _idx = os.path.join(_docs_dir, "index.rst")
    if os.path.isfile(_idx):
        try:
            with open(_idx, "r", encoding="utf-8") as _f:
                _head = _f.read(300)
            if "i18n-toctree" in _head or "ACC 中文" in _head or "用户与开发者" in _head:
                _is_zh = True
        except Exception:
            pass

if _is_zh:
    language = "zh_CN"
    master_doc = "index"
    # Exclude X.md only when both X.md and X_zh.md exist (then we use X_zh.md for zh)
    exclude_patterns = (
        base_exclude
        + ["index_zh.rst", "index_zh.md"]
        + [x + ".md" for x in _both]
    )
    i18n_main_toctree = _toctree_list(use_zh_prefer_zh=True)
else:
    language = "en"
    master_doc = "index"
    # Exclude X_zh.md only when both exist (then we use X.md for en)
    exclude_patterns = (
        base_exclude
        + ["index_zh.rst", "index_zh.md"]
        + [x + "_zh.md" for x in _both]
    )
    i18n_main_toctree = _toctree_list(use_zh_prefer_zh=False)

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
