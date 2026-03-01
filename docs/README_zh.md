# ACC 文档 / ACC Documentation

本目录为 ACC 项目的文档工程（Sphinx + Read the Docs），说明文档方案、构建方式与本地预览方法。  
This directory is the ACC documentation project (Sphinx + RtD). It describes the doc scheme, how to build, and how to preview locally.

---

## Read the Docs 部署地址 / RtD Deployment

文档在线地址（在 Read the Docs 导入本仓库并构建成功后可用）：

**https://acc-docs.readthedocs.io/en/latest/**（英文）  
**https://acc-docs.readthedocs.io/zh_CN/latest/**（中文）

（若 RtD 项目 slug 不同，以项目设置中的 URL 为准。导入与语言切换见下文。）

---

## 文档方案 / Documentation Scheme

本项目采用 **Sphinx + Read the Docs**：

| 组件 | 作用 |
|------|------|
| **Sphinx** | 文档站点：RST + Markdown（MyST），统一导航与主题（Read the Docs 主题） |
| **Doxygen** | 从 `include/ACC` 头文件中的注释生成 **XML** |
| **Breathe** | 在 Sphinx 中引用 Doxygen XML，渲染 C++ API 文档 |
| **Read the Docs** | 在线托管、多版本、搜索、可选 PDF |

接口文档生成链路：

```
include/ACC/*.h (/// Doxygen 注释) → Doxygen (XML) → Breathe → Sphinx (HTML/PDF)
```

用户文档（入门、TODO、目录结构、项目计划等）使用本目录下已有的 `*.md`，由 Sphinx 通过 MyST 解析并纳入侧栏。

---

## 构建方式 / How to Build

### 环境准备

- **Python 3**（建议 3.10+）
- **Doxygen**、**Graphviz**（用于 C++ API；可选用于关系图）

```bash
# macOS
brew install doxygen graphviz

# Ubuntu / Debian
sudo apt-get install doxygen graphviz

# Python 依赖（在仓库根目录）
pip install -r docs/requirements.txt
```

### 一键构建 HTML

在 **仓库根目录** 执行：

```bash
make -C docs html
```

或手动两步：

```bash
doxygen docs/Doxyfile
sphinx-build -b html docs docs/_build/html
```

构建产物在 **docs/_build/html/**。

---

## 本地预览静态文档 / Local Preview

构建完成后，在浏览器中打开生成的静态 HTML 即可本地预览：

```bash
# 方式一：直接打开 index（从仓库根目录）
open docs/_build/html/index.html
# 或
xdg-open docs/_build/html/index.html   # Linux

# 方式二：用本地 HTTP 服务（避免部分链接/资源问题）
cd docs/_build/html && python3 -m http.server 8080
# 浏览器访问 http://localhost:8080
```

推荐使用方式二，更接近 RtD 在线效果。若已执行 `make -C docs html-all`，则英文站在 http://localhost:8080/ ，中文站在 http://localhost:8080/zh_CN/ ；**侧栏顶部会显示「English | 中文」链接**，可在此切换语言。

---

## 在 Read the Docs 上部署 / Deploy on RtD

1. 打开 [Read the Docs](https://readthedocs.org/) 并登录，**Import a Project**，选择本仓库（如 GitHub 上的 `acc`）。
2. 仓库根目录已有 **.readthedocs.yaml**，RtD 会据此安装 doxygen/graphviz、在 pre_build 中运行 `doxygen docs/Doxyfile`、再用 Sphinx 构建。
3. 构建成功后，文档发布到 **https://acc-docs.readthedocs.io/en/latest/**（或你在 RtD 中配置的地址）。若点击「中文」出现 404，需在 RtD 新建**翻译项目**并关联（见「中英文与 RtD 语言切换」）；切换链接使用 `/zh_CN/latest/`，仅在该翻译项目构建后才存在。

---

## 中英文与 RtD 语言切换 / i18n and language switcher

项目支持**中英文双语文档**（无 gettext）：**默认 .md 为英文**，中文为 **`*_zh.md`** 成对文件；**API 文档仅英文**，中英文站共用同一 API 页。

- **英文**：父项目（如 `acc-docs`），使用 `index.rst` + 各 `.md`，URL 如 `/en/latest/`。
- **中文**：翻译项目（如 `acc-docs-zh`），同一仓库，Language 设为 Chinese (Simplified)；构建时使用 `index_zh.rst` + 各 `*_zh.md`，URL 如 `/zh_CN/latest/`。
- 在父项目 **Admin → Translations** 中关联中文项目后，站点飞出菜单可切换 English / 中文。

**本地命令：**

```bash
make -C docs html      # 仅英文
make -C docs html-zh   # 仅中文
make -C docs html-all  # 英文 + 中文
```

---

## 中英文文档约定 / i18n conventions

| 项目 | 约定 |
|------|------|
| **英文** | `docs/` 下所有 `.md` 默认为英文（如 `GETTING_STARTED.md`、`README.md`）。 |
| **中文** | 需翻译的文档对应 **`_zh.md`** 文件（如 `GETTING_STARTED_zh.md`、`README_zh.md`）。 |
| **API 文档** | 仅英文，由 Doxygen 生成；`api/index.rst` 中英文站共用。 |
| **Read the Docs** | 父项目（英文）+ 翻译项目（中文）同一仓库，在 Admin → Translations 关联后飞出菜单可切换语言。 |

**新增一篇可翻译文档时：**

1. 新增英文文件，如 `docs/NewDoc.md`，并加入 `index.rst` 的 toctree。
2. 新增中文文件 `docs/NewDoc_zh.md`，并加入 `index_zh.rst` 的 toctree。
3. 在 `conf.py` 的 `english_docs_with_zh` 列表中追加 `"NewDoc.md"`。

---

## 目录结构速览 / Doc Layout

| 路径 | 说明 |
|------|------|
| **conf.py** | Sphinx 配置、Breathe；按 READTHEDOCS_LANGUAGE 选择 index / index_zh 与 exclude |
| **index.rst** / **index_zh.rst** | 英文 / 中文首页与 toctree |
| **\*.md** / **\*_zh.md** | 英文文档 / 中文文档（成对） |
| **api/index.rst** | C++ API（仅英文，中英文站共用） |
| **Doxyfile** | Doxygen 配置，输出到 `docs/doxygen/` |
| **requirements.txt** | sphinx, sphinx-rtd-theme, breathe, myst-parser |
| **Makefile** | `html` / `html-zh` / `html-all` / `clean` |
