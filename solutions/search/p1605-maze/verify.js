// 【验算脚本，不是讲解代码】讲解代码一律看 solution.cpp（AGENTS.md §3：其他语言只允许当验算工具）。
// 复现命令（编译产物统一放 E:/ai/suanfa_study/_work/search-b/，不落在题目目录里）：
//   cd E:/ai/suanfa_study/oi-solutions/solutions/search/p1605-maze
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o E:/ai/suanfa_study/_work/search-b/p1605-sol.exe
//   node verify.js                                   # 默认就用上面那个路径，也可 node verify.js 别的.exe
//
// 参照实现故意换机制：正解是"网格 vis 数组 + 回溯撤销"的 DFS；
// 参照是"状态压缩"，把（已访问集合 bitmask，当前格）看成一个状态，按集合大小一层层往前推，
// 全程不撤销、不递归回溯。两条路算出的方案数必须逐组相同。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const DEFAULT_EXE = path.resolve(__dirname, '../../../../_work/search-b/p1605-sol.exe');
const EXE = process.argv[2] || DEFAULT_EXE;
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o ${DEFAULT_EXE}`);
  process.exit(1);
}
// C++ 在 Windows 下 stdout 换行是 \r\n，比对前先把 \r 去掉；再去掉结尾空白
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

// 把输入串原样解析一遍，保证参照读到的和 C++ 读到的是同一份数据
function parse(inp) {
  const a = inp.trim().split(/\s+/).map(Number);
  const [n, m, t] = a, [sx, sy, fx, fy] = a.slice(3);
  const blockedSet = new Set();
  for (let i = 0; i < t; i++) {
    const x = a[7 + 2 * i], y = a[8 + 2 * i];   // 前 7 个数是 N M T SX SY FX FY，障碍从下标 7 开始
    blockedSet.add((x - 1) * m + (y - 1));
  }
  return { n, m, t, sx, sy, fx, fy, blockedSet };
}

// ---------- 参照实现：状态压缩（bitmask + 当前格）逐层扩展 ----------
const DIRS = [[1, 0], [-1, 0], [0, 1], [0, -1]];
function refPaths(inp) {
  const { n, m, sx, sy, fx, fy, blockedSet } = parse(inp);
  const idx = (r, c) => r * m + c;            // 0-based 格子编号，直接当 bitmask 的第几位
  const start = idx(sx - 1, sy - 1), target = idx(fx - 1, fy - 1);
  const cells = n * m;
  const key = (mask, pos) => mask * cells + pos;
  const dp = new Map([[key(1 << start, start), 1]]);
  const q = [[1 << start, start]];
  let ans = 0;
  while (q.length) {
    const [mask, pos] = q.shift();
    const ways = dp.get(key(mask, pos));
    if (pos === target) { ans += ways; continue; }   // 到终点即结束，和正解一样"不借道终点"
    const r = Math.floor(pos / m), c = pos % m;
    for (const [dr, dc] of DIRS) {
      const nr = r + dr, nc = c + dc;
      if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
      const nid = idx(nr, nc);
      if (blockedSet.has(nid)) continue;
      const bit = 1 << nid;
      if (mask & bit) continue;                       // "每个方格最多经过一次" => 集合里已有就不再走
      const nmask = mask | bit, k2 = key(nmask, nid);
      if (dp.has(k2)) dp.set(k2, dp.get(k2) + ways);
      else { dp.set(k2, ways); q.push([nmask, nid]); }
    }
  }
  return ans;
}

// ---------- ① 官方样例逐字比对 ----------
const SAMPLES = [
  { in: '2 2 1\n1 1 2 2\n1 2\n', out: '1' },
];
console.log('--- 官方样例（题面逐字版见 problem.txt）---');
for (const s of SAMPLES) {
  const got = run(s.in);
  const ok = got === s.out;
  console.log(`样例 ${JSON.stringify(s.in.split('\n')[0])} 期望=${JSON.stringify(s.out)} 正解=${JSON.stringify(got)} 参照=${refPaths(s.in)} ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) process.exitCode = 1;
}

// ---------- ② 随机对拍（<= 200 组）----------
function genCase() {
  // 题面保证 T >= 1，所以至少要有 3 个格子（起点、终点、一个障碍）
  let n = 1 + rnd(5), m = 1 + rnd(5);
  while (n * m < 3) { n = 1 + rnd(5); m = 1 + rnd(5); }
  const cells = [];
  for (let i = 1; i <= n; i++) for (let j = 1; j <= m; j++) cells.push([i, j]);
  const pick = () => cells.splice(rnd(cells.length), 1)[0];
  const [sx, sy] = pick(), [fx, fy] = pick();
  const t = 1 + rnd(Math.min(10, cells.length));      // 剩下的格子随便拿几个当障碍
  const obs = [];
  for (let i = 0; i < t; i++) obs.push(pick());
  let inp = `${n} ${m} ${t}\n${sx} ${sy} ${fx} ${fy}\n`;
  for (const [x, y] of obs) inp += `${x} ${y}\n`;
  return inp;
}

console.log('--- 随机对拍：vis 数组回溯 DFS vs 状态压缩逐层扩展 ---');
let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 200);
for (let i = 0; i < ROUNDS; i++) {
  const inp = genCase();
  const want = String(refPaths(inp));
  const got = run(inp);
  if (got !== want) {
    bad++;
    if (bad === 1) console.log(`首个不一致\n输入:\n${inp}参照=${want} 正解=${got}`);
  }
}
console.log(`共 ${ROUNDS} 组（n,m<=5，随机起终点与障碍），不一致 ${bad} 组`);
if (bad) process.exitCode = 1;

// ---------- 定向边界 ----------
console.log('--- 定向边界 ---');
const FIXED = [
  { name: '起点即终点', inp: '2 2 1\n1 1 1 1\n2 2\n' },
  { name: '3x3 只挡中心一格，角到角', inp: '3 3 1\n1 1 3 3\n2 2\n' },
  { name: '一整列障碍把出口堵死', inp: '3 3 3\n1 1 3 3\n1 2\n2 2\n3 2\n' },
  { name: '1x3 直线被中间障碍切断', inp: '1 3 1\n1 1 1 3\n1 2\n' },
  { name: '1x4 直线，障碍在终点前一格', inp: '1 4 1\n1 1 1 4\n1 3\n' },
];
for (const f of FIXED) console.log(`${f.name}: 正解=${run(f.inp)} 参照=${refPaths(f.inp)}`);

// ---------- ③ 极限规模：5x5 空格迷宫 T=1，路径计数爆炸 ----------
console.log('--- 极限规模（题面上限 5x5，T=1）---');
for (const s of [
  { name: '角到角 (1,1)->(5,5)，障碍 (3,3)', inp: '5 5 1\n1 1 5 5\n3 3\n' },
  { name: '中心 (3,3)->(3,2)，障碍 (1,1)，分支最茂密', inp: '5 5 1\n3 3 3 2\n1 1\n' },
]) {
  const t0 = Date.now();
  const got = run(s.inp);
  console.log(`${s.name}：方案数 ${got}，耗时 ${Date.now() - t0} ms（含进程启动）`);
}
