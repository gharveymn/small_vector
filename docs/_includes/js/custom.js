function toggleTheme() {
  const sheet = document.styleSheets[0];
  const rule = "transition: all 0ms ease 0ms !important;";
  sheet.insertRule("*, *::before, *::after, *:hover { " + rule + " }", 0);
  if ("light" === document.documentElement.getAttribute("data-theme"))
    overrideTheme("dark");
  else
    overrideTheme("light");
  setTimeout(function() { sheet.deleteRule(0); }, 120);
}
