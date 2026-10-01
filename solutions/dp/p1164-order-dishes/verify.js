// P1164 小 A 点菜 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                —— 官方样例 + JS 双实现随机对拍 400 组
//       node verify.js --exe PATH     —— 改成把编译好的 C++ 程序当正解，与子集枚举暴力对拍 400 组
//       node verify.js --build        —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp 再跑
const { execFileSync } = require('child_process');
const path = require('path');

const SAMPLE_IN = '4 4\n1 1 2 2\n';
const SAMPLE_OUT = '3';

// 正解思路：01 背包计数，f[0] = 1，内层倒序 f[j] += f[j - a]（与 solution.cpp 同一递推，独立实现）
function dpSolve(prices, m) {
  const f = new Array(m + 1).fill(0);
  f[0] = 1;                                  // "什么都不点"是花 0 元的唯一方案
  for (const a of prices)
    for (let j = m; j >= a; j--) f[j] += f[j - a];
  return f[m];
}

// 暴力思路：枚举 2^N 个菜品子集，统计价格之和恰好等于 M 的个数（下标不同即不同方案）
function bruteSolve(prices, m) {
  const n = prices.length;
  let cnt = 0;
  for (let mask = 0; mask < (1 << n); mask++) {
    let sum = 0;
    for (let i = 0; i < n; i++) if (mask >> i & 1) sum += prices[i];
    if (sum === m) cnt++;
  }
  return cnt;
}

function toInput(prices, m) {
  return prices.length + ' ' + m + '\n' + prices.join(' ') + '\n';
}
const strip = (s) => String(s).replace(/[\r\s]+$/g, '');

let seed = 20261001;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const n = ri(1, 16), m = ri(1, 40), prices = [];
  for (let i = 0; i < n; i++) prices.push(ri(1, 40));   // 价格可超过 m，也可能重复
  return { prices, m };
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
  const { prices, m } = genCase();
  const want = bruteSolve(prices, m);
  const got = exe ? runExe(toInput(prices, m)) : String(dpSolve(prices, m));
  if (String(want) !== got) {
    bad++;
    if (bad <= 3) console.log('MISMATCH N=' + prices.length + ' M=' + m + '\n' + toInput(prices, m) + 'brute=' + want + ' got=' + got);
  }
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 倒序 01 背包计数') + ' vs 2^N 子集枚举暴力 ---');
console.log('共 ' + ROUNDS + ' 组（N<=16、M<=40），不一致 ' + bad + ' 组');

// —— 定向边界：空方案、点不起、同价不同种、恰好一道菜花光 ——
const EDGE = [
  { prices: [1], m: 1, want: 1, why: '一道 1 元菜花光 1 元' },
  { prices: [1], m: 2, want: 0, why: '钱花不光' },
  { prices: [5, 7], m: 3, want: 0, why: '所有菜都比 M 贵，只能什么都不点，但什么都不点是花 0 元' },
  { prices: [1, 1, 2], m: 2, want: 2, why: '同价不同种要分开数：点 2 元那道 / 点两盘 1 元' },
  { prices: [1, 2, 5], m: 5, want: 1, why: '只有一道菜正好 5 元，1+2 凑不到 5' },
  { prices: [1, 1, 2, 2], m: 4, want: 3, why: '官方样例' },
];
let edgeBad = 0;
for (const e of EDGE) {
  const got = exe ? runExe(toInput(e.prices, e.m)) : String(dpSolve(e.prices, e.m));
  if (String(e.want) !== got) { edgeBad++; console.log('EDGE FAIL ' + e.why + ' 期望 ' + e.want + ' 得 ' + got); }
}
console.log('定向边界 ' + EDGE.length + ' 项，失败 ' + edgeBad + ' 项');
