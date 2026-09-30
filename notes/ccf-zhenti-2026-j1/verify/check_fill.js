// 完善程序 (1)(2) 逐空验证：把每个空的 A/B/C/D 四个候选各编一遍，
// 其余空保持参考答案的填法，然后和独立参考实现对拍 cases*/inNNNN.txt。
// 用法：node check_fill.js            （默认 300 组）
//       node check_fill.js 5000       （换目录 cases1_5000 / cases2_5000，需先生成）
const fs = require('fs');
const path = require('path');
const { execFileSync } = require('child_process');

const ROOT = __dirname;
const N = Number(process.argv[2] || 300);
const G = `g++`;
const FLAGS = ['-static', '-O2', '-std=c++14'];

function compile(src, out, defines) {
  const args = [...FLAGS, src, '-o', out];
  for (const [k, v] of Object.entries(defines)) args.push(`-D${k}=${v}`);
  try {
    execFileSync(G, args, { cwd: ROOT, stdio: ['ignore', 'ignore', 'pipe'] });
    return null;
  } catch (e) {
    return String(e.stderr || e.message).split('\n')[0];
  }
}

function run(exe, file) {
  try {
    return {
      code: 0,
      out: execFileSync('./' + exe, { cwd: ROOT, input: fs.readFileSync(path.join(ROOT, file)), timeout: 10000 })
        .toString().replace(/\s+$/g, ''),
    };
  } catch (e) {
    if (e.code === 'ETIMEDOUT') return { code: -1, out: '<TIMEOUT/死循环>' };
    return { code: e.status == null ? -2 : e.status, out: '<运行失败>' };
  }
}

const SPECS = {
  fill1: {
    src: 'src/fill1_tmpl.cpp', ref: 'fill1_ref.exe', cases: `cases1`,
    blanks: [
      { name: 'BLANK1', label: '题34 ①', opts: ['b[j] * n', 'b[j] * m', 'b[j - 1] * n', 'b[j - 1] * m'], key: 'D' },
      { name: 'BLANK2', label: '题35 ②', opts: ['x * n', 'x', '0', 'm'], key: 'B' },
      { name: 'BLANK3', label: '题36 ③', opts: ['b[j] / m', 'b[j] % n', 'b[j] % m', 'b[j] / n'], key: 'D' },
      { name: 'BLANK4', label: '题37 ④', opts: ['b[j] / m', 'b[j] % n', 'b[j] % m', 'b[j] / n'], key: 'B' },
      { name: 'BLANK5', label: '题38 ⑤', opts: ['len > 0 && b[len - 1] == 0', 'len > 0 && b[0] == 0', 'len > 1 && b[len - 1] == 0', 'len > 1 && b[0] == 0'], key: 'C' },
    ],
  },
  fill2: {
    src: 'src/fill2_tmpl.cpp', ref: 'fill2_ref.exe', cases: `cases2`,
    blanks: [
      { name: 'BLANK1', label: '题39 ①', opts: ["c - '0'", "c - (c < 'A' ? '0' : 'A' - 10)", "c - 'A' + 10", "c"], key: 'B' },
      { name: 'BLANK2', label: '题40 ②', opts: ['int r = 1; r <= n; ++r', 'int r = l; r < n; ++r', 'int r = l; r <= n - 1; ++r', 'int r = l; r <= n; ++r'], key: 'D' },
      { name: 'BLANK3', label: '题41 ③', opts: ['sum', 'sum / (r - l + 1)', 'sum * 1.0 / (r - l + 1)', 'sum * 1.0 / (r - l)'], key: 'C' },
      { name: 'BLANK4', label: '题42 ④', opts: ['r + 1, cnt + (r < n), min(mnb, nwb), max(mxb, nwb)', 'r, cnt + (r < n), min(mnb, nwb), max(mxb, nwb)', 'r + 1, cnt + 1, min(mnb, nwb), max(mxb, nwb)', 'r + 1, cnt + (r < n), nwb, mxb'], key: 'A' },
      { name: 'BLANK5', label: '题43 ⑤', opts: ['1, 0, 0, 0', '1, 1, 1e100, -1e100', '0, 0, 1e100, -1e100', '1, 0, 1e100, -1e100'], key: 'D' },
    ],
  },
};

const lines = [];
for (const [prog, spec] of Object.entries(SPECS)) {
  const files = fs.readdirSync(path.join(ROOT, spec.cases)).filter((f) => f.endsWith('.txt')).sort();
  if (files.length !== N) lines.push(`⚠️ ${spec.cases} 里有 ${files.length} 组，和 --rounds ${N} 不一致`);
  const refOut = files.map((f) => run(spec.ref, path.join(spec.cases, f)).out);

  lines.push('', `### ${prog}：${files.length} 组随机数据对拍`, '');
  lines.push('| 空 | 选项 | 表达式 | 结果 |');
  lines.push('|---|---|---|---|');
  for (const b of spec.blanks) {
    for (let i = 0; i < 4; i++) {
      const letter = 'ABCD'[i];
      const defines = {};
      spec.blanks.forEach((x) => { defines[x.name] = x.opts['ABCD'.indexOf(x.key)]; });
      defines[b.name] = b.opts[i];
      const exe = `tmp_${prog}_${b.name}_${letter}.exe`;
      const err = compile(spec.src, exe, defines);
      const mark = letter === b.key ? ' ← **卷面参考答案**' : '';
      if (err) { lines.push(`| ${b.label} | ${letter}${mark} | \`${b.opts[i]}\` | ❌ 编译不过：${err} |`); continue; }
      {
        let bad = 0, firstBad = '';
        for (let k = 0; k < files.length; k++) {
          const got = run(exe, path.join(spec.cases, files[k])).out;
          if (got !== refOut[k]) { bad++; if (!firstBad) firstBad = `${files[k]}：期望 [${refOut[k]}] 实得 [${got}]`; }
        }
        const verdict = bad === 0 ? `✅ 全对（${files.length}/${files.length}）` : `❌ ${bad}/${files.length} 组不一致`;
        fs.unlinkSync(path.join(ROOT, exe));
        const mark = letter === b.key ? ' ← **卷面参考答案**' : '';
        lines.push(`| ${b.label} | ${letter}${mark} | \`${b.opts[i]}\` | ${verdict} |`);
        if (bad) lines.push(`| | | 首个反例 | ${firstBad} |`);
      }
    }
  }
}
console.log(lines.join('\n'));
