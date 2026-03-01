# ACC Documentation

This directory is the ACC documentation project (Sphinx + Read the Docs). It describes the doc scheme, how to build, and how to preview locally.

---

## Read the Docs deployment

Documentation URLs (after importing this repo and building on Read the Docs):

**https://acc-docs.readthedocs.io/en/latest/** (English)  
**https://acc-docs.readthedocs.io/zh_CN/latest/** (Chinese)

If your RtD project slug differs, use the URL shown in the project settings. See below for import and language switching.

---

## Documentation scheme

This project uses **Sphinx + Read the Docs**:

| Component | Role |
|-----------|------|
| **Sphinx** | Doc site: RST + Markdown (MyST), unified nav and theme (Read the Docs theme) |
| **Doxygen** | Generates **XML** from comments in `include/ACC` headers |
| **Breathe** | Pulls Doxygen XML into Sphinx and renders C++ API docs |
| **Read the Docs** | Hosting, versions, search, optional PDF |

API doc pipeline:

```
include/ACC/*.h (/// Doxygen comments) → Doxygen (XML) → Breathe → Sphinx (HTML/PDF)
```

User docs (getting started, project plan, etc.) are the `*.md` files in this directory, parsed by Sphinx via MyST and included in the sidebar.

---

## How to build

### Prerequisites

- **Python 3** (3.10+ recommended)
- **Doxygen**, **Graphviz** (for C++ API; optional for diagrams)

```bash
# macOS
brew install doxygen graphviz

# Ubuntu / Debian
sudo apt-get install doxygen graphviz

# Python deps (from repo root)
pip install -r docs/requirements.txt
```

### Build HTML

From the **repository root**:

```bash
make -C docs html
```

Or manually:

```bash
doxygen docs/Doxyfile
sphinx-build -b html docs docs/_build/html
```

Output is under **docs/_build/html/**.

---

## Local preview

Open the generated HTML in a browser:

```bash
# Option 1: open index (from repo root)
open docs/_build/html/index.html
# or
xdg-open docs/_build/html/index.html   # Linux

# Option 2: local HTTP server (avoids some link/asset issues)
cd docs/_build/html && python3 -m http.server 8080
# Then open http://localhost:8080
```

Option 2 is recommended and closest to RtD. After `make -C docs html-all`, the English site is at http://localhost:8080/ and the Chinese site at http://localhost:8080/zh_CN/; **the sidebar shows an "English | 中文" link** to switch languages.

---

## Deploy on Read the Docs

1. Go to [Read the Docs](https://readthedocs.org/), sign in, **Import a Project**, and select this repository (e.g. `acc` on GitHub).
2. The repo root has **.readthedocs.yaml**; RtD will install doxygen/graphviz, run `doxygen docs/Doxyfile` in pre_build, then build with Sphinx.
3. After a successful build, docs are published at **https://acc-docs.readthedocs.io/en/latest/** (or your configured URL). **If the 中文 link returns 404**, you must add a **Translation** project for Chinese and link it (see "i18n and language switcher" below); the switcher uses `/zh_CN/latest/`, which only exists after that translation project is built.

---

## i18n and language switcher

The project supports **English and Chinese** via dual source files (no gettext): **default `.md` files are English**; Chinese uses **`*_zh.md`** pairs. **API docs are English only** and shared by both sites.

- **English**: Parent project (e.g. `acc-docs`), uses `index.rst` and English `.md` files; URL like `/en/latest/`.
- **Chinese**: Translation project (e.g. `acc-docs-zh`), same repo, Language set to Chinese (Simplified); build uses `index_zh.rst` and `*_zh.md`; URL like `/zh_CN/latest/`.
- Link the Chinese project under the parent’s **Admin → Translations** so the flyout menu offers English / 中文.

**Local commands:**

```bash
make -C docs html      # English only
make -C docs html-zh   # Chinese only
make -C docs html-all  # Both
```

---

## i18n conventions

| Item | Convention |
|------|------------|
| **English** | All `.md` files in `docs/` are English by default (e.g. `GETTING_STARTED.md`, `README.md`). |
| **Chinese** | Documents with a Chinese version have a **`*_zh.md`** file (e.g. `GETTING_STARTED_zh.md`, `README_zh.md`). |
| **API docs** | English only, from Doxygen; `api/index.rst` is shared by both sites. |
| **Read the Docs** | Parent (en) + translation (zh) project, same repo; link in Admin → Translations for the language switcher. |

**Adding a new translatable document:**

1. Add the English file, e.g. `docs/NewDoc.md`, and add it to the toctree in `index.rst`.
2. Add the Chinese file `docs/NewDoc_zh.md` and add it to the toctree in `index_zh.rst`.
3. Append `"NewDoc.md"` to the `english_docs_with_zh` list in `conf.py`.

---

## Doc layout

| Path | Description |
|------|-------------|
| **conf.py** | Sphinx config, Breathe; selects index / index_zh and exclude via READTHEDOCS_LANGUAGE |
| **index.rst** / **index_zh.rst** | English / Chinese home and toctree |
| **\*.md** / **\*_zh.md** | English docs / Chinese docs (pairs) |
| **api/index.rst** | C++ API (English only, shared) |
| **Doxyfile** | Doxygen config, output under `docs/doxygen/` |
| **requirements.txt** | sphinx, sphinx-rtd-theme, breathe, myst-parser |
| **Makefile** | `html` / `html-zh` / `html-all` / `clean` |
