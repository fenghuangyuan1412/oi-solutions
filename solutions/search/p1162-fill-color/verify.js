// 【验算脚本，不是讲解代码】讲解代码一律看 solution.cpp（AGENTS.md §3：其他语言只允许当验算工具）。
// 复现命令（编译产物统一放 E:/ai/suanfa_study/_work/search-b/，不落在题目目录里）：
//   cd E:/ai/suanfa_study/oi-solutions/solutions/search/p1162-fill-color
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o E:/ai/suanfa_study/_work/search-b/p1162-sol.exe
//   node verify.js                          # 默认用上面那个路径，也可 node verify.js 别的.exe
//
// 参照实现故意换机制：正解是"从四条边界反向洪泛一次，标出所有圈外 0"；
// 参照是**照题面定义逐字硬做**：对每一个 0 格单独 BFS，问"它能不能只经过 0 走到边界"，
// 走不到就涂 2。朴素 O(n^4)，但和正解的思路完全无关，适合小网格互相校验。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const DEFAULT_EXE = path.resolve(__dirname, '../../../../_work/search-b/p1162-sol.exe');
const EXE = process.argv[2] || DEFAULT_EXE;
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o ${DEFAULT_EXE}`);
  process.exit(1);
}
// C++ 在 Windows 下 stdout 换行是 \r\n，比对前先把 \r 去掉；再去掉结尾空白
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

// ---------- 参照实现：逐格判定"能否到达边界" ----------
function refFill(g, n) {
  const canReachBorder = (si, sj) => {
    const seen = Array.from({ length: n }, () => new Array(n).fill(false));
    const q = [[si, sj]];
    seen[si][sj] = true;
    while (q.length) {
      const [x, y] = q.shift();
      if (x === 0 || y === 0 || x === n - 1 || y === n - 1) return true;   // 摸到边界 => 圈外
      for (const [dx, dy] of [[-1, 0], [1, 0], [0, -1], [0, 1]]) {
        const nx = x + dx, ny = y + dy;
        if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
        if (seen[nx][ny] || g[nx][ny] !== 0) continue;                     // 只能经过 0
        seen[nx][ny] = true;
        q.push([nx, ny]);
      }
    }
    return false;                                                          // 走遍了也到不了边界 => 圈内
  };
  const out = [];
  for (let i = 0; i < n; i++) {
    const row = [];
    for (let j = 0; j < n; j++) {
      if (g[i][j] === 0 && !canReachBorder(i, j)) row.push(2);             // 题面定义：到不了边界就是圈内
      else row.push(g[i][j]);
    }
    out.push(row.join(' '));
  }
  return out.join('\n');
}

const toInput = (g) => `${g.length}\n` + g.map((r) => r.join(' ')).join('\n') + '\n';

// ---------- ① 官方样例逐字比对 ----------
const SAMPLE = [
  [0, 0, 0, 0, 0, 0],
  [0, 0, 1, 1, 1, 1],
  [0, 1, 1, 0, 0, 1],
  [1, 1, 0, 0, 0, 1],
  [1, 0, 0, 0, 0, 1],
  [1, 1, 1, 1, 1, 1],
];
const SAMPLE_OUT = [
  '0 0 0 0 0 0',
  '0 0 1 1 1 1',
  '0 1 1 2 2 1',
  '1 1 2 2 2 1',
  '1 2 2 2 2 1',
  '1 1 1 1 1 1',
].join('\n');
console.log('--- 官方样例（题面逐字版见 problem.txt）---');
{
  const got = run(toInput(SAMPLE));
  const ok = got === SAMPLE_OUT;
  console.log(`样例 n=6 期望（逐字）:\n${SAMPLE_OUT}`);
  console.log(`实际:\n${got}`);
  console.log(`朴素参照（逐格 BFS）:\n${refFill(SAMPLE, 6)}`);
  console.log(ok ? '逐字一致 OK' : '不一致 FAIL');
  if (!ok) process.exitCode = 1;
}

// ---------- ② 随机对拍（<= 200 组）----------
// 造数据：随机矩形闭合圈（可能贴边）+ 偶尔往圈内外撒几个额外的 1；再混一批纯随机 0/1 图。
function genCase() {
  const n = 4 + rnd(6);                                   // 4..9，参照实现是 O(n^4)，别太大
  const g = Array.from({ length: n }, () => new Array(n).fill(0));
  if (Math.random() < 0.75) {
    const r1 = rnd(n - 2), r2 = r1 + 2 + rnd(n - 2 - r1);
    const c1 = rnd(n - 2), c2 = c1 + 2 + rnd(n - 2 - c1);
    for (let j = c1; j <= c2; j++) { g[r1][j] = 1; g[r2][j] = 1; }
    for (let i = r1; i <= r2; i++) { g[i][c1] = 1; g[i][c2] = 1; }
    if (Math.random() < 0.4) {                            // 撒几个无关的 1
      const k = 1 + rnd(3);
      for (let t = 0; t < k; t++) g[rnd(n)][rnd(n)] = 1;
    }
  } else {
    for (let i = 0; i < n; i++) for (let j = 0; j < n; j++) g[i][j] = Math.random() < 0.45 ? 1 : 0;
  }
  return { g, inp: toInput(g), n };
}

console.log('--- 随机对拍：边界反向洪泛 vs 逐格 BFS 判"能否到边界" ---');
let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 200);
for (let r = 0; r < ROUNDS; r++) {
  const c = genCase();
  const want = refFill(c.g, c.n);
  const got = run(c.inp);
  if (got !== want) {
    bad++;
    if (bad === 1) console.log(`首个不一致\n输入:\n${c.inp}参照:\n${want}\n正解:\n${got}`);
  }
}
console.log(`共 ${ROUNDS} 组（n=4..9，随机闭合圈 + 纯随机图混测），不一致 ${bad} 组`);
if (bad) process.exitCode = 1;

// ---------- 定向边界 ----------
console.log('--- 定向边界 ---');
const FIXED = [
  { name: '圈贴满四边（圈外没有 0）', g: [[1, 1, 1], [1, 0, 1], [1, 1, 1]] },
  { name: '圈贴着上边（角上是 1，从单点 (0,0) 出发就废）', g: [[1, 1, 1, 1], [1, 0, 0, 1], [0, 0, 0, 1], [1, 1, 1, 1]] },
  { name: '全 0（没有圈，整片都是圈外）', g: [[0, 0], [0, 0]] },
  { name: '最小合法图：3x3 圈 + 中心 1 格内空', g: [[0, 0, 0], [0, 1, 0], [0, 0, 0]] },
];
for (const f of FIXED) {
  const n = f.g.length;
  console.log(`${f.name}:\n正解:\n${run(toInput(f.g))}\n参照:\n${refFill(f.g, n)}`);
}

// ---------- ③ 极限规模 n=30 ----------
console.log('--- 极限规模（题面上限 n=30）---');
{
  // 最大面积情形：圈占第 1 行、第 28 行、第 1 列、第 28 列 => 圈内是 26x26 = 676 格，全要涂成 2；
  // 圈外只有最外那一圈（第 0 行/列、第 29 行/列）的 0，反向洪泛只需淹这 116 格
  const n = 30;
  const g = Array.from({ length: n }, () => new Array(n).fill(0));
  for (let j = 1; j < n - 1; j++) { g[1][j] = 1; g[n - 2][j] = 1; }
  for (let i = 1; i <= n - 2; i++) { g[i][1] = 1; g[i][n - 2] = 1; }
  const t0 = Date.now();
  const out = run(toInput(g));
  const inside = (out.match(/2/g) || []).length;
  console.log(`n=30 大圈（圈占第 1/28 行列，圈内 26x26）：涂了 ${inside} 个 2，耗时 ${Date.now() - t0} ms（含进程启动）`);
}
{
  // 题面上限内的同图对照
  const n = 30;
  const g = Array.from({ length: n }, () => { const r = []; for (let j = 0; j < n; j++) r.push(Math.random() < 0.4 ? 1 : 0); return r; });
  const inp = toInput(g);
  let t0 = Date.now();
  const a = run(inp), ac = (a.match(/2/g) || []).length, at = Date.now() - t0;
  t0 = Date.now();
  const bc = (refFill(g, n).match(/2/g) || []).length, bt = Date.now() - t0;
  console.log(`n=30 同一份随机 0/1 图：正解涂 ${ac} 个 2 / ${at} ms；朴素参照涂 ${bc} 个 2 / ${bt} ms${ac === bc ? '（两者一致）' : '（!! 不一致）'}`);
  if (ac !== bc) process.exitCode = 1;
}
{
  // 只看朴素参照自身的复杂度增长曲线。注意：solution.cpp 的数组按题面上限开成 MAXN=35，
  // n>30 属于越界写内存（实测输出会整体错位/清空），所以下面两组只跑 JS 参照，不与 C++ 对比。
  console.log('（以下 n>30 超出题面上限，只跑 JS 参照，用来观察 O(n^4) 增长）');
  for (const n of [60, 120, 240]) {
    const g = Array.from({ length: n }, () => { const r = []; for (let j = 0; j < n; j++) r.push(Math.random() < 0.4 ? 1 : 0); return r; });
    const t0 = Date.now();
    const bc = (refFill(g, n).match(/2/g) || []).length;
    console.log(`朴素参照 n=${n}：涂 ${bc} 个 2，耗时 ${Date.now() - t0} ms`);
  }
}
