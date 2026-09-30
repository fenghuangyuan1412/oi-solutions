// 【验算脚本，不是题解代码】题解一律看 solution.cpp / solution_full.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp      -o sol_brute.exe
//   g++ -static -O2 -std=c++14 -Wall solution_full.cpp -o sol_full.exe
//   node verify.js sol_brute.exe sol_full.exe
// 参照实现是一份独立的 JS 模拟器（每轮显式 sort，编号小的优先），与两份 C++ 都不共享代码。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const A = process.argv[2] || path.join(__dirname, 'sol_brute.exe');
const B = process.argv[3] || path.join(__dirname, 'sol_full.exe');
for (const e of [A, B]) if (!fs.existsSync(e)) {
  console.error(`找不到可执行文件 ${e}\n请先在本目录编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol_brute.exe 以及 solution_full.cpp -o sol_full.exe`);
  process.exit(1);
}
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (exe, input) => norm(execFileSync(exe, { input, encoding: 'utf8', maxBuffer: 1 << 26 }));

// ---- 官方样例 ----
{
  const inp = '2 4 2\n7 6 6 7\n10 5 20 15\n';
  const out = '1';
  for (const [tag, exe] of [['暴力版', A], ['归并版', B]]) {
    const got = run(exe, inp);
    console.log(`${tag} 官方样例：期望 ${out} 实际 ${got} ${got === out ? '✅' : '❌'}`);
    if (got !== out) process.exitCode = 1;
  }
}

// ---- 独立参照：真的按题面一步步模拟 ----
function ref(n, r, q, score, rank) {
  const p = score.map((s, i) => ({ id: i + 1, s, w: rank[i] }));
  for (let round = 1; round <= r; round++) {
    p.sort((x, y) => (y.s - x.s) || (x.id - y.id));          // 同分按编号升序（题面保证编号相邻配对）
    const g = [];
    for (let i = 0; i < 2 * n; i += 2) {
      const a = p[i], b = p[i + 1];
      if (a.w > b.w) { a.s++; g.push(a); g.push(b); }
      else { b.s++; g.push(b); g.push(a); }
    }
    g.sort((x, y) => (y.s - x.s) || (x.id - y.id));          // 赢者在前，各自内部有序
    p.length = 0; p.push(...g);
  }
  p.sort((x, y) => (y.s - x.s) || (x.id - y.id));
  return String(p[q - 1].id);
}

// ---- 随机对拍：两份 C++ 互拍 + 各自与 JS 参照拍 ----
let bad = 0, badRef = 0, cases = 0;
const ROUNDS = Number(process.env.ROUNDS || 1500);
for (let t = 0; t < ROUNDS; t++) {
  const n = 1 + Math.floor(Math.random() * 10);
  const r = 1 + Math.floor(Math.random() * 10);
  const q = 1 + Math.floor(Math.random() * 2 * n);
  const score = [], rank = [];
  for (let i = 0; i < 2 * n; i++) score.push(Math.floor(Math.random() * 11));   // 0..10，故意大量同分
  for (let i = 1; i <= 2 * n; i++) rank.push(i);
  for (let i = 2 * n - 1; i > 0; i--) { const j = Math.floor(Math.random() * (i + 1));[rank[i], rank[j]] = [rank[j], rank[i]]; }
  const inp = `${n} ${r} ${q}\n${score.join(' ')}\n${rank.join(' ')}\n`;
  const ga = run(A, inp), gb = run(B, inp), want = ref(n, r, q, score, rank);
  cases++;
  if (ga !== gb) { bad++; if (bad === 1) console.log('两版不一致:\n' + inp + '暴力 ' + ga + ' 归并 ' + gb); }
  if (ga !== want) { badRef++; if (badRef === 1) console.log('与参照不一致:\n' + inp + '参照 ' + want + ' 暴力 ' + ga); }
}
console.log(`P1309 对拍 ${cases} 组（n<=10, r<=10，同分密集）：暴力 vs 归并不一致 ${bad}；暴力 vs JS 参照不一致 ${badRef}`);
if (bad || badRef) process.exitCode = 1;

// ---- 极限规模计时：n=1e5, r=50, q=2n ----
{
  const n = 100000, r = 50, q = 2 * n;
  const score = [], rank = [];
  for (let i = 0; i < 2 * n; i++) score.push(Math.floor(Math.random() * 100000001));
  for (let i = 1; i <= 2 * n; i++) rank.push(i);
  for (let i = 2 * n - 1; i > 0; i--) { const j = Math.floor(Math.random() * (i + 1));[rank[i], rank[j]] = [rank[j], rank[i]]; }
  const inp = `${n} ${r} ${q}\n${score.join(' ')}\n${rank.join(' ')}\n`;
  for (const [tag, exe] of [['暴力版', A], ['归并版', B]]) {
    const ts = [];
    let out = '';
    for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(exe, inp); ts.push(Date.now() - t0); }
    console.log(`${tag} 极限 n=1e5,r=50：输出 ${out}，耗时 ${ts.join('/') } ms`);
  }
}
