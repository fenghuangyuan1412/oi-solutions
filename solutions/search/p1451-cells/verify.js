// 【验算脚本，不是讲解代码】讲解代码一律看 solution.cpp（AGENTS.md §3：其他语言只允许当验算工具）。
// 复现命令（编译产物统一放 E:/ai/suanfa_study/_work/search-b/，不落在题目目录里）：
//   cd E:/ai/suanfa_study/oi-solutions/solutions/search/p1451-cells
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o E:/ai/suanfa_study/_work/search-b/p1451-sol.exe
//   node verify.js                          # 默认用上面那个路径，也可 node verify.js 别的.exe
//
// 参照实现故意换机制：正解是"扫描 + 递归洪水填充（DFS）"；
// 参照是**并查集**：把四邻域里同为细胞数字（非 '0'）的格子合并，最后数不同根的个数。
// 数根时只数"确实是细胞"的格子 —— 把 '0' 也数进去是这类参照最经典的错法。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const DEFAULT_EXE = path.resolve(__dirname, '../../../../_work/search-b/p1451-sol.exe');
const EXE = process.argv[2] || DEFAULT_EXE;
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o ${DEFAULT_EXE}`);
  process.exit(1);
}
// C++ 在 Windows 下 stdout 换行是 \r\n，比对前先把 \r 去掉；再去掉结尾空白
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

// ---------- 参照实现：并查集合并四邻域 ----------
function refDSU(rows, n, m) {
  const id = (i, j) => i * m + j;
  const fa = new Array(n * m).fill(0).map((_, k) => k);
  const find = (x) => { while (fa[x] !== x) { fa[x] = fa[fa[x]]; x = fa[x]; } return x; };
  const uni = (a, b) => { a = find(a); b = find(b); if (a !== b) fa[a] = b; };
  const D4 = [[-1, 0], [1, 0], [0, -1], [0, 1]];
  for (let i = 0; i < n; i++)
    for (let j = 0; j < m; j++) {
      if (rows[i][j] === '0') continue;                    // '0' 不是细胞，不参与合并
      for (const [di, dj] of D4) {
        const ni = i + di, nj = j + dj;
        if (ni < 0 || ni >= n || nj < 0 || nj >= m) continue;
        if (rows[ni][nj] !== '0') uni(id(i, j), id(ni, nj));   // 只要"是细胞数字"就算同一细胞，数字不必相同
      }
    }
  const roots = new Set();
  for (let i = 0; i < n; i++) for (let j = 0; j < m; j++) if (rows[i][j] !== '0') roots.add(find(id(i, j)));
  return roots.size;                                       // 只数细胞的根
}

const toInput = (rows) => `${rows.length} ${rows[0].length}\n` + rows.join('\n') + '\n';

// ---------- ① 官方样例逐字比对 ----------
const SAMPLE_ROWS = ['0234500067', '1034560500', '2045600671', '0000000089'];
const SAMPLE_IN = toInput(SAMPLE_ROWS);
console.log('--- 官方样例（题面逐字版见 problem.txt）---');
{
  const got = run(SAMPLE_IN);
  const ok = got === '4';
  console.log(`样例 4x10 期望="4" 正解=${JSON.stringify(got)} 并查集参照=${refDSU(SAMPLE_ROWS, 4, 10)} ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) process.exitCode = 1;
}

// ---------- ② 随机对拍（<= 200 组）----------
console.log('--- 随机对拍：四方向 DFS 洪水填充 vs 并查集数根 ---');
let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 200);
for (let r = 0; r < ROUNDS; r++) {
  const n = 1 + rnd(12), m = 1 + rnd(12);
  const p0 = [0.1, 0.25, 0.5, 0.75, 0.9][rnd(5)];         // '0' 的比例，覆盖稀疏/稠密
  const rows = [];
  for (let i = 0; i < n; i++) {
    let s = '';
    for (let j = 0; j < m; j++) s += Math.random() < p0 ? '0' : String(1 + rnd(9));
    rows.push(s);
  }
  const want = String(refDSU(rows, n, m));
  const got = run(toInput(rows));
  if (got !== want) {
    bad++;
    if (bad === 1) console.log(`首个不一致\n输入:\n${toInput(rows)}参照=${want} 正解=${got}`);
  }
}
console.log(`共 ${ROUNDS} 组（n,m<=12，'0' 比例随机取 0.1/0.25/0.5/0.75/0.9），不一致 ${bad} 组`);
if (bad) process.exitCode = 1;

// ---------- 定向边界 ----------
console.log('--- 定向边界 ---');
const FIXED = [
  { name: '斜角相邻的两个数字（四方向=2 个细胞）', rows: ['10', '01'] },
  { name: '相邻但数字不同（1 与 9 贴边）仍算同一细胞', rows: ['19'] },
  { name: '一整行全 0', rows: ['0000'] },
  { name: '全 9 的 4x4（一个细胞）', rows: ['9999', '9999', '9999', '9999'] },
  { name: '1x1 单格 0', rows: ['0'] },
  { name: '十字形（中间连通）', rows: ['010', '111', '010'] },
  { name: 'Z 字形：只有斜角相连', rows: ['110', '001', '001'] },
];
for (const f of FIXED) {
  const n = f.rows.length, m = f.rows[0].length;
  console.log(`${f.name}: 正解=${run(toInput(f.rows))} 参照=${refDSU(f.rows, n, m)}`);
}

// ---------- ③ 极限规模 100x100 ----------
console.log('--- 极限规模（题面上限 100x100）---');
for (const c of [
  { name: "全 1（单个巨型细胞，递归最深约 1e4 层）", gen: () => Array.from({ length: 100 }, () => '1'.repeat(100)) },
  { name: "随机数字（'0' 比例 0.5）", gen: () => Array.from({ length: 100 }, () => { let s = ''; for (let j = 0; j < 100; j++) s += Math.random() < 0.5 ? '0' : String(1 + rnd(9)); return s; }) },
  { name: "竖条纹 0/1 交替（50 条宽 1 的细胞带）", gen: () => Array.from({ length: 100 }, () => { let s = ''; for (let j = 0; j < 100; j++) s += j % 2 ? '0' : '1'; return s; }) },
]) {
  const rows = c.gen();
  const t0 = Date.now();
  const got = run(toInput(rows));
  console.log(`${c.name}：细胞数 ${got}，耗时 ${Date.now() - t0} ms（含进程启动）`);
}
