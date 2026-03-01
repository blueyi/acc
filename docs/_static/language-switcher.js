/**
 * Local language switcher: add "English | 中文" links when serving _build/html
 * (e.g. python -m http.server). RtD provides its own switcher on readthedocs.io.
 */
function injectLanguageSwitcher() {
  var enHref = "/index.html";
  var zhHref = "/zh_CN/index_zh.html";  /* Chinese root is index_zh.html (master_doc=index_zh) */
  var html =
    '<div class="language-switcher-local" style="padding:8px 12px;margin:0;font-size:85%;border-bottom:1px solid #404040;background:#2c2c2c;">' +
    '<a href="' + enHref + '" style="color:#b3b3b3;">English</a> | ' +
    '<a href="' + zhHref + '" style="color:#b3b3b3;">中文</a>' +
    "</div>";
  var div = document.createElement("div");
  div.innerHTML = html;
  var el = div.firstElementChild;
  var sidebar = document.querySelector(".wy-nav-side");
  var scroll = sidebar ? sidebar.querySelector(".wy-side-scroll") : null;
  if (scroll && scroll.firstChild) {
    scroll.insertBefore(el, scroll.firstChild);
  } else if (sidebar && sidebar.firstChild) {
    sidebar.insertBefore(el, sidebar.firstChild);
  } else {
    var nav = document.querySelector(".wy-nav-content-wrap");
    if (nav) {
      var wrap = document.createElement("div");
      wrap.style.cssText = "padding:6px 12px;background:#343131;color:#b3b3b3;font-size:14px;";
      wrap.innerHTML = 'Languages: <a href="' + enHref + '" style="color:#fff;">English</a> | <a href="' + zhHref + '" style="color:#fff;">中文</a>';
      nav.parentNode.insertBefore(wrap, nav);
    }
  }
}
if (document.readyState === "loading") {
  document.addEventListener("DOMContentLoaded", injectLanguageSwitcher);
} else {
  injectLanguageSwitcher();
}
