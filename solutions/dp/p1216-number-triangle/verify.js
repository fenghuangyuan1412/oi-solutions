// P1216 数字三角形 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                —— 官方样例 + JS 双实现随机对拍 400 组
//       node verify.js --exe PATH     —— 改成把编译好的 C++ 程序当正解，与 DFS 暴力对拍 400 组
//       node verify.js --build        —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp 再跑
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const SAMPLE_IN = '5\n7\n3 8\n8 1 0\n2 7 4 4\n4 5 2 6 5\n';
const SAMPLE_OUT = '30';

// 正解思路：自底向上滚动一行（与 solution.cpp 同一递推，独立实现）
function dpSolve(rows) {
  const r = rows.length;
  let f = rows[r - 1].slice();
  for (let i = r - 2; i >= 0; i--) {
    const g = f.slice();
    for (let j = 0; j <= i; j++) f[j] = rows[i][j] + Math.max(g[j], g[j + 1]);
    f.length = i + 1;
  }
  return f[0];
}

// 暴力思路：DFS 枚举全部 2^(R-1) 条路径，逐条求和取最大
function dfsSolve(rows) {
  const R = rows.length;
  let best = -Infinity;
  (function go(i, j, s) {
    if (i === R - 1) { best = Math.max(best, s); return; }
    go(i + 1, j, s + rows[i + 1][j]);
    go(i + 1, j + 1, s + rows[i + 1][j + 1]);
  })(0, 0, rows[0][0]);
  return best;
}

function toInput(rows) {
  return rows.length + '\n' + rows.map((x) => x.join(' ')).join('\n') + '\n';
}
const strip = (s) => String(s).replace(/[\r\s]+$/g, '');

let seed = 20261001;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const R = ri(1, 10), rows = [];
  for (let i = 1; i <= R; i++) { const row = []; for (let j = 1; j <= i; j++) row.push(ri(0, 100)); rows.push(row); }
  return rows;
}

const exeIdx = process.argv.indexOf('--exe');
const exe = exeIdx > 0 ? path.resolve(process.argv[exeIdx + 1]) : null;
if (process.argv.includes('--build')) {
  execFileSync('g++', ['-static', '-O2', '-std=c++14', path.join(__dirname, 'solution.cpp'), '-o', path.join(__dirname, 'solution.exe')]);
}
const runExe = (input) => strip(execFileSync(exe, [], { input }));

// —— 官方样例 ——
if (exe) {
  const got = runExe(SAMPLE_IN);
  console.log('官方样例：C++ 程序输出 [' + got + ']，期望 [' + SAMPLE_OUT + '] ' + (got === SAMPLE_OUT ? 'OK' : 'MISMATCH'));
  if (got !== SAMPLE_OUT) process.exit(1);
}

const ROUNDS = Number(process.argv.find((a) => /^--rounds=\d+$/.test(a))?.split('=')[1] || 400);
let bad = 0;
for (let t = 0; t < ROUNDS; t++) {
  const rows = genCase();
  const want = dfsSolve(rows);
  const got = exe ? runExe(toInput(rows)) : String(dpSolve(rows));
  if (String(want) !== got) {
    bad++;
    if (bad <= 3) console.log('MISMATCH R=' + rows.length + '\n' + toInput(rows) + 'brute=' + want + ' got=' + got);
  }
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 滚动数组 DP') + ' vs DFS 全路径暴力 ---');
console.log('共 ' + ROUNDS + ' 组（R<=10），不一致 ' + bad + ' 组');

// —— 定向边界：R=1、全 0、塔底唯一最大 ——
const EDGE = [
  { rows: [[5]], want: 5, why: '只有一行' },
  { rows: [[0], [0, 0]], want: 0, why: '全 0' },
  { rows: [[1], [1, 1], [1, 100, 1]], want: 102, why: '最大值埋在塔底中间，贪心走不到' },
];
let edgeBad = 0;
for (const e of EDGE) {
  const got = exe ? runExe(toInput(e.rows)) : String(dpSolve(e.rows));
  if (String(e.want) !== got) { edgeBad++; console.log('EDGE FAIL ' + e.why + ' 期望 ' + e.want + ' 得 ' + got); }
}
console.log('定向边界 ' + EDGE.length + ' 项，失败 ' + edgeBad + ' 项');
