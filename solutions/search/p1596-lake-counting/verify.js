// 【验算脚本，不是讲解代码】讲解代码一律看 solution.cpp（AGENTS.md §3：其他语言只允许当验算工具）。
// 复现命令（编译产物统一放 E:/ai/suanfa_study/_work/search-b/，不落在题目目录里）：
//   cd E:/ai/suanfa_study/oi-solutions/solutions/search/p1596-lake-counting
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o E:/ai/suanfa_study/_work/search-b/p1596-sol.exe
//   node verify.js                          # 默认用上面那个路径，也可 node verify.js 别的.exe
//
// 参照实现故意换机制：正解是"扫描 + 递归洪水填充（DFS）"；
// 参照是**并查集**：把八邻域里同为 'W' 的格子合并，最后数不同根的个数。
// 数根时只数"确实是水"的格子的根 —— 把干地也数进去是这类参照最经典的错法。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const DEFAULT_EXE = path.resolve(__dirname, '../../../../_work/search-b/p1596-sol.exe');
const EXE = process.argv[2] || DEFAULT_EXE;
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o ${DEFAULT_EXE}`);
  process.exit(1);
}
// C++ 在 Windows 下 stdout 换行是 \r\n，比对前先把 \r 去掉；再去掉结尾空白
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

// ---------- 参照实现：并查集合并八邻域 ----------
function refDSU(rows, n, m) {
  const id = (i, j) => i * m + j;
  const fa = new Array(n * m).fill(0).map((_, k) => k);
  const find = (x) => { while (fa[x] !== x) { fa[x] = fa[fa[x]]; x = fa[x]; } return x; };
  const uni = (a, b) => { a = find(a); b = find(b); if (a !== b) fa[a] = b; };
  for (let i = 0; i < n; i++)
    for (let j = 0; j < m; j++) {
      if (rows[i][j] !== 'W') continue;                     // 干地不参与合并
      for (let di = -1; di <= 1; di++)
        for (let dj = -1; dj <= 1; dj++) {
          if (di === 0 && dj === 0) continue;               // 八邻域 = 3x3 减自己
          const ni = i + di, nj = j + dj;
          if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
          if (rows[ni][nj] === 'W') uni(id(i, j), id(ni, nj));
        }
    }
  const roots = new Set();
  for (let i = 0; i < n; i++) for (let j = 0; j < m; j++) if (rows[i][j] === 'W') roots.add(find(id(i, j)));
  return roots.size;                                        // 只数水的根
}

const toInput = (rows) => `${rows.length} ${rows[0].length}\n` + rows.join('\n') + '\n';

// ---------- ① 官方样例逐字比对 ----------
const SAMPLE_ROWS = [
  'W........WW.',
  '.WWW.....WWW',
  '....WW...WW.',
  '.........WW.',
  '.........W..',
  '..W......W..',
  '.W.W.....WW.',
  'W.W.W.....W.',
  '.W.W......W.',
  '..W.......W.',
];
const SAMPLE_IN = toInput(SAMPLE_ROWS);
console.log('--- 官方样例（题面逐字版见 problem.txt）---');
{
  const got = run(SAMPLE_IN);
  const ok = got === '3';
  console.log(`样例 10x12 期望="3" 正解=${JSON.stringify(got)} 并查集参照=${refDSU(SAMPLE_ROWS, 10, 12)} ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) process.exitCode = 1;
}

// ---------- ② 随机对拍（<= 200 组）----------
console.log('--- 随机对拍：八方向 DFS 洪水填充 vs 并查集数根 ---');
let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 200);
for (let r = 0; r < ROUNDS; r++) {
  const n = 1 + rnd(12), m = 1 + rnd(12);
  const p = [0.1, 0.3, 0.5, 0.7, 0.95][rnd(5)];        // 水密度多样，包括几乎全水/几乎全干
  const rows = [];
  for (let i = 0; i < n; i++) {
    let s = '';
    for (let j = 0; j < m; j++) s += Math.random() < p ? 'W' : '.';
    rows.push(s);
  }
  const want = String(refDSU(rows, n, m));
  const got = run(toInput(rows));
  if (got !== want) {
    bad++;
    if (bad === 1) console.log(`首个不一致\n输入:\n${toInput(rows)}参照=${want} 正解=${got}`);
  }
}
console.log(`共 ${ROUNDS} 组（n,m<=12，水密度随机取 0.1/0.3/0.5/0.7/0.95），不一致 ${bad} 组`);
if (bad) process.exitCode = 1;

// ---------- 定向边界：斜角连通 vs 四方向连通 ----------
console.log('--- 定向边界（专门卡"八方向"这一条）---');
const FIXED = [
  { name: '对角两滴水（八方向=1 个塘，四方向=2 个）', rows: ['W.', '.W'] },
  { name: '斜着一条线 3x3', rows: ['W..', '.W.', '..W'] },
  { name: '全干地', rows: ['...', '...'] },
  { name: '全水 5x5（一个塘）', rows: ['WWWWW', 'WWWWW', 'WWWWW', 'WWWWW', 'WWWWW'] },
  { name: '1x1 单格水', rows: ['W'] },
  { name: '1x8 交替 W.W.W.W.', rows: ['W.W.W.W.'] },
];
for (const f of FIXED) {
  const n = f.rows.length, m = f.rows[0].length;
  console.log(`${f.name}: 正解=${run(toInput(f.rows))} 参照(八方向)=${refDSU(f.rows, n, m)}`);
}

// ---------- ③ 极限规模 100x100 ----------
console.log('--- 极限规模（题面上限 100x100）---');
for (const c of [
  { name: '全水 100x100（单个巨型水塘，DFS 递归最深）', gen: () => Array.from({ length: 100 }, () => 'W'.repeat(100)) },
  { name: '水密度 0.5 随机 100x100', gen: () => Array.from({ length: 100 }, () => { let s = ''; for (let j = 0; j < 100; j++) s += Math.random() < 0.5 ? 'W' : '.'; return s; }) },
  { name: '棋盘格 W/.（斜角也算相邻 => 整片水连成一个塘）', gen: () => Array.from({ length: 100 }, (_, i) => { let s = ''; for (let j = 0; j < 100; j++) s += (i + j) % 2 ? 'W' : '.'; return s; }) },
]) {
  const rows = c.gen();
  const t0 = Date.now();
  const got = run(toInput(rows));
  console.log(`${c.name}：水塘数 ${got}，耗时 ${Date.now() - t0} ms（含进程启动）`);
}
