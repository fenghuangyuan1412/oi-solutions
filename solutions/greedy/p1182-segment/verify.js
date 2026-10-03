// P1182 数列分段 Section II · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                                   —— JS 双实现随机对拍 3000 组
//       node verify.js --build                           —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp
//       node verify.js --exe ./solution.exe --rounds=3000 —— 用 C++ 程序当正解再对拍
const { execFileSync } = require('child_process');
const path = require('path');

const SAMPLE_IN = '5 3\n4 2 4 5 1\n';
const SAMPLE_OUT = '6';

// 正解：二分答案 + 贪心分段（与 solution.cpp 同一算法，独立实现）
function solveBinary(a, m) {
  const n = a.length;
  let lo = 0, hi = 0;
  for (const x of a) { hi += x; if (x > lo) lo = x; }
  const check = (X) => {
    let cur = 0, cnt = 1;
    for (const x of a) { if (cur + x > X) { cnt++; cur = x; } else cur += x; }
    return cnt <= m;
  };
  let ans = hi;
  while (lo <= hi) {
    const mid = lo + ((hi - lo) >> 1);
    if (check(mid)) { ans = mid; hi = mid - 1; } else lo = mid + 1;
  }
  return ans;
}

// 暴力：DP，f[i][j] = 前 i 个数切成 j 段时"最大段和"的最小值。O(N^2 * M)，与贪心/二分完全无关
function bruteDP(a, m) {
  const n = a.length, INF = Number.MAX_SAFE_INTEGER;
  const pre = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i - 1];
  const sum = (l, r) => pre[r] - pre[l - 1];
  let f = new Array(n + 1).fill(INF);
  f[0] = 0;
  for (let j = 1; j <= m; j++) {
    const g = new Array(n + 1).fill(INF);
    for (let i = j; i <= n; i++)
      for (let k = j - 1; k < i; k++)
        g[i] = Math.min(g[i], Math.max(f[k], sum(k + 1, i)));
    f = g;
  }
  return f[n];
}

// 错版：把收口条件写成 >=（即"段和必须严格小于 X"），答案会系统性 +1
function solveWrongGE(a, m) {
  const n = a.length;
  let lo = 0, hi = 0;
  for (const x of a) { hi += x; if (x > lo) lo = x; }
  const check = (X) => {
    let cur = 0, cnt = 1;
    for (const x of a) { if (cur + x >= X) { cnt++; cur = x; } else cur += x; }
    return cnt <= m;
  };
  let ans = hi;
  while (lo <= hi) {
    const mid = lo + ((hi - lo) >> 1);
    if (check(mid)) { ans = mid; hi = mid - 1; } else lo = mid + 1;
  }
  return ans;
}

let seed = 20261003;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const n = ri(1, 9), m = ri(1, n), a = [];
  for (let i = 0; i < n; i++) a.push(ri(0, 12));   // 含 0，专门压 0 边界
  return { n, m, a };
}
const toInput = ({ n, m, a }) => n + ' ' + m + '\n' + a.join(' ') + '\n';
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

const ROUNDS = Number((process.argv.find((a) => /^--rounds=\d+$/.test(a)) || '').split('=')[1] || 500);
let bad = 0, wrongGE = 0;
for (let r = 0; r < ROUNDS; r++) {
  const c = genCase();
  const want = bruteDP(c.a, c.m);
  const got = exe ? strip(execFileSync(exe, [], { input: toInput(c) })) : String(solveBinary(c.a, c.m));
  if (String(want) !== got) { bad++; if (bad <= 3) console.log('MISMATCH ' + toInput(c) + 'brute=' + want + ' got=' + got); }
  if (solveWrongGE(c.a, c.m) !== want) wrongGE++;
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 二分+贪心') + ' vs O(N^2 M) DP 暴力 ---');
console.log('共 ' + ROUNDS + ' 组（N<=9, 含 0 值），不一致 ' + bad + ' 组');
console.log('其中 ' + wrongGE + ' 组里"收口写成 >= "的错版结果与正解不同 —— 它求的是"段和严格小于 X"，答案系统性 +1');
