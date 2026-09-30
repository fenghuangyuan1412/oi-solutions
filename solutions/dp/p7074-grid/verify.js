// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法（三个档各自编译）：
//   g++ -static -O2 -std=c++14 -Wall solution_dfs.cpp -o sol_dfs.exe     // 20 分：DFS 穷举
//   g++ -static -O2 -std=c++14 -Wall solution_mid.cpp -o sol_mid.exe     // 70 分：O(n^2 m)
//   g++ -static -O2 -std=c++14 -Wall solution.cpp     -o sol.exe         // 100 分：O(nm)
//   node verify.js [sol_dfs.exe] [sol_mid.exe] [sol.exe]
// 三个档是三条独立思路（穷举路径 / 枚举进列点 k / 前后缀最大值），互相打印不一致就算抓到 bug。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const D = process.argv[2] || path.join(__dirname, 'sol_dfs.exe');
const MID = process.argv[3] || path.join(__dirname, 'sol_mid.exe');
const F = process.argv[4] || path.join(__dirname, 'sol.exe');
const miss = [D, MID, F].filter((e) => !fs.existsSync(e));
if (miss.length) { console.error(`找不到 ${miss.join(', ')}\n请按文件头三行 g++ 命令先把三个档都编译出来。`); process.exit(1); }

const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (exe, input) => norm(execFileSync(exe, { input, encoding: 'utf8', maxBuffer: 1 << 26 }));
const rnd = (k) => Math.floor(Math.random() * k);

function grid(n, m, maxAbs) {
  let s = `${n} ${m}\n`;
  for (let i = 0; i < n; i++) {
    const row = [];
    for (let j = 0; j < m; j++) row.push(rnd(2 * maxAbs + 1) - maxAbs);
    s += row.join(' ') + '\n';
  }
  return s;
}

// ---- ① 官方样例（三个档都必须逐字一致）----
const SAMPLES = [
  { in: '3 4\n1 -1 3 2\n2 -1 4 -1\n-2 2 -3 -1', out: '9' },
  { in: '2 5\n-1 -1 -3 -2 -7\n-2 -1 -4 -1 -2', out: '-10' },
];
for (const s of SAMPLES) {
  for (const [tag, exe] of [['DFS', D], ['O(n^2m)', MID], ['O(nm)', F]]) {
    const got = run(exe, s.in + '\n');
    console.log(`样例 #${s.in.split('\n')[0]} ${tag}：期望 ${s.out} 实际 ${got} ${got === s.out ? '✅' : '❌'}`);
    if (got !== s.out) process.exitCode = 1;
  }
}

// ---- ② DFS vs 中间档 vs 满分档，n,m<=5（DFS 穷举是绝对参照）----
{
  let badMid = 0, badFull = 0;
  const R = Number(process.env.ROUNDS || 1500);
  for (let t = 0; t < R; t++) {
    const inp = grid(1 + rnd(5), 1 + rnd(5), 9);
    const want = run(D, inp);
    if (run(MID, inp) !== want) badMid++;
    if (run(F, inp) !== want) badFull++;
  }
  console.log(`② n,m<=5，值 -9..9，${R} 组：DFS vs O(n^2m) 不一致 ${badMid}；DFS vs O(nm) 不一致 ${badFull}`);
  if (badMid || badFull) process.exitCode = 1;
}

// ---- ③ 中间档 vs 满分档，n,m<=20（DFS 在这个规模跑不动，两版互拍）----
{
  let bad = 0;
  const R = Number(process.env.ROUNDS2 || 600);
  for (let t = 0; t < R; t++) {
    const inp = grid(1 + rnd(20), 1 + rnd(20), 10000);
    const a = run(MID, inp), b = run(F, inp);
    if (a !== b) { bad++; if (bad === 1) console.log('首个不一致:\n' + inp + 'mid=' + a + ' full=' + b); }
  }
  console.log(`③ n,m<=20，值 ±1e4，${R} 组：O(n^2m) vs O(nm) 不一致 ${bad}`);
  if (bad) process.exitCode = 1;
}

// ---- ④ 计时：三个档各自的能力边界 ----
function time(exe, inp, label) {
  const ts = [];
  let out = '';
  for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(exe, inp); ts.push(Date.now() - t0); }
  console.log(`④ ${label}：输出 ${out}，耗时 ${ts.join('/') } ms`);
  return out;
}
time(MID, grid(300, 300, 10000), 'O(n^2m) 在 300x300（70% 档上限）');
time(F, grid(300, 300, 10000), 'O(nm) 在 300x300');
const big = grid(1000, 1000, 10000);
time(F, big, 'O(nm) 在 1000x1000（题面上限）');
time(MID, big, 'O(n^2m) 在 1000x1000（1e9 次枚举）');
for (const k of [6, 7, 8]) {
  const inp = grid(k, k, 9);
  const t0 = Date.now();
  run(D, inp);
  console.log(`④ DFS 在 ${k}x${k}：耗时 ${Date.now() - t0} ms（题面 20% 档只保证 n,m<=5）`);
}
