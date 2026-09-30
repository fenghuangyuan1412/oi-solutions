// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                     （默认跑同目录 sol.exe，*.exe 已被 .gitignore 忽略）
//   node verify.js 路径/别的.exe
// 做两件事：① 官方样例逐字比对；② 与"真的拿二维数组一层层涂地毯"的独立参照实现随机对拍。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) { console.error(`找不到可执行文件 ${EXE}\n请先在本目录编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

// ---- ① 官方样例 ----
const SAMPLES = [
  { in: '3\n1 0 2 3\n0 2 3 3\n2 1 3 3\n2 2', out: '3' },
  { in: '3\n1 0 2 3\n0 2 3 3\n2 1 3 3\n4 5', out: '-1' },
];
for (const s of SAMPLES) {
  const got = run(s.in);
  console.log(`样例 in=(${s.in.split('\n').slice(-1)[0]}) 期望 ${s.out} 实际 ${got} ${got === s.out ? '✅' : '❌'}`);
  if (got !== s.out) process.exitCode = 1;
}

// ---- ② 参照实现：小规模时真的把网格涂出来（坐标从 0 开始，越界部分忽略）----
function refPaint(n, carpets, x, y) {
  const S = 64;
  const g = Array.from({ length: S }, () => new Array(S).fill(0));
  for (let i = 0; i < n; i++) {
    const [a, b, gg, k] = carpets[i];
    for (let px = a; px <= Math.min(S - 1, a + gg); px++)
      for (let py = b; py <= Math.min(S - 1, b + k); py++) g[px][py] = i + 1;   // 后铺的覆盖先铺的
  }
  return x < S && y < S && g[x][y] ? String(g[x][y]) : '-1';
}

let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 800);
for (let t = 0; t < ROUNDS; t++) {
  const n = 1 + rnd(8);
  const cs = [];
  let inp = n + '\n';
  for (let i = 0; i < n; i++) { const a = rnd(20), b = rnd(20), g = rnd(15), k = rnd(15); cs.push([a, b, g, k]); inp += `${a} ${b} ${g} ${k}\n`; }
  const x = rnd(30), y = rnd(30);
  inp += `${x} ${y}\n`;
  if (run(inp) !== refPaint(n, cs, x, y)) { bad++; if (bad === 1) console.log('首个不一致:\n' + inp); }
}
console.log(`P1003 与"真的涂网格"参照对拍 ${ROUNDS} 组（n<=8, 坐标<=34），不一致 ${bad}`);
if (bad) process.exitCode = 1;

// ---- ③ 极限规模计时：n=10000、坐标上到 1e5，涂网格不可行，倒序枚举应瞬时完成 ----
{
  const n = 10000;
  let inp = n + '\n';
  for (let i = 0; i < n; i++) inp += `${rnd(100000)} ${rnd(100000)} ${rnd(100000)} ${rnd(100000)}\n`;
  inp += '50000 50000\n';
  const t0 = Date.now();
  const got = run(inp);
  console.log(`极限 n=10000 随机数据：输出 ${got}，耗时 ${Date.now() - t0} ms`);
}
