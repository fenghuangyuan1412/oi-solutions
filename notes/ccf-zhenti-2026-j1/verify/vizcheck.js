// 批量体检本套卷的可视化页面：加载 → 狂点所有按钮把状态机走穿 →
// 检查 NaN/undefined/Infinity/抛错/横向溢出 → 逐张全页截图（截图不入库，只给人看排版）。
//   node notes/ccf-zhenti-2026-j1/verify/vizcheck.js              # 查全部 c-*.html
//   node notes/ccf-zhenti-2026-j1/verify/vizcheck.js c-bigadd     # 只查名字含 c-bigadd 的
const { createRequire } = require('module');
const path = require('path');
const fs = require('fs');
const req = createRequire('E:/code/fanqiezhong/package.json');
const { chromium } = req('playwright');

const root = path.resolve(__dirname, '..');
const filter = process.argv[2];
const files = fs.readdirSync(root).filter((f) => /^c-.*\.html$/.test(f) && (!filter || f.includes(filter)));
const outDir = path.join(__dirname, 'viz');
fs.mkdirSync(outDir, { recursive: true });

(async () => {
  const browser = await chromium.launch({ channel: 'chrome', headless: true });
  const ctx = await browser.newContext({ viewport: { width: 1280, height: 1500 } });
  let bad = 0;
  for (const f of files) {
    const page = await ctx.newPage();
    const errs = [];
    page.on('pageerror', (e) => errs.push('pageerror: ' + e.message));
    page.on('console', (m) => { if (m.type() === 'error') errs.push('console: ' + m.text()); });
    await page.goto('file:///' + path.join(root, f).replace(/\\/g, '/'));
    await page.click('body');
    const text0 = await page.evaluate(() => document.body.innerText.length);
    for (let round = 0; round < 45; round++) {
      const btns = await page.$$('button');
      for (const b of btns) {
        try { await b.click({ timeout: 1200 }); } catch (e) { /* 禁用或消失，忽略 */ }
      }
    }
    // 键盘也走一遍，确认 ← → 空格绑定没写崩
    for (const key of ['ArrowRight', 'ArrowRight', ' ', 'ArrowLeft', 'ArrowRight']) {
      await page.keyboard.press(key === ' ' ? 'Space' : key);
    }
    const report = await page.evaluate(() => {
      const t = document.body.innerText;
      const hit = (s) => (t.match(new RegExp(s, 'g')) || []).length;
      const overflow = [...document.querySelectorAll('body *:not(svg):not(svg *)')]
        .filter((el) => !/^(INPUT|TEXTAREA|SELECT|PRE)$/.test(el.tagName))
        .filter((el) => el.scrollWidth > el.clientWidth + 4 && el.clientWidth > 0)
        .map((el) => (el.getAttribute && el.getAttribute('class')) || el.tagName);
      return {
        nan: hit('NaN'), undef: hit('undefined'), inf: hit('Infinity'),
        overflow: [...new Set(overflow)].slice(0, 6),
        len: t.length,
      };
    });
    await page.screenshot({ path: path.join(outDir, f.replace(/\.html$/, '.png')), fullPage: true });
    const ok = report.nan + report.undef + report.inf === 0 && errs.length === 0 && report.overflow.length === 0;
    if (!ok) bad++;
    console.log(`${ok ? 'OK  ' : '!!  '} ${f}  NaN=${report.nan} undef=${report.undef} inf=${report.inf}` +
      ` 文字${text0}→${report.len} 溢出[${report.overflow.join(',')}] 报错[${errs.slice(0, 3).join(' | ')}]`);
    await page.close();
  }
  await browser.close();
  console.log(bad ? `${bad} 个页面有问题` : `${files.length} 个页面全部通过`);
})();
