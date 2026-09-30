// P2234 [HNOI2002] 营业额统计 —— 对拍脚本
// 用法：node verify.js              —— 官方样例 + 定向边界 + 随机对拍（正解 vs 暴力）
//       node verify.js --exe PATH   —— 额外把 PATH 里的 C++ 程序逐组跑 300 组与暴力比对
//
// 注意：本文件里 JS 重写的"正解"与"暴力"**只是验证工具，不是讲解代码**。
// 讲解代码一律见 solution.cpp（C++14，AGENTS.md §3）。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// ---------- 暴力：严格按题面定义 O(n^2) 枚举 ----------
// T = a1（题面规定第一天最小波动值就是第一天营业额本身，负数也照加）
//     + sum_{i>=2} min_{j<i} |a_i - a_j|
// 与正解完全没有共享代码：不看有序性，直接把前面每一天都算一遍。
function brute(a) {
  if (a.length === 0) return 0;
  let ans = a[0];                       // 第一天的特殊定义
  for (let i = 1; i < a.length; i++) {
    let best = Infinity;
    for (let j = 0; j < i; j++) {       // 朴素枚举"该天以前的每一天"
      const d = Math.abs(a[i] - a[j]);
      if (d < best) best = d;
    }
    ans += best;
  }
  return ans;
}

// ---------- 正解（JS 镜像）：有序去重表 + lower_bound 查前驱后继 ----------
function fast(a) {
  if (a.length === 0) return 0;
  const s = [];                         // 模拟 std::set：升序 + 去重
  // 返回第一个 >= x 的下标（等价于 lower_bound）
  const lb = (x) => {
    let lo = 0, hi = s.length;
    while (lo < hi) {
      const mid = (lo + hi) >> 1;
      if (s[mid] < x) lo = mid + 1; else hi = mid;
    }
    return lo;
  };
  let ans = 0;
  for (let i = 0; i < a.length; i++) {
    if (i === 0) {
      ans = a[0];
    } else {
      const p = lb(a[i]);
      let best = Infinity;
      if (p < s.length) best = Math.min(best, s[p] - a[i]);    // 后继 R
      if (p > 0) best = Math.min(best, a[i] - s[p - 1]);       // 前驱 L（p>0 才存在）
      ans += best;
    }
    const p = lb(a[i]);
    if (s[p] !== a[i]) s.splice(p, 0, a[i]);                   // 已存在则不重复插（set 语义）
  }
  return ans;
}

// ---------- 随机数据生成 ----------
function rndInt(lo, hi) { return lo + Math.floor(Math.random() * (hi - lo + 1)); }
// 三种"口味"轮番上阵：小值域（逼出大量重复）、负数域、全域 [-1e6,1e6]
function genData(kind) {
  const n = rndInt(1, 14);
  const a = [];
  for (let i = 0; i < n; i++) {
    if (kind === 0) a.push(rndInt(-4, 4));          // 密集重复
    else if (kind === 1) a.push(rndInt(-1000000, -1)); // 全负
    else if (kind === 2) a.push(rndInt(-1000000, 1000000)); // 全域，含 0
    else a.push(rndInt(0, 9));                        // 极小值域，几乎全是重复
  }
  return a;
}

const fmt = (a) => `${a.length}\n` + a.join('\n') + '\n';

// ---------- 官方样例 ----------
console.log('--- 官方样例（题面）---');
// 输入第一行的 6 是天数 n，营业额序列是后面的 5 1 2 5 4 6
const sampleIn = [5, 1, 2, 5, 4, 6];
{
  const g = fast(sampleIn), b = brute(sampleIn);
  const ok = g === 12 && b === 12;
  console.log(`输入 ${sampleIn.join(' ')} -> 正解=${g} 暴力=${b} 期望=12 ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) process.exit(1);
}

// ---------- 定向边界 ----------
// 下面每个数组都是"营业额序列本身"，天数 n 就是数组长度（不要把它当成 n）
console.log('--- 定向边界（含负数 / 重复值 / n=1）---');
const edges = [
  [7], [-8], [3, 3, 3, 3, 3], [-5, -1, -2, -5],
  [10, -10, 0], [100, 1, 2, 3], [-1, -3, -2, -5, -4],
  [-1000000, 1000000], [5, 4, 3, 2, 1, 0], [0, 1, 2, 3, 4, 5],
  [0, 0, 0], [-7, -7, -3, -20],
];
let edgeBad = 0;
for (const a of edges) {
  const g = fast(a), b = brute(a);
  if (g !== b) edgeBad++;
  console.log(`n=${a.length} [${a.join(',')}] 正解=${g} 暴力=${b} ${g === b ? 'OK' : 'FAIL'}`);
}

// ---------- 随机对拍 ----------
console.log('--- 随机对拍：set 前驱后继 vs 定义式 O(n^2) 暴力 ---');
let bad = 0, tested = 0;
for (let t = 0; t < 200000; t++) {
  const a = genData(t % 4);
  tested++;
  const g = fast(a), b = brute(a);
  if (g !== b) {
    bad++;
    if (bad <= 5) console.log(`不一致 n=${a.length} a=[${a.join(',')}] 正解=${g} 暴力=${b}`);
  }
}
console.log(`共 ${tested} 组，不一致 ${bad} 组`);
if (bad > 0 || edgeBad > 0) process.exit(1);

// ---------- 真·C++ 程序逐组复核 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0, cn = 0;
  for (let t = 0; t < 300; t++) {
    const a = genData(t % 4);
    const out = execFileSync(exe, { input: fmt(a), encoding: 'utf8' }).trim();
    const b = brute(a);
    cn++;
    if (String(b) !== out) {
      cbad++;
      if (cbad <= 5) console.log(`C++ 不一致 n=${a.length} a=[${a.join(',')}] 程序=${out} 暴力=${b}`);
    }
  }
  // 顺手把官方样例也喂给真程序
  const s = execFileSync(exe, { input: fmt(sampleIn), encoding: 'utf8' }).trim();
  console.log(`C++ 程序跑官方样例 -> ${s}（期望 12）${s === '12' ? 'OK' : 'FAIL'}`);
  console.log(`--- C++ 程序 vs 暴力：共 ${cn} 组，不一致 ${cbad} 组`);
  if (cbad > 0 || s !== '12') process.exit(1);
}
