// P1048 采药 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js            —— JS 双实现随机对拍 300 组
//       node verify.js --build    —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp
//       node verify.js --exe ./solution.exe --rounds=200  —— 用 C++ 程序当正解再对拍
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const SAMPLE_IN = '70 3\n71 100\n69 1\n1 2\n';
const SAMPLE_OUT = '3';

// 正解：一维滚动 + 容量倒序（与 solution.cpp 同一递推，独立实现）
function knapsack(T, items) {
  const f = new Array(T + 1).fill(0);
  for (const [t, v] of items)
    for (let j = T; j >= t; j--) f[j] = Math.max(f[j], f[j - t] + v);
  return f[T];
}

// 暴力：枚举 2^M 个采摘子集，时间和不超 T 的里面取价值最大
function brute(T, items) {
  const M = items.length;
  let best = 0;
  for (let s = 0; s < (1 << M); s++) {
    let t = 0, v = 0;
    for (let i = 0; i < M; i++) if ((s >> i) & 1) { t += items[i][0]; v += items[i][1]; }
    if (t <= T) best = Math.max(best, v);
  }
  return best;
}

// 完全背包版：只有内层方向不同，用来当"错误版本"的量级对照
function unbounded(T, items) {
  const f = new Array(T + 1).fill(0);
  for (const [t, v] of items)
    for (let j = t; j <= T; j++) f[j] = Math.max(f[j], f[j - t] + v);
  return f[T];
}

let seed = 20261001;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const T = ri(1, 60), M = ri(1, 16), items = [];
  for (let i = 0; i < M; i++) items.push([ri(1, 30), ri(1, 100)]);
  return { T, M, items };
}
const toInput = ({ T, M, items }) => T + ' ' + M + '\n' + items.map((x) => x.join(' ')).join('\n') + '\n';
const strip = (s) => String(s).replace(/[\r\s]+$/g, '');

const exeIdx = process.argv.indexOf('--exe');
const exe = exeIdx > 0 ? path.resolve(process.argv[exeIdx + 1]) : null;
if (process.argv.includes('--build')) {
  execFileSync('g++', ['-static', '-O2', '-std=c++14', path.join(__dirname, 'solution.cpp'), '-o', path.join(__dirname, 'solution.exe')]);
}
if (exe) {
  const got = strip(execFileSync(exe, [], { input: SAMPLE_IN }));
  console.log('官方样例：C++ 输出 [' + got + '] 期望 [' + SAMPLE_OUT + '] ' + (got === SAMPLE_OUT ? 'OK' : 'MISMATCH'));
  if (got !== SAMPLE_OUT) process.exit(1);
}

const ROUNDS = Number((process.argv.find((a) => /^--rounds=\d+$/.test(a)) || '').split('=')[1] || 300);
let bad = 0, unboundedBetter = 0;
for (let r = 0; r < ROUNDS; r++) {
  const c = genCase();
  const want = brute(c.T, c.items);
  const got = exe ? strip(execFileSync(exe, [], { input: toInput(c) })) : String(knapsack(c.T, c.items));
  if (String(want) !== got) { bad++; if (bad <= 3) console.log('MISMATCH ' + toInput(c) + 'brute=' + want + ' got=' + got); }
  if (unbounded(c.T, c.items) > want) unboundedBetter++;
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 倒序 01 背包') + ' vs 枚举 2^M 子集暴力 ---');
console.log('共 ' + ROUNDS + ' 组（T<=60, M<=16），不一致 ' + bad + ' 组');
console.log('其中 ' + unboundedBetter + ' 组里"正序（完全背包）"的结果比正解大 —— 方向写反不是偶尔撞上，是系统性偏大');
