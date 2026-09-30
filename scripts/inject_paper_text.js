// 把卷面原文机械地嵌进讲评 README：
//   · 每套卷的每一篇程序（阅读/完善）开头贴出「卷面完整程序」，一行不漏；
//   · 每一道题的小节下面贴出「卷面原文」+ 四个选项。
//
//   node scripts/inject_paper_text.js notes/luogu-2026-j1 --blanks 33-37,38-42
//   node scripts/inject_paper_text.js notes/luogu-2026-s1 --blanks 34-38,39-43
//
// 为什么要脚本化：手抄题面一定会抄错。README 里凡是「卷面原文」引用块，
// 内容都来自 problem.txt，改题面只需改 problem.txt 再重跑本脚本。
// 本脚本可重复执行：已经贴过引用块的小节不会被再贴一遍。
// 注意：引用块是生成内容，请不要手工修改；讲评正文随便改，脚本只往小节标题后插东西。

const fs = require('fs');
const path = require('path');

const CIRCLED = ['①', '②', '③', '④', '⑤'];

function parseQuestions(file) {
  const raw = fs.readFileSync(file, 'utf8').split(/\r?\n/).map((l) => l.replace(/\u00a0/g, ' ').replace(/\s+$/g, ''));
  const qs = {};
  let cur = null, inOpts = false, inAd = false;
  for (const l of raw) {
    const t = l.trim();
    if (!t) continue;
    if (/^<<<PAGE/.test(t)) continue;
    // 卷尾广告不是试题：整段跳过，直到参考答案那一行
    if (/^广告\s/.test(t)) { inAd = true; cur = null; continue; }
    if (inAd) { if (/^参考答案/.test(t)) inAd = false; else continue; }
    if (/^[一二三]、|^（\d+）|^·\s*(判断题|单选题)|^参考答案/.test(t)) { cur = null; continue; }
    // 页眉页脚
    if (/^\d+\s*页|^题号 \d|^答案 |^第\s*\d+\s*页，共|^LUOGU SCP-[JS]|^\d{4} LUOGU|^（SCP-[JS]\d）|^认证时间/.test(t)) { cur = null; continue; }
    if (cur !== null && /^[A-D]\.\s/.test(t)) {
      inOpts = true;
      for (const p of t.split(/\s+(?=[A-D]\.\s)/)) qs[cur].options.push(p.trim());
      continue;
    }
    const q = /^(\d{1,2})\.\s*(.*)$/.exec(t);
    if (q && Number(q[1]) >= 1 && Number(q[1]) <= 43 && !/^\d{2}\s/.test(t)) {
      cur = Number(q[1]);
      qs[cur] = qs[cur] || { stem: [], options: [] };
      qs[cur].stem.push(q[2]);
      inOpts = false;
      continue;
    }
    if (cur === null || inOpts) continue;
    qs[cur].stem.push(t);
  }
  for (const k of Object.keys(qs)) qs[k].stemText = qs[k].stem.join('').replace(/\s+/g, ' ').trim();
  return qs;
}

function parsePrograms(paperFile) {
  const L = fs.readFileSync(paperFile, 'utf8').split('\n');
  const parts = {};
  let cur = null, inb = false, buf = [];
  for (const l of L) {
    const h = /^### (阅读程序|完善程序)（(\d)）$/.exec(l);
    if (h) { cur = (h[1] === '完善程序' ? 'fill' : 'read') + h[2]; continue; }
    if (/^```text$/.test(l)) { inb = true; buf = []; continue; }
    if (/^```$/.test(l)) { if (inb && cur) parts[cur] = (parts[cur] || []).concat(buf); inb = false; buf = []; continue; }
    if (inb) buf.push(l);
  }
  return parts;
}

// 兼容 `### 题 19：`、`（题 16）`、`题 22 / 23：`、`### ① / 题 34　` 四种写法
function questionNumsIn(line) {
  if (/^## 第 \(\d\) 篇/.test(line)) return [];
  const nums = [];
  const re = /题 (\d{1,2})(?:\s*\/\s*(?:题\s*)?(\d{1,2}))?/g;
  let m;
  while ((m = re.exec(line))) {
    const a = Number(m[1]);
    if (a >= 1 && a <= 43) nums.push(a);
    if (m[2]) { const b = Number(m[2]); if (b >= 1 && b <= 43) nums.push(b); }
  }
  return [...new Set(nums)];
}

function quote(stem, options) {
  const out = ['> **卷面原文**　' + stem];
  if (options.length) { out.push('>'); for (const o of options) out.push('> `' + o + '`'); }
  return out.join('\n');
}

function inject(readmeFile, qs, parts, blanks) {
  const L = fs.readFileSync(readmeFile, 'utf8').split('\n');
  const out = [];
  const done = new Set();
  let fillPart = null, bigSection = '', added = 0;
  const missing = new Set();

  const already = (i) => {
    for (let j = i + 1; j < Math.min(i + 4, L.length); j++)
      if (/^> \*\*(卷面原文|卷面完整程序)/.test(L[j])) return true;
    return false;
  };

  const pushQ = (n) => {
    if (done.has(n)) return;
    const q = qs[n];
    if (!q) { missing.add(n); return; }
    done.add(n);
    out.push('', quote(q.stemText, q.options), '');
    added++;
  };

  for (let i = 0; i < L.length; i++) {
    const l = L[i];
    out.push(l);

    const bs = /^## ([一二三])、/.exec(l);
    if (bs) { bigSection = bs[1]; fillPart = null; continue; }

    // 每篇开头：把这一篇的完整程序原样贴出来
    const pp = /^## 第 \((\d)\) 篇/.exec(l);
    if (pp) {
      const key = (bigSection === '三' ? 'fill' : 'read') + pp[1];
      if (bigSection === '三') fillPart = key;
      if (parts[key] && !already(i)) {
        const label = bigSection === '三'
          ? '> **卷面完整程序**（一行不漏；行号就是卷面行号，`①②③④⑤` 是 5 个待填的空）：'
          : '> **卷面完整程序**（一行不漏；代码块里的行号就是题目里说的"第几行"）：';
        out.push('', label, '', '```text', ...parts[key], '```', '',
          bigSection === '三' ? '下面把 5 个空逐个拆开讲。' : '下面把最关键的几行单独拎出来讲。');
        added++;
      }
      continue;
    }

    // 完善程序「逐空推导」里的小标题：`**① = A `a[i] <= x`**`
    const bm = /^\*\*([①②③④⑤]) = /.exec(l);
    if (bm && fillPart) {
      const idx = CIRCLED.indexOf(bm[1]);
      const n = idx >= 0 ? blanks[fillPart][idx] : 0;
      if (n && !already(i)) pushQ(n);
      continue;
    }

    if (!/^#{2,4} /.test(l)) continue;
    const nums = questionNumsIn(l);
    if (nums.length && !already(i)) { out.push(''); nums.forEach(pushQ); out.push(''); }
  }
  fs.writeFileSync(readmeFile, out.join('\n').replace(/\n{3,}/g, '\n\n'), 'utf8');
  return { added, done: [...done].sort((a, b) => a - b), missing: [...missing].sort((a, b) => a - b) };
}

const dir = process.argv[2];
const spec = (process.argv.find((a) => a.startsWith('--blanks=')) || process.argv[process.argv.indexOf('--blanks') + 1] || '');
const ranges = spec.split(',').map((r) => r.split('-').map(Number));
if (!dir || ranges.length !== 2 || ranges.some((r) => r.length !== 2 || !r[0])) {
  console.error('用法：node scripts/inject_paper_text.js <notes/某套卷> --blanks 33-37,38-42');
  process.exit(1);
}
const base = dir.replace(/\/$/, '');
const qs = parseQuestions(path.join(base, 'problem.txt'));
const parts = parsePrograms(path.join(base, 'paper.md'));
const blanks = { fill1: [], fill2: [] };
for (let i = ranges[0][0]; i <= ranges[0][1]; i++) blanks.fill1.push(i);
for (let i = ranges[1][0]; i <= ranges[1][1]; i++) blanks.fill2.push(i);
console.log(base + '  卷面题数=' + Object.keys(qs).length +
  '  程序=' + Object.keys(parts).map((k) => k + '(' + parts[k].length + '行)').join(' '));
const r = inject(path.join(base, 'README.md'), qs, parts, blanks);
console.log('  注入 ' + r.added + ' 处  卷面缺题=' + (r.missing.length ? r.missing.join(',') : '无'));
console.log('  已贴卷面原文的题号: ' + (r.done.join(' ') || '（全都已经贴过了）'));
