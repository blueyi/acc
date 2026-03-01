# ACC 文档 / ACC Documentation

本目录为 ACC 项目的文档工程（Sphinx + Read the Docs），说明文档方案、构建方式与本地预览方法。  
This directory is the ACC documentation project (Sphinx + RtD). It describes the doc scheme, how to build, and how to preview locally.

---

## Read the Docs 部署地址 / RtD Deployment

文档在线地址（在 Read the Docs 导入本仓库并构建成功后可用）：

**https://acc.readthedocs.io/**

（若项目在 RtD 上的 slug 不同，请以 RtD 项目设置中显示的 URL 为准。导入方法见下文「在 RtD 上部署」。）

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

推荐使用方式二，更接近 RtD 在线效果。

---

## 在 Read the Docs 上部署 / Deploy on RtD

1. 打开 [Read the Docs](https://readthedocs.org/) 并登录，**Import a Project**，选择本仓库（如 GitHub 上的 `acc`）。
2. 仓库根目录已有 **.readthedocs.yaml**，RtD 会据此安装 doxygen/graphviz、在 pre_build 中运行 `doxygen docs/Doxyfile`、再用 Sphinx 构建。
3. 构建成功后，文档发布到 **https://acc.readthedocs.io/**（或你在 RtD 中配置的地址）。

---

## 目录结构速览 / Doc Layout

| 路径 | 说明 |
|------|------|
| **conf.py** | Sphinx 配置、Breathe 指向 `doxygen/xml` |
| **index.rst** | 首页与 toctree |
| **api/index.rst** | C++ API 页（`.. doxygenclass::` 等） |
| **Doxyfile** | Doxygen 配置，输出到 `docs/doxygen/` |
| **requirements.txt** | sphinx, sphinx-rtd-theme, breathe, myst-parser |
| **Makefile** | `make -C docs html` 入口 |

更多细节见 [DOCUMENTATION_STRATEGY](DOCUMENTATION_STRATEGY)（注释规范、故障排查等）。
