// P2058 海港 对拍工具（仅用于验证，非讲解代码；讲解代码一律 C++，见 solution.cpp / README.md）
// 用法：node verify.js                 —— 官方样例断言 + 正解 vs 暴力随机对拍
//       node verify.js --exe ./sol.exe —— 额外把编译好的 C++ 程序逐组跑一遍（300 组）
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const DAY = 86400;

// ---------- 正解（与 C++ 版逐句对应）：滑动窗口队列 + 国籍计数数组 ----------
function solved(ships) {
  const cnt = new Int32Array(100005); // 国籍 x <= 1e5，与 C++ 的 cnt[100005] 一致
  const q = [];                       // 队列，head 指针代替真实 shift，O(1) 弹出
  let head = 0;
  let distinct = 0;
  const ans = [];
  for (const s of ships) {
    for (const x of s.pax) {
      if (cnt[x]++ === 0) distinct++;
      q.push({ x, t: s.t });
    }
    const limit = s.t - DAY;
    while (head < q.length && q[head].t <= limit) {
      if (--cnt[q[head].x] === 0) distinct--;
      head++;
    }
    ans.push(distinct);
  }
  return ans;
}

// ---------- 暴力（独立思路，照抄题目定义）----------
// 对每艘船 i：往回把窗口内每一艘船的每一个乘客塞进一个全新的 Set，Set 的大小就是答案。
// 不维护任何增量信息，每艘船都从零重建 —— 这正是 O(n * sum k) 的笨办法。
function brute(ships) {
  const ans = [];
  for (let i = 0; i < ships.length; i++) {
    const set = new Set();
    for (let p = i; p >= 0; p--) {
      if (ships[p].t > ships[i].t - DAY) {
        for (const x of ships[p].pax) set.add(x);
      }
    }
    ans.push(set.size);
  }
  return ans;
}

// ---------- 输入输出文本格式（与洛谷题面一致，供 --exe 使用）----------
function toInput(ships) {
  const lines = [String(ships.length)];
  for (const s of ships) lines.push([s.t, s.pax.length, ...s.pax].join(' '));
  return lines.join('\n') + '\n';
}
function parseOut(text) {
  return text.trim().split(/\s+/).filter(s => s !== '').map(Number);
}

// ---------- 随机数据生成 ----------
// 时间增量刻意取 86399 / 86400 / 86401 / 86402 这几个值，专门踩 "t_i - 86400" 这条边界；
// 国籍只从小集合里抽，制造大量重复，逼出 cnt 的 0<->1 跳变是否正确。
function genRandom(rng) {
  const n = 1 + Math.floor(rng() * 12);
  const steps = [1, 2, 86399, 86400, 86401, 86402, 172800];
  let t = 1 + Math.floor(rng() * 5000);
  const ships = [];
  const pool = [1, 2, 3, 4, 5, 6];
  const big = rng() < 0.15; // 少数数据用大国籍号，检查数组下标上限
  for (let i = 0; i < n; i++) {
    if (i > 0) {
      const d = rng() < 0.5 ? steps[Math.floor(rng() * steps.length)]
                            : 1 + Math.floor(rng() * 200000);
      t += d;
    }
    const k = Math.floor(rng() * 5); // 允许 0 个乘客的空船
    const pax = [];
    for (let j = 0; j < k; j++) pax.push(big ? 1 + Math.floor(rng() * 100000) : pool[Math.floor(rng() * pool.length)]);
    ships.push({ t, pax });
  }
  return ships;
}

function makeRng(seed) {
  let s = seed >>> 0;
  return () => { s = (s * 1664525 + 1013904223) >>> 0; return s / 4294967296; };
}

// ---------- 错误写法复现（专门用来证明 README 里的坑是真的）----------
// 把过期判定 `q[head].t <= limit` 写成 `< limit`（即窗口当成闭区间 [t-86400, t]），
// 左边界上那个人会被多留下来。
function solvedWrongOpen(ships) {
  const cnt = new Int32Array(100005);
  const q = [];
  let head = 0, distinct = 0;
  const ans = [];
  for (const s of ships) {
    for (const x of s.pax) { if (cnt[x]++ === 0) distinct++; q.push({ x, t: s.t }); }
    const limit = s.t - DAY;
    while (head < q.length && q[head].t < limit) { if (--cnt[q[head].x] === 0) distinct--; head++; }
    ans.push(distinct);
  }
  return ans;
}

// ---------- 官方样例 ----------
const SAMPLES = [
  {
    name: '样例 #1',
    input: '3\n1 4 4 1 2 2\n2 2 2 3\n10 1 3\n',
    expect: [3, 4, 4],
  },
  {
    name: '样例 #2',
    input: '4\n1 4 1 2 2 3\n3 2 2 3\n86401 2 3 4\n86402 1 5\n',
    expect: [3, 3, 3, 4],
  },
];

function parseInput(text) {
  const tk = text.trim().split(/\s+/).filter(s => s !== '').map(Number);
  let i = 0;
  const n = tk[i++];
  const ships = [];
  for (let j = 0; j < n; j++) {
    const t = tk[i++], k = tk[i++];
    const pax = [];
    for (let q = 0; q < k; q++) pax.push(tk[i++]);
    ships.push({ t, pax });
  }
  return ships;
}

let sampleFail = 0;
console.log('--- 官方样例（题面逐字版见 problem.txt）---');
for (const sp of SAMPLES) {
  const ships = parseInput(sp.input);
  const g = solved(ships), b = brute(ships);
  const okG = g.join(',') === sp.expect.join(',');
  const okB = b.join(',') === sp.expect.join(',');
  if (!okG || !okB) sampleFail++;
  console.log(`${sp.name}: 期望=${sp.expect.join(' ')} 正解=${g.join(' ')} 暴力=${b.join(' ')} ${okG && okB ? 'OK' : 'FAIL'}`);
}

console.log('--- 错误写法对照：过期判定写成 `< limit`（窗口当闭区间）---');
for (const sp of SAMPLES) {
  const ships = parseInput(sp.input);
  const w = solvedWrongOpen(ships);
  console.log(`${sp.name}: 期望=${sp.expect.join(' ')} 错误写法=${w.join(' ')} ${w.join(',') === sp.expect.join(',') ? '碰巧对' : 'WA（正是坑 1）'}`);
}

// ---------- 定向边界（窗口开闭、空船、一次弹多艘）----------
console.log('--- 定向边界 ---');
const EDGE = [
  { name: '恰好相差 86400 秒（左开：t = t_i-86400 的人必须弹出）', ships: [{ t: 1, pax: [7] }, { t: 86401, pax: [8] }] },
  { name: '相差 86399 秒（仍在窗口内，不能弹）', ships: [{ t: 1, pax: [7] }, { t: 86400, pax: [8] }] },
  { name: '空船 k=0', ships: [{ t: 5, pax: [1, 2] }, { t: 6, pax: [] }, { t: 7, pax: [3] }] },
  { name: '一次弹掉多艘（时间跳跃约 3 天）', ships: [{ t: 1, pax: [1] }, { t: 2, pax: [2] }, { t: 3, pax: [3] }, { t: 300000, pax: [4] }] },
  { name: '同国籍同船重复 3 次', ships: [{ t: 1, pax: [9, 9, 9] }] },
  { name: '国籍号到 1e5 上限', ships: [{ t: 1, pax: [100000] }, { t: 2, pax: [100000, 1] }] },
  { name: '两艘船时间相同（非严格递增的极端）', ships: [{ t: 100, pax: [1] }, { t: 100, pax: [2] }] },
  { name: '窗口内只剩 1 人，下一艘船把他挤出去', ships: [{ t: 10, pax: [1] }, { t: 86411, pax: [2] }] },
];
let edgeBad = 0;
for (const e of EDGE) {
  const g = solved(e.ships), b = brute(e.ships), w = solvedWrongOpen(e.ships);
  const ok = g.join(',') === b.join(',');
  if (!ok) edgeBad++;
  console.log(`${e.name}: 正解=${g.join(' ')} 暴力=${b.join(' ')} ${ok ? 'OK' : 'FAIL'}｜` +
              `错写 < 版=${w.join(' ')}${w.join(',') === b.join(',') ? '（碰巧也对）' : '（错）'}`);
}

// ---------- 随机对拍 ----------
console.log('--- 随机对拍：滑动窗口正解 vs Set 重建暴力 ---');
const rng = makeRng(20161015);
const GROUPS = 200000;
let bad = 0, wrongBad = 0;
for (let it = 0; it < GROUPS; it++) {
  const ships = genRandom(rng);
  const g = solved(ships), b = brute(ships);
  if (solvedWrongOpen(ships).join(',') !== b.join(',')) wrongBad++;
  if (g.join(',') !== b.join(',')) {
    bad++;
    console.log(`不一致！输入：\n${toInput(ships)}\n正解=${g.join(' ')} 暴力=${b.join(' ')}`);
    break;
  }
}
console.log(`共 ${GROUPS} 组，不一致 ${bad} 组`);
console.log(`（同批数据上，把 <= 写成 < 的错误版本错了 ${wrongBad} 组 —— 坑 1 不是理论问题）`);

// ---------- 规模压力（只跑正解，验证 3e5 乘客不超时）----------
{
  const ships = [];
  let t = 1;
  const r2 = makeRng(7);
  let total = 0;
  while (ships.length < 100000 && total < 300000) {
    t += 1 + Math.floor(r2() * 3);
    const k = Math.min(5, 300000 - total);
    const pax = [];
    for (let j = 0; j < k; j++) pax.push(1 + Math.floor(r2() * 100000));
    total += k;
    ships.push({ t, pax });
  }
  const t0 = Date.now();
  const g = solved(ships);
  console.log(`--- 压力：${ships.length} 艘船 / ${total} 名乘客，正解耗时 ${Date.now() - t0}ms，最后一行答案 ${g[g.length - 1]}`);
}

// ---------- 真·C++ 程序逐组复核 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0;
  const CN = 300;
  const r3 = makeRng(987654321);
  for (let it = 0; it < CN; it++) {
    const ships = genRandom(r3);
    const out = parseOut(execFileSync(exe, { input: toInput(ships), encoding: 'utf8' }));
    const b = brute(ships);
    if (out.join(',') !== b.join(',')) {
      cbad++;
      console.log(`C++ 不一致！输入：\n${toInput(ships)}\n程序=${out.join(' ')} 暴力=${b.join(' ')}`);
      break;
    }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${CN} 组，不一致 ${cbad} 组`);
  if (cbad > 0) process.exit(1);
}

if (sampleFail > 0 || bad > 0 || edgeBad > 0) process.exit(1);
console.log('全部通过');
