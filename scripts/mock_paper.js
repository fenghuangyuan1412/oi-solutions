// 把自编模拟赛的 problem.txt 渲染成「像真卷一样」的题面：paper.md + paper.html。
//
//   node scripts/mock_paper.js notes/mock-cspj-round2/set1 --set "第 1 套"
//   node scripts/mock_paper.js notes/mock-cspj-round2 --all --set-prefix "第 " --set-suffix " 套"
//
// 约定（与 AGENTS.md §5.5 一致）：题目文字**只**写在 problem.txt 里，paper.* 一律由本脚本生成，
// 不要手改生成物。要改题面就改 problem.txt 再重跑。
//
// problem.txt 的语法（尽量贴近 CCF 复赛卷的排版）：
//   # 卷名行            → 整卷标题（每个 set 文件开头一次）
//   % 键: 值             → 整卷信息行（时长、总分、文件名约定等），渲染在标题下方
//   == T1 题名 / name == → 一题的开始，`题名` 与 `英文名` 之间用 ` / ` 分隔
//   【小标题】           → 该题内的黑体小标题，独占一行
//   ```                 → 代码/样例输入输出的等宽块
//   - 行                → 无序列表项（连续行合成一组）
//   | a | b | 行         → 表格行（连续行合成一张表，首行是表头，| --- | 分隔行自动吃掉）
//                          单元格里的反引号可以名正言顺地包含竖线，如 `|s| ≤ 10`
//   其它非空行          → 正文段落；段内 `xx` 渲染成等宽，**xx** 渲染成粗体
//   // 注释行           → 只给作者看，不输出

const fs = require('fs');
const path = require('path');

function arg(name, dflt) {
  const i = process.argv.indexOf('--' + name);
  return i > 0 && process.argv[i + 1] && !process.argv[i + 1].startsWith('--') ? process.argv[i + 1] : dflt;
}
const has = (name) => process.argv.indexOf('--' + name) > 0;

// ---------- 解析 ----------
// 表格里单元格的竖线必须写成 `\|`（markdown 的规矩），反引号里的竖线也当作内容而非分隔符。
const VBAR = '\u0000';
function splitRow(s) {
  const protectedStr = s.slice(1, -1).replace(/\\\|/g, VBAR);
  const cells = [];
  let buf = '', inCode = false;
  for (const ch of protectedStr) {
    if (ch === '`') inCode = !inCode;
    if (ch === '|' && !inCode) { cells.push(buf.trim()); buf = ''; continue; }
    buf += ch;
  }
  cells.push(buf.trim());
  return cells;
}

function parse(file) {
  const lines = fs.readFileSync(file, 'utf8').replace(/^/, '').split(/\r?\n/);
  const paper = { title: '', info: [], problems: [] };
  let cur = null, mode = 'p';

  const closeBlock = () => { if (mode === 'code') { cur.body[cur.body.length - 1].closed = true; mode = 'p'; } };

  // 上一行是不是表格行？用它来决定「新的一行是接进上一张表还是另起一张表」，
  // 这样中间空一行、或插了小标题的两张表不会被错误合并。
  let prevTable = null;

  for (const raw0 of lines) {
    const raw = raw0.replace(/\s+$/, '');
    if (/^\/\//.test(raw.trim())) continue;

    const tr = /^\|.+\|$/.test(raw.trim()) ? splitRow(raw.trim()) : null;
    if (!tr) prevTable = null;
    if (tr && cur) {
      if (tr.every((c) => /^:?-{2,}:?$/.test(c))) continue; // | --- | --- | 分隔行
      if (prevTable) prevTable.rows.push(tr);
      else { prevTable = { k: 'table', rows: [tr] }; cur.body.push(prevTable); }
      continue;
    }

    const t = /^==\s*(T\d+)\s+(.+?)\s*==$/.exec(raw);
    if (t) {
      const seg = /(.+?)\s*\/\s*([A-Za-z0-9_\-]+)\s*$/.exec(t[2]);
      cur = { no: t[1], title: seg ? seg[1].trim() : t[2], en: seg ? seg[2] : '', body: [] };
      paper.problems.push(cur);
      mode = 'p';
      continue;
    }
    if (!cur) {
      if (/^#\s+/.test(raw)) { paper.title = raw.replace(/^#\s+/, ''); continue; }
      const inf = /^%\s*([^:：]+?)\s*[:：]\s*(.+)$/.exec(raw);
      if (inf) { paper.info.push([inf[1].trim(), inf[2].trim()]); continue; }
      if (raw.trim() === '') continue;
      paper.info.push(['', raw.trim()]);
      continue;
    }

    if (raw.trim() === '```') {
      if (mode === 'code') { mode = 'p'; cur.body[cur.body.length - 1].closed = true; }
      else { cur.body.push({ k: 'code', lines: [] }); mode = 'code'; }
      continue;
    }
    if (mode === 'code') { cur.body[cur.body.length - 1].lines.push(raw0); continue; }

    if (/^【.+】$/.test(raw.trim())) { cur.body.push({ k: 'h', text: raw.trim() }); continue; }
    if (/^-\s+/.test(raw.trim())) {
      const last = cur.body[cur.body.length - 1];
      if (last && last.k === 'ul') last.items.push(raw.trim().replace(/^-\s+/, ''));
      else cur.body.push({ k: 'ul', items: [raw.trim().replace(/^-\s+/, '')] });
      continue;
    }
    if (raw.trim() === '') continue;
    cur.body.push({ k: 'p', text: raw.trim() });
  }
  closeBlock();
  return paper;
}

// ---------- 行内排版 ----------
function esc(s) {
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}
// paper.md 用原生 markdown；paper.html 用 HTML。两者分开转，别把 <code> 混进 md。
// problem.txt 本身就用 `code` / **粗体** 写，paper.md 原样保留即合法 markdown。
function inMd(s) {
  return s;
}
function inline(s) {
  let t = esc(s);
  t = t.replace(/\*\*([^*]+)\*\*/g, '<strong>$1</strong>');
  t = t.replace(/`([^`]+)`/g, '<code>$1</code>');
  return t;
}
// 单元格里被保护起来的竖线，在两种输出各自还原：md 继续用 `\|`，HTML 直接给竖线。
const mdCell = (s) => s.split(VBAR).join('\\|');
const htmlCell = (s) => inline(s.split(VBAR).join('|'));

// ---------- markdown ----------
function toMd(paper, meta) {
  const o = [];
  o.push('# ' + paper.title, '');
  for (const [k, v] of paper.info) o.push(k ? `- **${k}**：${v}` : `> ${v}`);
  o.push('');
  for (const p of paper.problems) {
    o.push('---', '', `## ${p.no}　${p.title}`, '');
    if (p.en) o.push(`*(源文件名 / 程序名：\`${p.en}\`.cpp)*`, '');
    for (const b of p.body) {
      if (b.k === 'h') o.push('**' + b.text + '**', '');
      else if (b.k === 'p') o.push(inMd(b.text), '');
      else if (b.k === 'ul') { b.items.forEach((i) => o.push('- ' + inMd(i))); o.push(''); }
      else if (b.k === 'code') { o.push('```', ...b.lines, '```', ''); }
      else if (b.k === 'table') {
        b.rows.forEach((r, i) => {
          o.push('| ' + r.map(mdCell).join(' | ') + ' |');
          if (i === 0) o.push('| ' + r.map(() => '---').join(' | ') + ' |');
        });
        o.push('');
      }
    }
  }
  o.push('---', '', meta.footer.join('\n'), '');
  return o.join('\n');
}

// ---------- HTML（仿 CCF 复赛卷排版，方便逐题截图）----------
const CSS = `
:root{--ink:#16181d;--sub:#4b5563;--line:#d7dbe0;--paper:#fff;--bg:#e9edf2;--accent:#8a1c1c;}
*{box-sizing:border-box}
body{margin:0;padding:28px 16px 60px;background:var(--bg);color:var(--ink);
 font-family:"Times New Roman","Songti SC","SimSun","Noto Serif CJK SC",serif;
 font-size:16px;line-height:1.9;-webkit-font-smoothing:antialiased}
.sheet{max-width:860px;margin:0 auto 26px;background:var(--paper);padding:34px 44px 40px;
 border:1px solid var(--line);border-radius:3px;box-shadow:0 2px 10px rgba(0,0,0,.08)}
.masthead{text-align:center;border-bottom:3px double var(--ink);padding-bottom:14px;margin-bottom:16px}
.masthead h1{font-size:23px;margin:0 0 6px;letter-spacing:2px}
.masthead .sub{font-size:14px;color:var(--sub);letter-spacing:1px}
.warn{margin:0 0 16px;padding:9px 13px;border-left:4px solid var(--accent);background:#fdf3f3;
 font-size:13.5px;line-height:1.7;color:#6b2020}
.meta{display:grid;grid-template-columns:120px 1fr;gap:2px 12px;font-size:14.5px;margin:0 0 14px}
.meta dt{color:var(--sub)}
.meta dd{margin:0}
h2.pt{font-size:20px;text-align:center;margin:8px 0 4px;letter-spacing:1px}
p.pname{text-align:center;font-size:13.5px;color:var(--sub);margin:0 0 14px}
.scoreline{display:flex;justify-content:space-between;border-top:1px solid var(--line);
 border-bottom:1px solid var(--line);padding:5px 2px;margin:0 0 16px;font-size:13.5px;color:var(--sub)}
h3.sec{font-size:16.5px;margin:20px 0 6px;font-weight:700}
p.para{margin:0 0 11px;text-align:justify}
ul{margin:0 0 12px;padding-left:26px}
li{margin:0 0 3px}
pre{margin:0 0 13px;padding:10px 13px;background:#f6f7f9;border:1px solid var(--line);border-radius:3px;
 font-family:Consolas,"Courier New",monospace;font-size:13.5px;line-height:1.65;white-space:pre-wrap;overflow-x:auto}
code{font-family:Consolas,"Courier New",monospace;font-size:14px;background:#f2f3f5;padding:0 3px;border-radius:2px}
strong{font-weight:700}
.tblwrap{overflow-x:auto;margin:0 0 14px}
table{border-collapse:collapse;font-size:14px;width:100%}
th,td{border:1px solid var(--ink);padding:4px 9px;text-align:center}
th{background:#f1f2f4;font-weight:700}
.hintbox{margin:0 0 13px;padding:10px 13px;background:#f6f7f9;border:1px dashed var(--line);
 font-size:14.5px;line-height:1.85;white-space:pre-wrap;font-family:Consolas,monospace}
.pgno{text-align:center;font-size:12.5px;color:#9aa1ab;margin-top:20px}
@media print{body{background:#fff;padding:0}.sheet{border:0;box-shadow:none;page-break-after:always}}
`;

function renderBlocks(p) {
  const o = [];
  for (const b of p.body) {
    if (b.k === 'h') o.push('<h3 class="sec">' + esc(b.text) + '</h3>');
    else if (b.k === 'p') o.push('<p class="para">' + inline(b.text) + '</p>');
    else if (b.k === 'ul') o.push('<ul>' + b.items.map((i) => '<li>' + inline(i) + '</li>').join('') + '</ul>');
    else if (b.k === 'code') {
      const txt = b.lines.join('\n').replace(/^\n+|\n+$/g, '');
      o.push('<pre>' + esc(txt) + '</pre>');
    } else if (b.k === 'table') {
      const head = '<tr>' + b.rows[0].map((c) => '<th>' + htmlCell(c) + '</th>').join('') + '</tr>';
      const body = b.rows.slice(1)
        .map((r) => '<tr>' + r.map((c) => '<td>' + htmlCell(c) + '</td>').join('') + '</tr>')
        .join('\n');
      o.push(`<div class="tblwrap"><table><thead>${head}</thead><tbody>${body}</tbody></table></div>`);
    }
  }
  return o.join('\n');
}

function toHtml(paper, meta) {
  const infoDl = paper.info
    .map(([k, v]) => (k ? `<dt>${esc(k)}</dt><dd>${inline(v)}</dd>` : `<dt></dt><dd>${inline(v)}</dd>`))
    .join('\n');
  const sheets = [];
  sheets.push(`<div class="sheet">
<div class="masthead"><h1>${esc(paper.title)}</h1><div class="sub">${esc(meta.subtitle)}</div></div>
<div class="warn">${meta.warning}</div>
<dl class="meta">${infoDl}</dl>
<div class="pgno">${esc(meta.setName)} · 卷首</div>
</div>`);
  for (const p of paper.problems) {
    sheets.push(`<div class="sheet">
<h2 class="pt">${esc(p.no)}　${esc(p.title)}</h2>
${p.en ? `<p class="pname">程序文件名：<code>${esc(p.en)}.cpp</code>　输入文件名：<code>${esc(p.en)}.in</code>　输出文件名：<code>${esc(p.en)}.out</code></p>` : ''}
<div class="scoreline"><span>满分 100 分</span><span>时间限制 1000ms　空间限制 512MB</span></div>
${renderBlocks(p)}
<div class="pgno">${esc(meta.setName)} · ${esc(p.no)} ${esc(p.title)}</div>
</div>`);
  }
  return `<!DOCTYPE html>
<html lang="zh-CN"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>${esc(paper.title)}</title>
<style>${CSS}</style></head>
<body>${sheets.join('\n')}</body></html>
`;
}

// 表格列数不齐多半是单元格里写了裸竖线没转义，直接报警而不是画出残缺的表。
function checkTables(paper) {
  for (const p of paper.problems) {
    for (const b of p.body) {
      if (b.k !== 'table') continue;
      if (b.rows.length < 2) console.warn(`⚠ ${p.no}「${p.title}」的表格只有表头没有数据行`);
      const w = b.rows[0].length;
      b.rows.forEach((r, i) => {
        if (r.length !== w) {
          console.warn(`⚠ ${p.no}「${p.title}」表格第 ${i + 1} 行有 ${r.length} 列，表头是 ${w} 列：` +
            `单元格里单独的竖线请写成 \\|  →  | ${r.join(' | ')} |`);
        }
      });
    }
  }
}

// ---------- 驱动 ----------
function build(dir, opts) {
  const src = path.join(dir, 'problem.txt');
  if (!fs.existsSync(src)) { console.error('跳过（没有 problem.txt）：' + dir); return false; }
  const paper = parse(src);
  checkTables(paper);
  if (!paper.problems.length) { console.error('警告：' + dir + ' 没解析到任何 == T1 == 题块'); }
  const setName = opts.setName || path.basename(dir);
  const meta = {
    subtitle: opts.subtitle || 'CCF CSP-J 第二轮（复赛）模拟卷 · 仿真题面',
    warning: opts.warning ||
      '<strong>自编模拟卷，不是任何一年的 CCF 真题。</strong>题目背景为练习而设，' +
      '知识点与数据规模对齐 CSP-J 2023—2025 第二轮。',
    setName,
    footer: [
      '*生成物：本文件由 `scripts/mock_paper.js` 从 `problem.txt` 生成，请勿手改；要改题面改 `problem.txt` 后重跑。*',
      '',
      '**参考答案与评分**：见 `answer-key.md`（题面里没有，截图不会穿帮）。',
    ],
  };
  fs.writeFileSync(path.join(dir, 'paper.md'), toMd(paper, meta), 'utf8');
  fs.writeFileSync(path.join(dir, 'paper.html'), toHtml(paper, meta), 'utf8');
  console.log(`OK  ${dir.replace(/\\/g, '/')}  →  paper.md + paper.html（${paper.problems.length} 题）`);
  return true;
}

const first = process.argv[2];
if (!first) {
  console.error('用法：node scripts/mock_paper.js <题目目录> [--set-name X] [--subtitle Y] [--warning Z]');
  process.exit(1);
}
const opts = {
  setName: arg('set-name', null),
  subtitle: arg('subtitle', null),
  warning: arg('warning', null),
};
if (has('all')) {
  const kids = fs.readdirSync(first).filter((n) => fs.existsSync(path.join(first, n, 'problem.txt')));
  let n = 0;
  for (const k of kids) { if (build(path.join(first, k), { ...opts, setName: null })) n++; }
  console.log(`共生成 ${n} 份卷子`);
} else build(first, opts);
