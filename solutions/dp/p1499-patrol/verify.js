// P1499 公路巡逻 · 对拍（仅用于验证，非讲解代码；讲解代码一律 C++，见 AGENTS.md §3）
// 用法：
//   node verify.js                 —— 官方样例(内存实现) + 缩小时限窗口的 DP vs 全枚举暴力 20000 组
//                                      + “闭式判据 vs 10001 点采样定义”三方一致性检查
//   node verify.js --exe sol.exe   —— 额外把编译好的真·C++ 程序在【完整 [300,600] 窗口】下逐组对拍
//
// 三份实现互相独立：
//   1) closedDP  —— 正解：滚动 DP + 差分桶（照搬 solution.cpp 的半区间优化）
//   2) bruteDFS  —— 暴力：对每段耗时 d∈[lo,hi] 做 DFS 全搜索，逐辆车判相遇
//   3) meetSampling —— 相遇判据的“几何定义版”：把 [0,1] 均匀采样 10001 点看两曲线是否相交 + 端点同时到达单独判
//   meetAnalytic  —— 相遇判据的“解析版”：直接用两直线端点差判相交/共点（独立于差分桶，用作 20000 组的快速判据）
// 目标：验证 (1)≡(2)（算法正确），并验证 (meetAnalytic)≡(meetSampling)（判据忠实于定义）。

const { execFileSync } = require('child_process');

// ---------- 相遇判据 ----------
function meetAnalytic(a, b, T, X) {
  const g0 = a - T, g1 = b - X;
  if (g0 === 0) return false;              // 同时出发（含共轨）→ 整段不算相遇
  if (g1 === 0) return true;               // 同时到达下一关口且非同时出发 → 相遇
  return (g0 < 0 && g1 > 0) || (g0 > 0 && g1 < 0); // 异号 → 超车 → 相遇
}

// 几何定义版：对 x∈[0,1] 采样 10001 点，用整数值 g(x_k)*M 精确判号，检测内部穿越
function meetSampling(a, b, T, X) {
  const M = 10000;
  const g0 = a - T;
  if (g0 === 0) return false;              // 根只在 x=0（或整条重合），不算
  const slope = (b - a) - (X - T);         // g(x) = g0 + slope*x（巡逻车 τ_p=T+(X-T)x）
  const GM0 = g0 * M;
  let prev = GM0;                          // x=0 处
  for (let k = 1; k <= M; k++) {
    const cur = GM0 + slope * k;           // = M * g(k/M)，整数无浮点误差
    if ((prev < 0 && cur > 0) || (prev > 0 && cur < 0)) return true; // 相邻样本异号 → 内部交叉
    if (cur === 0 && k < M) return true;   // 内部样本恰为 0 → 交叉
    prev = cur;
  }
  const g1 = b - X;                        // 端点同时到达单独判（g0≠0 已保证非同时出发）
  return g1 === 0;
}

// ---------- 正解：滚动 DP + 差分桶（closedDP 与 solution.cpp 同逻辑） ----------
function closedDP(cars, n, Dlo, Dhi, T0) {
  const W = Dhi - Dlo, INF = 1e9;
  const MAX = T0 + Dhi * (n - 1) + 2;
  let cur = new Array(MAX + 1).fill(INF); cur[T0] = 0;
  for (let i = 1; i <= n - 1; i++) {
    const nxt = new Array(MAX + 1).fill(INF);
    const seg = cars[i] || [];
    const loA = T0 + Dlo * (i - 1), hiA = T0 + Dhi * (i - 1);
    for (let a = loA; a <= hiA; a++) {
      if (cur[a] >= INF) continue;
      const dif = new Array(W + 1).fill(0);
      for (const p of seg) {
        const T = p.T, X = p.X;
        if (T === a) continue;
        const k = X - (a + Dlo);
        if (T > a) { if (k <= W) dif[k < 0 ? 0 : k]++; }
        else { if (k >= 0) { dif[0]++; if (k + 1 <= W) dif[k + 1]--; } }
      }
      let acc = 0;
      for (let kk = 0; kk <= W; kk++) { acc += dif[kk]; const b = a + Dlo + kk; const v = cur[a] + acc; if (v < nxt[b]) nxt[b] = v; }
    }
    cur = nxt;
  }
  const loF = T0 + Dlo * (n - 1), hiF = T0 + Dhi * (n - 1);
  let best = INF, bj = loF;
  for (let j = loF; j <= hiF; j++) if (cur[j] < best) { best = cur[j]; bj = j; }
  return { enc: best, end: bj };
}

// ---------- 暴力：DFS 全枚举每段耗时 ----------
function bruteDFS(cars, n, Dlo, Dhi, T0, meet) {
  let best = Infinity, bj = 0, total = 0;
  const arr = new Array(n + 2); arr[1] = T0;
  (function rec(i) {
    if (i === n) {
      if (total < best || (total === best && arr[n] < bj)) { best = total; bj = arr[n]; }
      return;
    }
    const a = arr[i], seg = cars[i] || [];
    for (let d = Dlo; d <= Dhi; d++) {
      const b = a + d;
      let e = 0;
      for (const p of seg) if (meet(a, b, p.T, p.X)) e++;
      total += e; arr[i + 1] = b; rec(i + 1); total -= e;
    }
  })(1);
  return { enc: best, end: bj };
}

// ---------- 随机出题（缩小窗口，便于 DFS 全搜索） ----------
function genInstance(n, m, Dlo, Dhi, wideT) {
  const T0 = 21600, cars = {};
  const span = Dhi * (n - 1);
  for (let c = 0; c < m; c++) {
    const s = 1 + Math.floor(Math.random() * (n - 1)); // 段号 1..n-1
    const dur = Dlo + Math.floor(Math.random() * (Dhi - Dlo + 1));
    let T;
    if (wideT) T = T0 - 2 * span + Math.floor(Math.random() * (4 * span)); // 覆盖早于/晚于目标车
    else T = T0 + Math.floor(Math.random() * (span + 1));
    (cars[s] || (cars[s] = [])).push({ T, X: T + dur });
  }
  return { cars, n, T0 };
}

function same(r1, r2) { return r1.enc === r2.enc && r1.end === r2.end; }

// ---------- 官方样例（内存实现，完整窗口） ----------
console.log('--- 官方样例（3 2 / 段1:060000 301 / 段2:060300 600）---');
{
  const cars = { 1: [{ T: 21600, X: 21901 }], 2: [{ T: 21780, X: 22380 }] };
  const dp = closedDP(cars, 3, 300, 600, 21600);
  const bf = bruteDFS(cars, 3, 300, 600, 21600, meetAnalytic);
  const fmt = (s) => { const h = (s / 3600) | 0, m = ((s % 3600) / 60) | 0, ss = s % 60; return String(h).padStart(2, '0') + String(m).padStart(2, '0') + String(ss).padStart(2, '0'); };
  console.log(`closedDP -> enc=${dp.enc} end=${fmt(dp.end)} | bruteDFS -> enc=${bf.enc} end=${fmt(bf.end)} | 期望 enc=0 end=061301`);
  console.log((dp.enc === 0 && fmt(dp.end) === '061301' && same(dp, bf)) ? 'OK' : 'FAIL');
}

// ---------- 20000 组：缩小时限窗口 DP正解 vs 全枚举暴力(解析判据) ----------
console.log('--- 主对拍：closedDP vs bruteDFS(解析)，缩窗口 [3,7]、n≤5、m≤6，20000 组 ---');
{
  let bad = 0;
  for (let g = 0; g < 20000; g++) {
    const n = 2 + Math.floor(Math.random() * 4);     // 2..5
    const m = 1 + Math.floor(Math.random() * 6);      // 1..6
    const inst = genInstance(n, m, 3, 7, g % 3 === 0);
    const A = closedDP(inst.cars, inst.n, 3, 7, inst.T0);
    const B = bruteDFS(inst.cars, inst.n, 3, 7, inst.T0, meetAnalytic);
    if (!same(A, B)) { bad++; if (bad <= 5) console.log('不一致 n=' + inst.n + ' m=' + m + ' JSON=' + JSON.stringify(inst.cars) + ' dp=' + JSON.stringify(A) + ' bf=' + JSON.stringify(B)); }
  }
  console.log(bad === 0 ? '20000 组全部一致' : `存在 ${bad} 组不一致`);
  if (bad > 0) process.exitCode = 1;
}

// ---------- 采样定义 vs 解析判据 vs DP 三方一致性 ----------
console.log('--- 三方一致性：meetSampling(10001点) vs meetAnalytic vs closedDP，n≤4、m≤4、窗口[2,5] ---');
{
  let N = 0, mismatchMeet = 0, mismatchAns = 0;
  const SAMPLE_GROUPS = 300;
  for (let g = 0; g < SAMPLE_GROUPS; g++) {
    const n = 2 + Math.floor(Math.random() * 3);      // 2..4
    const m = 1 + Math.floor(Math.random() * 4);       // 1..4
    const inst = genInstance(n, m, 2, 5, true);
    N++;
    // 逐辆车的两种判据是否一致
    let meetDiff = false;
    for (const s in inst.cars) for (const p of inst.cars[s]) {
      for (let d = 2; d <= 5; d++) for (let aOff = -3; aOff <= 8; aOff++) {
        const a = inst.T0 + aOff, b = a + d;
        if (meetAnalytic(a, b, p.T, p.X) !== meetSampling(a, b, p.T, p.X)) meetDiff = true;
      }
    }
    if (meetDiff) mismatchMeet++;
    const DPA = closedDP(inst.cars, inst.n, 2, 5, inst.T0);
    const BFS = bruteDFS(inst.cars, inst.n, 2, 5, inst.T0, meetSampling);
    if (!same(DPA, BFS)) mismatchAns++;
  }
  console.log(`检查 ${N} 组：判据不一致 ${mismatchMeet} 组，采样暴力答案与 DP 不一致 ${mismatchAns} 组`);
  if (mismatchMeet > 0 || mismatchAns > 0) process.exitCode = 1;
}

// ---------- --exe：真·C++ 程序在完整 [300,600] 窗口逐组对拍 ----------
const eIdx = process.argv.indexOf('--exe');
if (eIdx >= 0) {
  const exe = process.argv[eIdx + 1];
  const toHHMMSS = (s) => { const h = (s / 3600) | 0, m = ((s % 3600) / 60) | 0, ss = s % 60; return String(h).padStart(2, '0') + String(m).padStart(2, '0') + String(ss).padStart(2, '0'); };
  const fromSec = (s) => toHHMMSS(s);
  console.log('--- --exe：sol.exe(完整窗口) vs bruteDFS(解析判据)，n≤3 ---');
  let bad = 0, done = 0;
  for (let g = 0; g < 200; g++) {
    const n = 2 + (g % 2);              // 2 或 3（DFS 301^(n-1) 可承受）
    const m = 1 + Math.floor(Math.random() * 6);
    const cars = {};
    const lines = [n + ' ' + m];
    for (let c = 0; c < m; c++) {
      const s = 1 + Math.floor(Math.random() * (n - 1));
      const dur = 300 + Math.floor(Math.random() * 301);       // [300,600]
      const T = 18000 + Math.floor(Math.random() * (82800 - 18000)); // 05:00..23:00
      (cars[s] || (cars[s] = [])).push({ T, X: T + dur });
      lines.push(s + ' ' + fromSec(T) + ' ' + dur);
    }
    const input = lines.join('\n') + '\n';
    const out = execFileSync(exe, { input, encoding: 'utf8' }).trim().split(/\s+/);
    const B = bruteDFS(cars, n, 300, 600, 21600, meetAnalytic);
    const expTime = fromSec(B.end);
    done++;
    if (String(B.enc) !== out[0] || expTime !== out[1]) {
      bad++;
      if (bad <= 5) console.log('不一致:\n' + input + '=> exe: ' + out.join(' ') + ' | brute: ' + B.enc + ' ' + expTime);
    }
  }
  console.log(`exe 对拍 ${done} 组，不一致 ${bad} 组`);
  if (bad > 0) process.exitCode = 1;
}
