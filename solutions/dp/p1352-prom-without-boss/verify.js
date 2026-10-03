// P1352 没有上司的舞会 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                                   —— JS 双实现随机对拍 500 组
//       node verify.js --build                           —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp
//       node verify.js --exe ./solution.exe --rounds=500  —— 用 C++ 程序当正解再对拍
const { execFileSync } = require('child_process');
const path = require('path');

const SAMPLE_IN = '7\n1\n1\n1\n1\n1\n1\n1\n1 3\n2 3\n6 4\n7 4\n4 5\n3 5\n';
const SAMPLE_OUT = '5';

// 正解：树形 DP（与 solution.cpp 同一算法，独立实现）
function solveTree(n, r, edges) {
  const child = Array.from({ length: n + 1 }, () => []);
  const hasP = new Array(n + 1).fill(false);
  for (const [L, K] of edges) { child[K].push(L); hasP[L] = true; }
  let root = 1;
  for (let i = 1; i <= n; i++) if (!hasP[i]) { root = i; break; }
  const f0 = new Array(n + 1).fill(0), f1 = new Array(n + 1).fill(0);
  const dfs = (u) => {
    f1[u] = r[u]; f0[u] = 0;
    for (const v of child[u]) { dfs(v); f1[u] += f0[v]; f0[u] += Math.max(f0[v], f1[v]); }
  };
  dfs(root);
  return Math.max(f0[root], f1[root]);
}

// 暴力：枚举 2^N 个出席集合，检查没有"父子同时出席"，取快乐值之和最大。与树形 DP 完全无关
function bruteSubset(n, r, edges) {
  const parentOf = new Array(n + 1).fill(0);
  for (const [L, K] of edges) parentOf[L] = K;
  let best = 0;                       // 空集：一个人都不请，快乐值 0
  for (let s = 0; s < (1 << n); s++) {
    let ok = true, sum = 0;
    for (let u = 1; u <= n && ok; u++) {
      if (!((s >> (u - 1)) & 1)) continue;
      sum += r[u];
      if (parentOf[u] && ((s >> (parentOf[u] - 1)) & 1)) ok = false;   // 父子都来了
    }
    if (ok && sum > best) best = sum;
  }
  return best;
}

// 错版：f[u][0] 写成 Σ f[v][0]（忘了"上司不来时孩子可以来"），答案会严重偏小
function solveWrong(n, r, edges) {
  const child = Array.from({ length: n + 1 }, () => []);
  const hasP = new Array(n + 1).fill(false);
  for (const [L, K] of edges) { child[K].push(L); hasP[L] = true; }
  let root = 1;
  for (let i = 1; i <= n; i++) if (!hasP[i]) { root = i; break; }
  const f0 = new Array(n + 1).fill(0), f1 = new Array(n + 1).fill(0);
  const dfs = (u) => {
    f1[u] = r[u]; f0[u] = 0;
    for (const v of child[u]) { dfs(v); f1[u] += f0[v]; f0[u] += f0[v]; }   // 错：漏了 max
  };
  dfs(root);
  return Math.max(f0[root], f1[root]);
}

let seed = 20261003;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

function genCase() {
  const n = ri(1, 11), r = [0], edges = [];
  for (let i = 1; i <= n; i++) r.push(ri(-5, 10));   // 含负数，压"全负"边界
  for (let v = 2; v <= n; v++) edges.push([v, ri(1, v - 1)]);   // 随机父节点，保证是一棵树
  return { n, r, edges };
}
const toInput = ({ n, r, edges }) =>
  n + '\n' + r.slice(1).join('\n') + '\n' + edges.map((e) => e[0] + ' ' + e[1]).join('\n') + (edges.length ? '\n' : '');
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

const ROUNDS = Number((process.argv.find((a) => /^--rounds=\d+$/.test(a)) || '').split('=')[1] || 500);
let bad = 0, wrong = 0;
for (let t = 0; t < ROUNDS; t++) {
  const c = genCase();
  const want = bruteSubset(c.n, c.r, c.edges);
  const got = exe ? strip(execFileSync(exe, [], { input: toInput(c) })) : String(solveTree(c.n, c.r, c.edges));
  if (String(want) !== got) { bad++; if (bad <= 3) console.log('MISMATCH ' + JSON.stringify(c) + ' brute=' + want + ' got=' + got); }
  if (solveWrong(c.n, c.r, c.edges) !== want) wrong++;
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 树形 DP') + ' vs 2^N 枚举出席集合暴力 ---');
console.log('共 ' + ROUNDS + ' 组（N<=11, r_i 含负数），不一致 ' + bad + ' 组');
console.log('其中 ' + wrong + ' 组里"f[u][0] 漏写 max"的错版结果与正解不同 —— 它假设上司不来时孩子也不来');
