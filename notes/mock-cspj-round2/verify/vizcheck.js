// 批量体检可视化页面：加载 → 狂点按钮 → 检查 NaN/undefined/抛错 → 逐张截图。
//   node notes/mock-cspj-round2/verify/vizcheck.js set1 set2
// 截图落在 verify/viz/，只用于人工核对排版，看完可删（不入库）。
const { createRequire } = require('module');
const path = require('path');
const fs = require('fs');
const req = createRequire('E:/code/fanqiezhong/package.json');
const { chromium } = req('playwright');

const sets = process.argv.slice(2).filter((a) => /^set\d$/.test(a));
const root = path.resolve(__dirname, '..');
const outDir = path.join(__dirname, 'viz');
fs.mkdirSync(outDir, { recursive: true });

(async () => {
  const browser = await chromium.launch({ channel: 'chrome', headless: true });
  const ctx = await browser.newContext({ viewport: { width: 1280, height: 1400 } });
  let bad = 0;
  for (const s of sets) {
    for (const slug of fs.readdirSync(path.join(root, s))) {
      const html = path.join(root, s, slug, 'visualization.html');
      if (!fs.existsSync(html)) continue;
      const page = await ctx.newPage();
      const errs = [];
      page.on('pageerror', (e) => errs.push('pageerror: ' + e.message));
      page.on('console', (m) => { if (m.type() === 'error') errs.push('console: ' + m.text()); });
      await page.goto('file:///' + html.replace(/\\/g, '/'));
      await page.click('body');
      const steps0 = await page.evaluate(() => document.body.innerText.length);
      // 交替点「下一步 / 自动播放 / 载入」等所有按钮，把状态机走穿
      for (let round = 0; round < 60; round++) {
        const btns = await page.$$('button');
        for (const b of btns) {
          try { await b.click({ timeout: 1500 }); } catch (e) { /* 禁用或消失，忽略 */ }
        }
      }
      const report = await page.evaluate(() => {
        const t = document.body.innerText;
        const hit = (re) => (t.match(new RegExp(re, 'g')) || []).length;
        const overflow = [...document.querySelectorAll('body *:not(svg):not(svg *)')]
          .filter((el) => !/^(INPUT|TEXTAREA|SELECT|PRE)$/.test(el.tagName))
          .filter((el) => el.scrollWidth > el.clientWidth + 4 && el.clientWidth > 0)
          .map((el) => (el.getAttribute && el.getAttribute('class')) || el.tagName);
        return {
          nan: hit('NaN'), undef: hit('undefined'), inf: hit('Infinity'),
          overflow: [...new Set(overflow)].slice(0, 5),
          len: t.length,
        };
      });
      const shot = path.join(outDir, `${s}-${slug}.png`);
      await page.screenshot({ path: shot, fullPage: true });
      const ok = report.nan + report.undef + report.inf === 0 && errs.length === 0 && report.overflow.length === 0;
      if (!ok) bad++;
      console.log(`${ok ? 'OK  ' : '!!  '} ${s}/${slug}  NaN=${report.nan} undef=${report.undef} inf=${report.inf}` +
        ` 文字${steps0}→${report.len} 溢出[${report.overflow.join(',')}] 报错[${errs.slice(0, 2).join(' | ')}]`);
      await page.close();
    }
  }
  await browser.close();
  console.log(bad ? `${bad} 个页面有问题` : '全部通过');
})();
