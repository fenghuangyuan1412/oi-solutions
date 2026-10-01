// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 复现命令（在本目录）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js
// 若不想把 exe 落在题目目录（*.exe 已被 .gitignore 忽略，落哪都不会入库），可以：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o ../../../_work/search-c/sol.exe
//   node verify.js ../../../_work/search-c/sol.exe
//
// 三件事：
//  ① 官方样例比对。**比对口径：按"整数序列"逐项比较**，不是逐字节。
//     原因：洛谷原样例每行行末带空格，且题面明示 2022-08 后"空格或合理场宽分割都判对"，
//     逐字节比会把这些无关紧要的空白差异误报成 WA。C++ 在 Windows 下 stdout 带 \r，
//     统一先 .replace(/\r/g,'') 再切 token。
//  ② 随机对拍 ≤200 组：参照实现故意换机制 —— 对**每个目标格单独跑一次 BFS**
//     （O((nm)^2) 的"单源单目标"写法，只在 n,m<=12 的小棋盘上用），
//     与讲解代码"从起点一次扩散、读整张 dist 表"是两条独立路子。
//  ③ 极限规模 400x400：打印耗时与输出体量。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) {
  console.error(`找不到 ${EXE}\n请先按文件头注释用 g++ -static -O2 -std=c++14 编译。`);
  process.exit(1);
}

const norm = (s) => s.replace(/\r/g, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8', maxBuffer: 1 << 26 }));
const toks = (s) => s.trim().split(/\s+/).map(Number);
const rnd = (k) => Math.floor(Math.random() * k);

// ---- 参照实现：对每格单独 BFS（机制不同于讲解代码），返回 n x m 的答案矩阵 ----
const KDX = [1, 2, 2, 1, -1, -2, -2, -1];
const KDY = [2, 1, -1, -2, -2, -1, 1, 2];
function refOneTarget(n, m, sx, sy, tx, ty) {
  if (sx === tx && sy === ty) return 0;
  const vis = new Int8Array((n + 1) * (m + 1));
  const id = (x, y) => x * (m + 1) + y;
  let q = [[sx, sy]];
  vis[id(sx, sy)] = 1;
  let step = 0;
  while (q.length) {
    step++;
    const nq = [];
    for (const [x, y] of q)
      for (let k = 0; k < 8; k++) {
        const nx = x + KDX[k], ny = y + KDY[k];
        if (nx < 1 || nx > n || ny < 1 || ny > m || vis[id(nx, ny)]) continue;
        if (nx === tx && ny === ty) return step;   // 一层一层扩，第一次碰到目标就是最少步
        vis[id(nx, ny)] = 1;
        nq.push([nx, ny]);
      }
    q = nq;
  }
  return -1;
}
function refBoard(n, m, sx, sy) {
  const flat = [];
  for (let i = 1; i <= n; i++) for (let j = 1; j <= m; j++) flat.push(refOneTarget(n, m, sx, sy, i, j));
  return flat;
}

// ---- ① 官方样例 ----
{
  const got = toks(run('3 3 1 1\n'));
  const want = [0, 3, 2, 3, -1, 1, 2, 1, 4];   // 洛谷样例矩阵展平（行末空格不参与比较，见头部口径说明）
  const ok = got.length === want.length && got.every((v, i) => v === want[i]);
  console.log(`① 官方样例 3 3 1 1：期望 9 个数 [${want}]，实际 [${got}] ${ok ? '✅' : '❌'}`);
  if (!ok) process.exit(1);
}

// ---- ② 随机对拍（n,m<=12，含 1x1 / 1x2 退化棋盘），200 组 ----
{
  const R = Number(process.env.ROUNDS || 200);
  let bad = 0;
  for (let t = 0; t < R; t++) {
    const n = 1 + rnd(12), m = 1 + rnd(12);
    const x = 1 + rnd(n), y = 1 + rnd(m);
    const inp = `${n} ${m} ${x} ${y}\n`;
    const got = toks(run(inp));
    const want = refBoard(n, m, x, y);
    if (got.length !== want.length || got.some((v, i) => v !== want[i])) {
      bad++;
      if (bad === 1) console.log(`首个不一致 ${inp}\nsol=${got}\nref=${want}`);
    }
  }
  console.log(`② n,m<=12 随机 ${R} 组 vs "每格单独 BFS" 参照：不一致 ${bad} 组`);
  if (bad) process.exit(1);
}

// ---- ③ 极限规模 400x400 ----
{
  const inp = '400 400 1 1\n';
  const ts = [];
  let out = '';
  for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(inp); ts.push(Date.now() - t0); }
  const t = toks(out);
  const sum = t.reduce((s, v) => s + v, 0);
  console.log(`③ 400x400（题面上限）：耗时 ${ts.join('/')} ms，输出 ${t.length} 个整数 / ${out.length} 字节，全部值之和 = ${sum}`);
  console.log('   说明：400x400 = 160000 个数、约 0.7 MB 输出。printf 走 stdio 缓冲没问题；' +
              '若改用 cin/cout 必须 ios::sync_with_stdio(false)，否则 IO 开销会盖过算法本身。');
}
