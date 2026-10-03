// P1880 石子合并 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                                   —— JS 双实现随机对拍 2000 组
//       node verify.js --build                           —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp
//       node verify.js --exe ./solution.exe --rounds=2000 —— 用 C++ 程序当正解再对拍
const { execFileSync } = require('child_process');
const path = require('path');

const SAMPLE_IN = '4\n4 5 9 4\n';
const SAMPLE_OUT = '43\n54';

// 正解：断环成链 + 区间 DP（与 solution.cpp 同一算法，独立实现）
function solveDP(a) {
  const n = a.length;
  const b = [0].concat(a, a);                 // 1-indexed，长度 2n
  const pre = new Array(2 * n + 1).fill(0);
  for (let i = 1; i <= 2 * n; i++) pre[i] = pre[i - 1] + b[i];
  const INF = Infinity;
  const mn = Array.from({ length: 2 * n + 1 }, () => new Array(2 * n + 1).fill(0));
  const mx = Array.from({ length: 2 * n + 1 }, () => new Array(2 * n + 1).fill(0));
  for (let len = 2; len <= n; len++)
    for (let i = 1; i + len - 1 <= 2 * n; i++) {
      const j = i + len - 1, s = pre[j] - pre[i - 1];
      let lo = INF, hi = -INF;
      for (let k = i; k < j; k++) {
        lo = Math.min(lo, mn[i][k] + mn[k + 1][j] + s);
        hi = Math.max(hi, mx[i][k] + mx[k + 1][j] + s);
      }
      mn[i][j] = lo; mx[i][j] = hi;
    }
  let ansmin = INF, ansmax = -INF;
  for (let i = 1; i <= n; i++) {
    ansmin = Math.min(ansmin, mn[i][i + n - 1]);
    ansmax = Math.max(ansmax, mx[i][i + n - 1]);
  }
  return [ansmin, ansmax];
}

// 暴力：直接枚举所有合并顺序（每次挑相邻两堆并起来），不做任何区间 DP
function bruteMerge(piles) {
  let best = Infinity, worst = -Infinity;
  const rec = (arr, score) => {
    const k = arr.length;
    if (k === 1) { if (score < best) best = score; if (score > worst) worst = score; return; }
    for (let i = 0; i < k; i++) {
      const j = (i + 1) % k;
      const s = arr[i] + arr[j];
      const nx = [];
      for (let t = 0; t < k; t++) {
        if (t === i) nx.push(s);
        else if (t === j) continue;
        else nx.push(arr[t]);
      }
      rec(nx, score + s);
    }
  };
  rec(piles.slice(), 0);
  return [best, worst];
}

// 错版：忘了断环成链，直接在原数组上做直线区间 DP —— 只能拿到"不跨越首尾边界"的方案
function solveWrongLine(a) {
  const n = a.length;
  const pre = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i - 1];
  const mn = Array.from({ length: n + 1 }, () => new Array(n + 1).fill(0));
  const mx = Array.from({ length: n + 1 }, () => new Array(n + 1).fill(0));
  for (let len = 2; len <= n; len++)
    for (let i = 1; i + len - 1 <= n; i++) {
      const j = i + len - 1, s = pre[j] - pre[i - 1];
      let lo = Infinity, hi = -Infinity;
      for (let k = i; k < j; k++) {
        lo = Math.min(lo, mn[i][k] + mn[k + 1][j] + s);
        hi = Math.max(hi, mx[i][k] + mx[k + 1][j] + s);
      }
      mn[i][j] = lo; mx[i][j] = hi;
    }
  return [mn[1][n], mx[1][n]];
}

let seed = 20261003;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const n = ri(1, 7), a = [];
  for (let i = 0; i < n; i++) a.push(ri(1, 20));   // 与题面一致：1 <= a_i <= 20
  return a;
}
const toInput = (a) => a.length + '\n' + a.join(' ') + '\n';
const strip = (s) => String(s).replace(/[\r\s]+$/g, '');

const exeIdx = process.argv.indexOf('--exe');
const exe = exeIdx > 0 ? path.resolve(process.argv[exeIdx + 1]) : null;
if (process.argv.includes('--build')) {
  execFileSync('g++', ['-static', '-O2', '-std=c++14', path.join(__dirname, 'solution.cpp'), '-o', path.join(__dirname, 'solution.exe')]);
}
if (exe) {
  const got = strip(execFileSync(exe, [], { input: SAMPLE_IN }));
  const want = SAMPLE_OUT.replace(/\n/g, '\n');
  console.log('官方样例：C++ 输出 [' + got.replace(/\n/g, '/') + '] 期望 [' + want.replace(/\n/g, '/') + '] ' + (got === want ? 'OK' : 'MISMATCH'));
  if (got !== want) process.exit(1);
}

const ROUNDS = Number((process.argv.find((a) => /^--rounds=\d+$/.test(a)) || '').split('=')[1] || 500);
let bad = 0, lineWrong = 0;
for (let r = 0; r < ROUNDS; r++) {
  const c = genCase();
  const want = bruteMerge(c);
  const got = exe ? strip(execFileSync(exe, [], { input: toInput(c) })).split('\n').map(Number)
                  : solveDP(c);
  if (String(want[0]) !== String(got[0]) || String(want[1]) !== String(got[1])) {
    bad++;
    if (bad <= 3) console.log('MISMATCH n=' + c.length + ' a=[' + c.join(',') + '] brute=' + want.join('/') + ' got=' + got.join('/'));
  }
  const w = solveWrongLine(c);
  if (w[0] !== want[0] || w[1] !== want[1]) lineWrong++;
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 断环成链区间 DP') + ' vs 枚举所有合并顺序暴力 ---');
console.log('共 ' + ROUNDS + ' 组（N<=7, 1<=a_i<=20），不一致 ' + bad + ' 组');
console.log('其中 ' + lineWrong + ' 组里"忘了断环成链"的错版结果与正解不同 —— 它漏掉了跨越首尾边界的合并方案');
