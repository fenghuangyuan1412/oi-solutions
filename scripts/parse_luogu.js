// 用法: node scripts/parse_luogu.js <洛谷题目 SSR html 路径>
// 从洛谷题目页的 SSR JSON 里取出题面字段。
const fs = require('fs');

const h = fs.readFileSync(process.argv[2], 'utf8');
const k = h.indexOf('"problem":');
if (k < 0) {
  console.error('未找到 problem 字段：页面可能被跳转或需要登录');
  process.exit(1);
}

let i = k + '"problem":'.length;
while (h[i] !== '{') i++;
// 扫描时要跳过字符串字面量，否则题面里的 {} 会打乱括号配对
let depth = 0, end = -1, instr = false, esc = false;
for (let j = i; j < h.length; j++) {
  const c = h[j];
  if (instr) {
    if (esc) esc = false;
    else if (c === '\\') esc = true;
    else if (c === '"') instr = false;
    continue;
  }
  if (c === '"') instr = true;
  else if (c === '{') depth++;
  else if (c === '}') { depth--; if (!depth) { end = j + 1; break; } }
}
if (end < 0) {
  console.error('题面 JSON 括号不配对，可能被截断');
  process.exit(1);
}

let p;
try {
  p = JSON.parse(h.slice(i, end));
} catch (e) {
  console.error('JSON 解析失败，输出原始片段：' + e.message);
  console.log(h.slice(i, end));
  process.exit(1);
}

const c = p.content || p.contenu || {};
const out = {
  pid: p.pid,
  name: c.name,
  difficulty: p.difficulty,
  fullScore: p.fullScore,
  tags: p.tags,
  provider: p.provider && p.provider.name,
  description: c.description,
  inputFormat: c.formatI,
  outputFormat: c.formatO,
  hint: c.hint,
  samples: p.samples,
  limits: p.limits && { time: (p.limits.time || [])[0], memoryKB: (p.limits.memory || [])[0] },
};
for (const [key, v] of Object.entries(out)) {
  if (v === undefined || v === null) continue;
  console.log('===== ' + key + ' =====');
  console.log(typeof v === 'string' ? v : JSON.stringify(v));
}
