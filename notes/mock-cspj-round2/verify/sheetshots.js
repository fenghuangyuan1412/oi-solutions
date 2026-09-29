// 逐张截取 paper.html 里的 .sheet（每张题一页），用于人工核对排版：
//   node notes/mock-cspj-round2/verify/sheetshots.js set1 set3
// 输出到 verify/sheets/<set>-<n>.png。用本机已装好的 Chrome，不下载浏览器。
const { createRequire } = require('module');
const path = require('path');
const fs = require('fs');
const req = createRequire('E:/code/fanqiezhong/package.json');
const { chromium } = req('playwright');

const sets = process.argv.slice(2);
const root = path.resolve(__dirname, '..');
const outDir = path.join(__dirname, 'sheets');
fs.mkdirSync(outDir, { recursive: true });

(async () => {
  const browser = await chromium.launch({ channel: 'chrome', headless: true });
  const page = await browser.newPage({ viewport: { width: 1000, height: 1200 } });
  for (const s of sets) {
    const file = path.join(root, s, 'paper.html');
    await page.goto('file:///' + file.replace(/\\/g, '/'));
    const sheets = await page.$$('.sheet');
    const bad = await page.evaluate(() => {
      const t = document.body.innerText;
      const hits = [];
      if (/\|\s*---/.test(t)) hits.push('残留 markdown 表格分隔行');
      if (/\|s\||\|\s*n\s*\|/.test(t)) hits.push('裸竖线');
      for (const el of document.querySelectorAll('.sheet')) {
        if (el.scrollWidth > el.clientWidth + 2) hits.push('溢出: ' + el.querySelector('h2')?.textContent);
      }
      return hits;
    });
    for (let i = 0; i < sheets.length; i++) {
      const label = await sheets[i].evaluate((el) => (el.querySelector('h2') || el.querySelector('.masthead h1')).textContent.trim());
      const safe = label.replace(/[^\w\u4e00-\u9fff.-]+/g, '').slice(0, 24);
      await sheets[i].screenshot({ path: path.join(outDir, `${s}-${i}-${safe}.png`) });
      console.log(`${s} [${i}] ${label}`);
    }
    if (bad.length) console.log('  !! ' + bad.join(' / '));
  }
  await browser.close();
})();
