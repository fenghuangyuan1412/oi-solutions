// P2678 跳石头 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                                   —— JS 双实现随机对拍 3000 组
//       node verify.js --build                           —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp
//       node verify.js --exe ./solution.exe --rounds=3000 —— 用 C++ 程序当正解再对拍
const { execFileSync } = require('child_process');
const path = require('path');

const SAMPLE_IN = '25 5 2\n2\n11\n14\n17\n21\n';
const SAMPLE_OUT = '4';

// 定向边界 / 专门打"多趟扫描重复计数"错版的 hack（见 README 易错点 3）
const CASES = [
  ['4 1 1\n2\n', '4', 'Hack1：单块石头同时贴近起点与终点'],
  ['10 2 1\n4\n7\n', '4', 'Hack2：靠近终点的石头与前面保留的石头同时过近'],
  ['1 0 0\n', '1', 'N=0：中间没有石头，答案就是 L'],
  ['100 3 3\n10\n20\n30\n', '100', 'M=N：可以全移走，答案就是 L'],
  ['10 3 0\n3\n6\n9\n', '1', 'M=0：一块都不能移，答案是最小原始间距'],
];

// 正解：二分答案 + 单趟贪心（与 solution.cpp 同一算法，独立实现）
function solve(L, d, m) {
  const n = d.length;
  const a = [0, ...d];
  const check = (X) => {
    let cnt = 0, last = 0;
    for (let i = 1; i <= n; i++) { if (a[i] - a[last] < X) cnt++; else last = i; }
    if (L - a[last] < X) cnt++;
    return cnt <= m;
  };
  let lo = 0, hi = L, ans = 0;
  while (lo <= hi) {
    const mid = lo + ((hi - lo) >> 1);
    if (check(mid)) { ans = mid; lo = mid + 1; } else hi = mid - 1;
  }
  return ans;
}

// 暴力：枚举保留哪几块石头（2^N），算相邻落点距离的最小值，取最大。与贪心/二分完全无关
function brute(L, d, m) {
  const n = d.length;
  let best = 0;
  for (let s = 0; s < (1 << n); s++) {
    let removed = 0;
    for (let i = 0; i < n; i++) if (!((s >> i) & 1)) removed++;
    if (removed > m) continue;
    const pos = [0];
    for (let i = 0; i < n; i++) if ((s >> i) & 1) pos.push(d[i]);
    pos.push(L);
    let mn = Infinity;
    for (let i = 1; i < pos.length; i++) mn = Math.min(mn, pos[i] - pos[i - 1]);
    if (mn > best) best = mn;
  }
  return best;
}

// 错版：把"太近"的条件写成 <=，等价于求"距离严格大于 X"，答案系统性 −1
function solveWrongLE(L, d, m) {
  const n = d.length;
  const a = [0, ...d];
  const check = (X) => {
    let cnt = 0, last = 0;
    for (let i = 1; i <= n; i++) { if (a[i] - a[last] <= X) cnt++; else last = i; }
    if (L - a[last] <= X) cnt++;
    return cnt <= m;
  };
  let lo = 0, hi = L, ans = 0;
  while (lo <= hi) {
    const mid = lo + ((hi - lo) >> 1);
    if (check(mid)) { ans = mid; lo = mid + 1; } else hi = mid - 1;
  }
  return ans;
}

let seed = 20261003;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const L = ri(1, 25);
  const n = ri(0, Math.min(10, L - 1));
  const pool = [];
  for (let v = 1; v < L; v++) pool.push(v);
  // 洗牌后取 n 个并升序 —— 保证互不相同且在 (0, L) 内
  for (let i = pool.length - 1; i > 0; i--) { const j = ri(0, i); [pool[i], pool[j]] = [pool[j], pool[i]]; }
  const d = pool.slice(0, n).sort((x, y) => x - y);
  const m = ri(0, n);
  return { L, n, d, m };
}
const toInput = ({ L, n, d, m }) => L + ' ' + n + ' ' + m + '\n' + (d.length ? d.join('\n') + '\n' : '');
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

console.log('--- 定向边界 / hack ---');
let caseBad = 0;
for (const [input, want, why] of CASES) {
  const got = exe ? strip(execFileSync(exe, [], { input })) : String(solve(...parseInput(input)));
  const okk = got === want;
  if (!okk) caseBad++;
  console.log('  [' + (okk ? 'OK' : 'FAIL') + '] ' + why + '  期望 ' + want + ' 实得 ' + got);
}
function parseInput(txt) {
  const t = txt.trim().split(/\s+/).map(Number);
  const L = t[0], n = t[1], m = t[2];
  return [L, t.slice(3, 3 + n), m];
}

const ROUNDS = Number((process.argv.find((a) => /^--rounds=\d+$/.test(a)) || '').split('=')[1] || 500);
let bad = 0, wrongLE = 0;
for (let r = 0; r < ROUNDS; r++) {
  const c = genCase();
  const want = brute(c.L, c.d, c.m);
  const got = exe ? strip(execFileSync(exe, [], { input: toInput(c) })) : String(solve(c.L, c.d, c.m));
  if (String(want) !== got) { bad++; if (bad <= 3) console.log('MISMATCH ' + toInput(c) + 'brute=' + want + ' got=' + got); }
  if (solveWrongLE(c.L, c.d, c.m) !== want) wrongLE++;
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 二分+贪心') + ' vs 2^N 枚举保留集合暴力 ---');
console.log('共 ' + ROUNDS + ' 组（L<=25, N<=10），不一致 ' + bad + ' 组；定向边界失败 ' + caseBad + ' 项');
console.log('其中 ' + wrongLE + ' 组里"太近写成 <= "的错版结果与正解不同 —— 它求的是"距离严格大于 X"，答案系统性 −1');
