#!/usr/bin/env node
/*
 * P11855 [CSP-J 2022 山东] 部署 —— 随机对拍脚本
 *
 * 用法：
 *   node verify.js                  # 默认 500 组随机对拍（用户要求：少生成数据、快一点）
 *   node verify.js --rounds=2000    # 覆盖轮数
 *   node verify.js --exe=./solution.exe
 *                                   # 额外把编译好的 C++ 正解也拉进对拍（沙箱里可能 EBUSY，见 README）
 *
 * 三个实现（互相独立）：
 *   solveFast —— 正解：离线差分 + 前缀和（复刻 solution.cpp 的算法）
 *   solveBrute—— 暴力：逐条操作真的去改每个点的值（操作 1 用 DFS 扫子树，操作 2 直接改邻域）
 *   solveWrong—— 错版：操作 2 只加"自己 + 儿子"，漏掉"父亲"那一项
 */
'use strict';
const fs = require('fs');
const path = require('path');
const { spawnSync } = require('child_process');

/* ------------------------------------------------------------------ *
 * 通用解析：把文本解析成 { n, a, edges, ops, queries }
 * ------------------------------------------------------------------ */
function parse(text) {
  const t = text.trim().split(/\s+/).map(Number);
  let p = 0;
  const n = t[p++];
  const a = [0];
  for (let i = 1; i <= n; ++i) a.push(t[p++]);
  const adj = Array.from({ length: n + 1 }, () => []);
  for (let i = 0; i < n - 1; ++i) {
    const u = t[p++], v = t[p++];
    adj[u].push(v); adj[v].push(u);
  }
  const m = t[p++];
  const ops = [];
  for (let i = 0; i < m; ++i) ops.push([t[p++], t[p++], t[p++]]);
  const q = t[p++];
  const queries = [];
  for (let i = 0; i < q; ++i) queries.push(t[p++]);
  return { n, a, adj, ops, queries };
}

/* ------------------------------------------------------------------ *
 * 正解：离线差分 + 前缀和
 * ------------------------------------------------------------------ */
function solveFast(text) {
  const { n, a, adj, ops, queries } = parse(text);
  const parent = new Array(n + 1).fill(0);
  const order = [];
  // BFS
  const bfsQ = [1];
  parent[1] = 0;
  const seen = new Array(n + 1).fill(false);
  seen[1] = true;
  for (let h = 0; h < bfsQ.length; ++h) {
    const u = bfsQ[h];
    order.push(u);
    for (const v of adj[u]) {
      if (v === parent[u] || seen[v]) continue;
      seen[v] = true; parent[v] = u; bfsQ.push(v);
    }
  }
  const raw1 = new Array(n + 1).fill(0);
  const A = new Array(n + 1).fill(0);
  for (const [op, x, y] of ops) {
    if (op === 1) raw1[x] += y; else A[x] += y;
  }
  const delta1 = new Array(n + 1).fill(0);
  for (const u of order) delta1[u] = raw1[u] + (parent[u] ? delta1[parent[u]] : 0);
  const childSum = new Array(n + 1).fill(0);
  for (let v = 1; v <= n; ++v) if (parent[v]) childSum[parent[v]] += A[v];
  return queries.map((u) =>
    a[u] + delta1[u] + A[u] + (parent[u] ? A[parent[u]] : 0) + childSum[u]
  );
}

/* ------------------------------------------------------------------ *
 * 暴力：老老实实逐条操作去改值
 * ------------------------------------------------------------------ */
function solveBrute(text) {
  const { n, a, adj, ops, queries } = parse(text);
  const val = a.slice();
  // 求父亲
  const parent = new Array(n + 1).fill(0);
  const seen = new Array(n + 1).fill(false);
  const bfsQ = [1]; seen[1] = true;
  for (let h = 0; h < bfsQ.length; ++h) {
    const u = bfsQ[h];
    for (const v of adj[u]) { if (!seen[v]) { seen[v] = true; parent[v] = u; bfsQ.push(v); } }
  }
  // 子树 DFS（迭代）
  function addSubtree(x, y) {
    const st = [x];
    while (st.length) {
      const u = st.pop();
      val[u] += y;
      for (const v of adj[u]) if (v !== parent[u]) st.push(v);
    }
  }
  for (const [op, x, y] of ops) {
    if (op === 1) {
      addSubtree(x, y);
    } else {
      val[x] += y;                                   // 自己
      if (parent[x]) val[parent[x]] += y;            // 父亲
      for (const v of adj[x]) if (v !== parent[x]) val[v] += y;  // 儿子
    }
  }
  return queries.map((u) => val[u]);
}

/* ------------------------------------------------------------------ *
 * 错版：操作 2 漏掉"父亲"那一项
 * ------------------------------------------------------------------ */
function solveWrong(text) {
  const { n, a, adj, ops, queries } = parse(text);
  const parent = new Array(n + 1).fill(0);
  const seen = new Array(n + 1).fill(false);
  const bfsQ = [1]; seen[1] = true;
  for (let h = 0; h < bfsQ.length; ++h) {
    const u = bfsQ[h];
    for (const v of adj[u]) { if (!seen[v]) { seen[v] = true; parent[v] = u; bfsQ.push(v); } }
  }
  const val = a.slice();
  function addSubtree(x, y) {
    const st = [x];
    while (st.length) {
      const u = st.pop();
      val[u] += y;
      for (const v of adj[u]) if (v !== parent[u]) st.push(v);
    }
  }
  for (const [op, x, y] of ops) {
    if (op === 1) {
      addSubtree(x, y);
    } else {
      val[x] += y;                                                // 自己
      for (const v of adj[x]) if (v !== parent[x]) val[v] += y;   // 儿子（漏了父亲！）
    }
  }
  return queries.map((u) => val[u]);
}

/* ------------------------------------------------------------------ *
 * 随机数据生成器：随机树 + 随机操作 + 询问所有点
 * ------------------------------------------------------------------ */
function rndInt(lo, hi) { return lo + Math.floor(Math.random() * (hi - lo + 1)); }

function genRandom() {
  const n = rndInt(1, 14);
  const a = [];
  for (let i = 1; i <= n; ++i) a.push(rndInt(1, 20));
  // 生成随机树：节点 i 的父亲随机取 [1, i-1]
  const edges = [];
  const par = new Array(n + 1).fill(0);
  for (let i = 2; i <= n; ++i) {
    const p = rndInt(1, i - 1);
    par[i] = p;
    edges.push(rndInt(0, 1) ? [p, i] : [i, p]);   // 随机方向，测无向读入
  }
  // 随机打乱边序
  for (let i = edges.length - 1; i > 0; --i) {
    const j = rndInt(0, i);
    [edges[i], edges[j]] = [edges[j], edges[i]];
  }
  const m = rndInt(1, 12);
  const ops = [];
  for (let i = 0; i < m; ++i) ops.push([rndInt(1, 2), rndInt(1, n), rndInt(1, 10)]);
  // 询问：全部点（保证 q = n，覆盖面最大）
  const qs = [];
  for (let i = 1; i <= n; ++i) qs.push(i);

  let s = n + '\n' + a.join(' ') + '\n';
  for (const [u, v] of edges) s += u + ' ' + v + '\n';
  s += m + '\n';
  for (const [o, x, y] of ops) s += o + ' ' + x + ' ' + y + '\n';
  s += qs.length + '\n' + qs.join('\n') + '\n';
  return s;
}

/* ------------------------------------------------------------------ *
 * 定点用例
 * ------------------------------------------------------------------ */
const CASES = [
  { name: '样例1', text: '5\n1 2 3 4 5\n1 2\n1 3\n2 4\n3 5\n4\n1 1 2\n2 2 3\n1 3 3\n2 5 1\n4\n1\n2\n3\n4\n', exp: [6, 7, 9, 9] },
  { name: '样例2', text: '4\n1 1 1 1\n1 2\n1 3\n1 4\n1\n1 1 1\n2\n1\n2\n', exp: [2, 2] },
  // n=1：没有边，直接跟 m；注意题面【没有】"边数"这一行
  { name: 'n=1 无操作', text: '1\n7\n0\n1\n1\n', exp: [7] },
  { name: 'n=1 唯一操作1', text: '1\n5\n1\n1 1 10\n1\n1\n', exp: [15] },
  { name: 'n=1 唯一操作2', text: '1\n5\n1\n2 1 10\n1\n1\n', exp: [15] },
  { name: '链-根被操作2', text: '3\n0 0 0\n1 2\n2 3\n1\n2 1 5\n3\n1\n2\n3\n', exp: [5, 5, 0] },
  { name: 'y 取上界10', text: '2\n1000000000 1000000000\n1 2\n2\n1 1 10\n2 1 10\n2\n1\n2\n', exp: [1000000010 + 10, 1000000010 + 10] },
];

/* 手算核对 "链-根被操作2"：树 1-2-3（1 为根）。操作 2 1 5：加给 1 + 父亲(无) + 儿子{2}
 *   => val[1]=5, val[2]=5, val[3]=0
 * 手算核对 "y 取上界10"：两个操作都作用在 1：
 *   op1 1 10 -> 子树{1,2} 各 +10
 *   op2 1 10 -> 1 自己 +10，儿子{2} +10  => 1 +10, 2 +10
 *   val[1] = 1e9 + 10 + 10 = 1000000020
 *   val[2] = 1e9 + 10 + 10 = 1000000020
 */

/* ------------------------------------------------------------------ *
 * 主流程
 * ------------------------------------------------------------------ */
function eq(x, y) { return x.length === y.length && x.every((v, i) => v === y[i]); }

function main() {
  const argv = process.argv.slice(2);
  let rounds = 500;
  let exePath = null;
  for (const arg of argv) {
    let mm;
    if ((mm = arg.match(/^--rounds=(\d+)$/))) rounds = parseInt(mm[1], 10);
    else if ((mm = arg.match(/^--exe=(.+)$/))) exePath = mm[1];
  }

  let fail = 0;

  // --- 1) 定点用例 ---
  for (const c of CASES) {
    const got = solveFast(c.text);
    if (!eq(got, c.exp)) {
      console.log(`[定点] ${c.name}: 期望 ${c.exp.join(',')} 实得 ${got.join(',')}  ❌`);
      ++fail;
    }
  }
  console.log(`定点用例 ${CASES.length} 组，失败 ${fail} 组`);

  // --- 2) 随机对拍：正解 vs 暴力 ---
  let bad1 = 0, bad2 = 0;
  let firstBad = null, firstWrong = null;
  for (let r = 0; r < rounds; ++r) {
    const t = genRandom();
    const f = solveFast(t), b = solveBrute(t), w = solveWrong(t);
    if (!eq(f, b)) {
      ++bad1;
      if (!firstBad) firstBad = { t, f, b };
    }
    if (!eq(f, w)) {
      ++bad2;
      if (!firstWrong) firstWrong = { t, f, w };
    }
  }
  console.log(`随机对拍 ${rounds} 组：正解 vs 独立暴力  不一致 ${bad1} 组`);
  console.log(`错版量化 ${rounds} 组：「操作2 漏掉父亲项」与正解  不一致 ${bad2} 组`);
  if (firstBad) {
    console.log('--- 首个正解/暴力不一致 ---');
    console.log(firstBad.t);
    console.log('正解:', firstBad.f.join(','));
    console.log('暴力:', firstBad.b.join(','));
  }
  if (firstWrong) {
    console.log('--- 首个错版不一致（示例）---');
    console.log('正解:', firstWrong.f.join(','));
    console.log('错版:', firstWrong.w.join(','));
  }

  // --- 3) 可选：调真实 exe ---
  if (exePath) {
    const exe = path.resolve(exePath);
    if (!fs.existsSync(exe)) {
      console.log(`[跳过] 找不到 exe: ${exe}`);
    } else {
      let badExe = 0, tried = 0;
      for (let r = 0; r < Math.min(rounds, 60); ++r) {
        const t = genRandom();
        const exp = solveFast(t);
        const res = spawnSync(exe, { input: t, encoding: 'utf8' });
        if (res.error) { console.log(`[跳过] spawn 失败（沙箱常见 EBUSY）：${res.error.code || res.error.message}`); break; }
        const got = res.stdout.trim().split(/\s+/).filter(Boolean).map(Number);
        ++tried;
        if (!eq(got, exp)) { ++badExe; if (badExe <= 2) console.log('exe 不一致:\n' + t + '\n期望 ' + exp.join(',') + '\n实得 ' + got.join(',')); }
      }
      console.log(`exe 对拍 ${tried} 组：不一致 ${badExe} 组`);
      fail += badExe;
    }
  }

  if (bad1 > 0) ++fail;
  console.log(fail === 0 ? '全部通过 ✅' : `存在 ${fail} 类问题 ❌`);
  process.exit(fail === 0 ? 0 : 1);
}

main();
