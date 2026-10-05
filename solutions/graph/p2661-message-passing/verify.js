#!/usr/bin/env node
/*
 * P2661 [NOIP 2015 提高组] 信息传递 —— 随机对拍脚本
 *
 * 用法：
 *   node verify.js                  # 默认 500 组随机对拍（用户要求：少生成数据、快一点）
 *   node verify.js --rounds=2000    # 覆盖轮数
 *   node verify.js --exe=./solution.exe   # 额外把编译好的 C++ 正解也拉进对拍
 *
 * 三个实现（互相独立）：
 *   solveFast —— 正解：函数图上走链 + state 三态标记，求最短环（复刻 solution.cpp）
 *   solveBrute—— 暴力：【逐字模拟游戏】。每一轮让所有人同时把"已知生日集合"投递给 to[i]，
 *                一旦有人集合里出现自己的编号就立刻返回轮数。这是对题面最忠实的翻译，
 *                与正解的机制（图论找环）完全不同。
 *   solveWrong—— 错版：不区分"当前链上"与"已完结"，只要撞到任何访问过的点就当环，
 *                环长直接取"走过的总步数"（把接入环的长链也算进去）。
 */
'use strict';
const fs = require('fs');
const path = require('path');
const { spawnSync } = require('child_process');

/* ------------------------------------------------------------------ *
 * 正解：走链 + 三态标记，求最短环
 * ------------------------------------------------------------------ */
function solveFast(to, n) {
  const state = new Array(n + 1).fill(0); // 0 未访问 / 1 当前链 / 2 已完结
  const depth = new Array(n + 1).fill(0);
  let ans = Infinity;
  for (let s = 1; s <= n; ++s) {
    if (state[s] !== 0) continue;
    const pathArr = [];
    let u = s;
    while (state[u] === 0) {
      state[u] = 1;
      depth[u] = pathArr.length;
      pathArr.push(u);
      u = to[u];
    }
    if (state[u] === 1) {
      const cyc = pathArr.length - depth[u];
      if (cyc < ans) ans = cyc;
    }
    for (const v of pathArr) state[v] = 2;
  }
  return [ans];
}

/* ------------------------------------------------------------------ *
 * 暴力：逐字模拟游戏每一轮
 * ------------------------------------------------------------------ */
function solveBrute(to, n) {
  // known[i] = 第 i 个人当前掌握的生日集合（起始时只知道自己的，故为 {i}）。
  // 判据不是"谁手里有自己编号"，而是"谁【从别人那里收到】了含自己编号的消息"：
  // 因此每轮只看「发送方 i 把消息发给 to[i] 时，这条消息里是否含 to[i] 自己的编号」。
  let known = [];
  for (let i = 1; i <= n; ++i) known.push(new Set([i]));
  for (let round = 1; round <= n + 1; ++round) {
    // 同时投递：先冻结"本轮开始时各人掌握的信息"，再统一合并
    const send = known.map((s) => s);       // send[i-1] = 第 i 人本轮发出去的内容
    for (let i = 1; i <= n; ++i) {
      const recv = to[i];                   // 第 i 人把消息发给 recv
      if (send[i - 1].has(recv)) return [round];  // recv 从 i 处听到了自己的生日 → 游戏结束
    }
    const nxt = known.map((s) => new Set(s));
    for (let i = 1; i <= n; ++i) {
      for (const info of send[i - 1]) nxt[to[i] - 1].add(info); // 投递
    }
    known = nxt;
  }
  return [-1];
}

/* ------------------------------------------------------------------ *
 * 错版：凡是撞到已访问的点都当成环，且把整条链长当环长
 * ------------------------------------------------------------------ */
function solveWrong(to, n) {
  const vis = new Array(n + 1).fill(false);
  let ans = Infinity;
  for (let s = 1; s <= n; ++s) {
    if (vis[s]) continue;
    const pathArr = [];
    let u = s;
    while (!vis[u]) {
      vis[u] = true;
      pathArr.push(u);
      u = to[u];
    }
    // 错在这里：无条件把"整条链长度"当成环长，也不区分是不是本链上的点
    if (pathArr.length < ans) ans = pathArr.length;
  }
  return [ans];
}

/* ------------------------------------------------------------------ *
 * 随机数据生成器
 * ------------------------------------------------------------------ */
function rndInt(lo, hi) { return lo + Math.floor(Math.random() * (hi - lo + 1)); }

function genRandom(selfLoopAllowed) {
  // 禁止自环时至少要有 2 个人，否则 do/while 永远找不到 v !== i（会死循环）。
  const n = selfLoopAllowed ? rndInt(1, 12) : rndInt(2, 12);
  const to = [0];
  for (let i = 1; i <= n; ++i) {
    let v;
    do { v = rndInt(1, n); } while (!selfLoopAllowed && v === i);
    to.push(v);
  }
  const text = n + '\n' + to.slice(1).join(' ') + '\n';
  return { text, to, n };
}

/* ------------------------------------------------------------------ *
 * 定点用例
 * ------------------------------------------------------------------ */
const CASES = [
  { name: '官方样例', text: '5\n2 4 2 3 1\n', exp: [3] },
  { name: 'n=1 自环', text: '1\n1\n', exp: [1] },
  { name: 'n=2 互相传', text: '2\n2 1\n', exp: [2] },
  { name: 'n=2 自环+自环', text: '2\n1 2\n', exp: [1] },
  { name: 'n=3 一个大环', text: '3\n2 3 1\n', exp: [3] },
  // 1->2->3->4->2：环是 {2,3,4} 长 3，点 1 是接入环的尾巴
  // 正解 3；错版会把 {1,2,3,4} 整条链当环算出 4
  { name: '尾巴接环（关键用例）', text: '4\n2 3 4 2\n', exp: [3] },
  // 5->4->3->2->1->5 一个大环 5，加一个自环 6->6
  { name: '大环 + 自环取最小', text: '6\n5 1 2 3 4 6\n', exp: [1] },
];

/* ------------------------------------------------------------------ *
 * 主流程
 * ------------------------------------------------------------------ */
function eq(x, y) { return x.length === y.length && x.every((v, i) => v === y[i]); }

function main() {
  const argv = process.argv.slice(2);
  let rounds = 500, exePath = null;
  for (const arg of argv) {
    let mm;
    if ((mm = arg.match(/^--rounds=(\d+)$/))) rounds = parseInt(mm[1], 10);
    else if ((mm = arg.match(/^--exe=(.+)$/))) exePath = mm[1];
  }

  let fail = 0;

  // --- 1) 定点用例：正解必须命中期望；暴力也必须一致（互相印证） ---
  for (const c of CASES) {
    const t = c.text.trim().split(/\s+/).map(Number);
    const n = t[0], to = [0, ...t.slice(1, 1 + n)];
    const got = solveFast(to, n);
    const br = solveBrute(to, n);
    if (!eq(got, c.exp)) { console.log(`[定点] ${c.name}: 正解期望 ${c.exp} 实得 ${got}  ❌`); ++fail; }
    if (!eq(br, c.exp)) { console.log(`[定点] ${c.name}: 暴力期望 ${c.exp} 实得 ${br}  ❌`); ++fail; }
  }
  console.log(`定点用例 ${CASES.length} 组（正解 + 暴力各验一遍），失败 ${fail} 组`);

  // --- 2) 随机对拍 ---
  let bad1 = 0, bad2 = 0, firstBad = null, firstWrong = null;
  for (let r = 0; r < rounds; ++r) {
    // 一半用例允许自环（答案可能为 1），一半禁止自环
    const allowSelf = (r % 2 === 0);
    const { to, n } = genRandom(allowSelf);
    const f = solveFast(to, n), b = solveBrute(to, n), w = solveWrong(to, n);
    if (!eq(f, b)) { ++bad1; if (!firstBad) firstBad = { to, n, f, b }; }
    if (!eq(f, w)) { ++bad2; if (!firstWrong) firstWrong = { to, n, f, w }; }
  }
  console.log(`随机对拍 ${rounds} 组：正解 vs 逐轮模拟暴力  不一致 ${bad1} 组`);
  console.log(`错版量化 ${rounds} 组：「不区分在链/已完结 + 链长当环长」与正解  不一致 ${bad2} 组`);
  if (firstBad) console.log('首个正解/暴力不一致: to=' + firstBad.to.slice(1).join(' ') + ' 正解=' + firstBad.f + ' 暴力=' + firstBad.b);
  if (firstWrong) console.log('首个错版示例: to=' + firstWrong.to.slice(1).join(' ') + ' 正解=' + firstWrong.f + ' 错版=' + firstWrong.w);

  // --- 3) 可选：调真实 exe ---
  if (exePath) {
    const exe = path.resolve(exePath);
    if (!fs.existsSync(exe)) {
      console.log(`[跳过] 找不到 exe: ${exe}`);
    } else {
      let badExe = 0, tried = 0;
      for (let r = 0; r < Math.min(rounds, 80); ++r) {
        const { to, n } = genRandom(r % 2 === 0);
        const exp = solveFast(to, n);
        const res = spawnSync(exe, { input: n + '\n' + to.slice(1).join(' ') + '\n', encoding: 'utf8' });
        if (res.error) { console.log(`[跳过] spawn 失败（沙箱常见 EBUSY）：${res.error.code || res.error.message}`); break; }
        const got = res.stdout.trim().split(/\s+/).filter(Boolean).map(Number);
        ++tried;
        if (!eq(got, exp)) { ++badExe; if (badExe <= 2) console.log('exe 不一致: to=' + to.slice(1).join(' ') + ' 期望 ' + exp + ' 实得 ' + got); }
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
