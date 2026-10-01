// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 复现命令（在本目录）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js
// 想把 exe 挪出题目目录（*.exe 已在 .gitignore 里）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o ../../../_work/search-c/sol.exe
//   node verify.js ../../../_work/search-c/sol.exe
//
// 三件事：
//  ① 官方样例逐字比对（输出只有一个整数，先 .replace(/\r/g,'') 去掉 Windows 的 \r）。
//  ② 随机对拍 ≤200 组：参照实现用 **Floyd 最小按键数闭包**（三层循环逐点松弛，
//     dist[i][j] = i 楼到 j 楼最少按键数），与讲解代码"从 A 出发的单层 BFS"是完全
//     不同的机制；Floyd 是 O(N^3)，只在 N<=60 的小数据上用。
//  ③ 极限规模 N=200 随机 K，打印耗时；另附 A==B 与不可达两个定向边界用例。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) {
  console.error(`找不到 ${EXE}\n请先按文件头注释用 g++ -static -O2 -std=c++14 编译。`);
  process.exit(1);
}

const norm = (s) => s.replace(/\r/g, '').trim();
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

// ---- 参照实现：Floyd 求任意两楼间最少按键数 ----
function refFloyd(n, k, a, b) {
  const INF = 1e9;
  const d = Array.from({ length: n + 1 }, () => new Array(n + 1).fill(INF));
  for (let i = 1; i <= n; i++) {
    if (i + k[i] <= n) d[i][i + k[i]] = 1;         // 按"上"这条有向边
    if (i - k[i] >= 1) d[i][i - k[i]] = 1;         // 按"下"
  }
  // 注意顺序：k[i]=0 时上面两条都是自环 d[i][i]=1，必须最后再统一压回 0，
  // 否则 A==B 的用例参照会错报 1（这是写参照实现时真实踩到的坑，见 README 易错点）。
  for (let i = 1; i <= n; i++) d[i][i] = 0;        // 同一层 0 次按键（覆盖 A==B）
  for (let t = 1; t <= n; t++)
    for (let i = 1; i <= n; i++)
      for (let j = 1; j <= n; j++)
        if (d[i][t] + d[t][j] < d[i][j]) d[i][j] = d[i][t] + d[t][j];
  return d[a][b] >= INF ? -1 : d[a][b];
}

// ---- ① 官方样例 ----
{
  const got = run('5 1 5\n3 3 1 2 5\n');
  console.log(`① 官方样例：期望 3，实际 ${got} ${got === '3' ? '✅' : '❌'}`);
  if (got !== '3') process.exit(1);
}

// ---- ② 随机对拍，N<=60，200 组 ----
{
  const R = Number(process.env.ROUNDS || 200);
  let bad = 0;
  for (let t = 0; t < R; t++) {
    const n = 1 + rnd(60);                          // 1..60，含单楼层退化
    const k = [0, ...Array.from({ length: n }, () => rnd(n + 1))];  // 0<=K[i]<=N，故意含 0
    const a = 1 + rnd(n), b = 1 + rnd(n);
    const inp = `${n} ${a} ${b}\n${k.slice(1).join(' ')}\n`;
    const got = run(inp);
    const want = String(refFloyd(n, k, a, b));
    if (got !== want) {
      bad++;
      if (bad === 1) console.log(`首个不一致:\n${inp}sol=${got} ref=${want}`);
    }
  }
  console.log(`② N<=60、K 含 0 随机 ${R} 组 vs Floyd 参照：不一致 ${bad} 组`);
  if (bad) process.exit(1);
}

// ---- ③ 定向边界 + 极限规模 ----
{
  // A==B：题面问"最少按键次数"，答案是 0，不是 -1
  const same = run('5 3 3\n3 3 1 2 5\n');
  console.log(`③ A==B 定向用例：期望 0，实际 ${same} ${same === '0' ? '✅' : '❌'}`);
  if (same !== '0') process.exit(1);
  // 不可达：2 层楼 K=0，1 楼去不了 2 楼
  const dead = run('2 1 2\n0 0\n');
  console.log(`③ 不可达定向用例：期望 -1，实际 ${dead} ${dead === '-1' ? '✅' : '❌'}`);
  if (dead !== '-1') process.exit(1);

  const n = 200;
  // 定向可核算的极限数据：K 全 1 时 1 楼到 200 楼必须按 199 次"上"
  const all1 = '200 1 200\n' + Array(200).fill(1).join(' ') + '\n';
  // 随机 K（含 0，可能不可达），考察真实分布下的行为。
  // 用固定种子的 xorshift32（Math.imul 保证 32 位整型精确）：答案与耗时可复现。
  let seed = 20230801;
  const rndSeeded = (mod) => {
    seed ^= seed << 13; seed |= 0;
    seed ^= seed >>> 17;
    seed ^= seed << 5;  seed |= 0;
    return Math.abs(seed) % mod;
  };
  const k = Array.from({ length: n }, () => rndSeeded(n + 1)).join(' ');
  const inp = `${n} 1 ${n}\n${k}\n`;
  let ts = [];
  let out = '';
  for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(all1); ts.push(Date.now() - t0); }
  console.log(`③ N=200、K 全 1（1 楼到 200 楼，手算应为 199）：答案 ${out} ${out === '199' ? '✅' : '❌'}，耗时 ${ts.join('/')} ms`);
  if (out !== '199') process.exit(1);
  ts = [];
  for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(inp); ts.push(Date.now() - t0); }
  console.log(`③ N=200（题面上限）随机 K：答案 ${out}，耗时 ${ts.join('/')} ms（瓶颈基本是进程启动 + printf，算法本身只碰 200 个状态）`);
}
