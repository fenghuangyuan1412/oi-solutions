// P1616 疯狂的采药 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                                   —— JS 双实现随机对拍 2000 组
//       node verify.js --build                           —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp
//       node verify.js --exe ./solution.exe --rounds=2000 —— 用 C++ 程序当正解再对拍
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const SAMPLE_IN = '70 3\n71 100\n69 1\n1 2\n';
const SAMPLE_OUT = '140';   // 与 P1048 采药【同一组样例输入】，但答案从 3 变成 140

// 正解：一维完全背包（内层正序）
function complete(T, items) {
  const f = new Array(T + 1).fill(0);
  for (const [a, b] of items)
    for (let j = a; j <= T; j++) f[j] = Math.max(f[j], f[j - a] + b);
  return f[T];
}

// 01 背包（内层倒序）—— 与 P1048 同款，用来量化"方向写反"的差距
function zeroOne(T, items) {
  const f = new Array(T + 1).fill(0);
  for (const [a, b] of items)
    for (let j = T; j >= a; j--) f[j] = Math.max(f[j], f[j - a] + b);
  return f[T];
}

// 暴力：DFS 枚举每种草药采几株，总时间不超过 T
function brute(T, items) {
  let best = 0;
  const dfs = (i, used, val) => {
    if (i === items.length) { if (val > best) best = val; return; }
    const [a, b] = items[i];
    for (let k = 0; used + k * a <= T; k++) dfs(i + 1, used + k * a, val + k * b);
  };
  dfs(0, 0, 0);
  return best;
}

let seed = 20261003;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const T = ri(1, 60), M = ri(1, 6), items = [];
  for (let i = 0; i < M; i++) items.push([ri(1, 12), ri(1, 20)]);  // 值域压小，逼出重复采摘
  return { T, M, items };
}
const toInput = ({ T, M, items }) => T + ' ' + M + '\n' + items.map((x) => x.join(' ')).join('\n') + '\n';
const strip = (s) => String(s).replace(/[\r\s]+$/g, '');

const exeIdx = process.argv.indexOf('--exe');
const exe = exeIdx > 0 ? path.resolve(process.argv[exeIdx + 1]) : null;
if (process.argv.includes('--build')) {
  execFileSync('g++', ['-static', '-O2', '-std=c++14', path.join(__dirname, 'solution.cpp'), '-o', path.join(__dirname, 'solution.exe')]);
}

console.log('样例输入：' + JSON.stringify(SAMPLE_IN) + ' 期望输出 ' + SAMPLE_OUT);
console.log('  完全背包(正序)  = ' + complete(70, [[71, 100], [69, 1], [1, 2]]));
console.log('  01 背包(倒序)   = ' + zeroOne(70, [[71, 100], [69, 1], [1, 2]]) + '   <- 这就是 P1048 采药的答案');

if (exe) {
  const got = strip(execFileSync(exe, [], { input: SAMPLE_IN }));
  console.log('官方样例：C++ 输出 [' + got + '] 期望 [' + SAMPLE_OUT + '] ' + (got === SAMPLE_OUT ? 'OK' : 'MISMATCH'));
  if (got !== SAMPLE_OUT) process.exit(1);
}

const ROUNDS = Number((process.argv.find((a) => /^--rounds=\d+$/.test(a)) || '').split('=')[1] || 500);
let bad = 0, z01 = 0;
for (let r = 0; r < ROUNDS; r++) {
  const c = genCase();
  const want = brute(c.T, c.items);
  const got = exe ? strip(execFileSync(exe, [], { input: toInput(c) })) : String(complete(c.T, c.items));
  if (String(want) !== got) { bad++; if (bad <= 3) console.log('MISMATCH ' + toInput(c) + 'brute=' + want + ' got=' + got); }
  if (zeroOne(c.T, c.items) !== want) z01++;
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 正序完全背包') + ' vs DFS 枚举株数暴力 ---');
console.log('共 ' + ROUNDS + ' 组（T<=60, M<=6），不一致 ' + bad + ' 组');
console.log('其中 ' + z01 + ' 组里"倒序（01 背包）"的结果严格小于正解 —— 少采了重复的株数，方向写反是系统性偏小');
