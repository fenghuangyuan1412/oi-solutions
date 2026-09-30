// P4387 验证栈序列 对拍工具（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 正解：贪心模拟（按 pushed 顺序压入，"能弹就弹"——栈顶==poped[j] 就连续弹）
// 暴力：DFS 穷举所有合法压/弹调度，判断 poped 是否可达（本质区别：贪心只走一条路，暴力两分支全试）
// 用法：node verify.js              —— 官方样例 + 随机对拍
//       node verify.js --exe PATH   —— 额外用编译好的 C++ 程序复核
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// 正解：贪心
function solveGreedy(pushed, poped) {
  const n = pushed.length, st = [];
  let j = 0;
  for (let i = 0; i < n; i++) {
    st.push(pushed[i]);
    while (st.length && j < n && st[st.length - 1] === poped[j]) { st.pop(); j++; }
  }
  return j === n;
}

// 暴力：DFS 枚举所有调度
function solveBrute(pushed, poped) {
  const n = pushed.length, stk = [];
  let found = false;
  function dfs(i, j) { // i=已压入个数, j=已按序弹出个数
    if (found) return;
    if (j === n) { found = true; return; }
    if (stk.length && stk[stk.length - 1] === poped[j]) { // 弹（仅当正好是目标）
      stk.pop(); dfs(i, j + 1); stk.push(poped[j]);
    }
    if (i < n && !found) { // 压
      stk.push(pushed[i]); dfs(i + 1, j); stk.pop();
    }
  }
  dfs(0, 0);
  return found;
}

function shuffle(a) {
  for (let i = a.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [a[i], a[j]] = [a[j], a[i]];
  }
  return a;
}
function rndPerm(n) { return shuffle([...Array(n)].map((_, i) => i + 1)); }
function inputText(cases) {
  let s = cases.length + '\n';
  for (const [p, q] of cases) s += p.length + '\n' + p.join(' ') + '\n' + q.join(' ') + '\n';
  return s;
}

// ---------- 官方样例 ----------
console.log('--- 官方样例 ---');
const sampleInput = [
  [[1, 2, 3, 4, 5], [5, 4, 3, 2, 1], true],
  [[1, 2, 3, 4], [2, 4, 1, 3], false],
];
let sampleOK = true;
for (const [p, q, exp] of sampleInput) {
  const g = solveGreedy(p, q), b = solveBrute(p, q);
  const ok = g === exp && b === exp;
  if (!ok) sampleOK = false;
  console.log(`pushed=${p} poped=${q} -> 贪心=${g ? 'Yes' : 'No'} 暴力=${b ? 'Yes' : 'No'} 期望=${exp ? 'Yes' : 'No'} ${ok ? 'OK' : 'FAIL'}`);
}

// ---------- 定向边界 ----------
console.log('--- 定向边界 ---');
const edge = [
  [[1], [1]], [[1, 2], [1, 2]], [[1, 2], [2, 1]],
  [[2, 1], [1, 2]], [[2, 1], [2, 1]], [[1, 2, 3], [1, 3, 2]],
  [[1, 2, 3], [3, 1, 2]], [[3, 1, 2], [1, 2, 3]],
];
for (const [p, q] of edge) {
  const g = solveGreedy(p, q), b = solveBrute(p, q);
  console.log(`pushed=${p} poped=${q} -> 贪心=${g ? 'Yes' : 'No'} 暴力=${b ? 'Yes' : 'No'} ${g === b ? 'OK' : 'FAIL'}`);
}

// ---------- 随机对拍：贪心 vs DFS 暴力 ----------
// 暴力是指数级，n 取小值；两组 pushed/poped 都随机打乱，重点覆盖 No 情形
console.log('--- 随机对拍：贪心 vs DFS 暴力 ---');
const GROUPS = 30000;
let bad = 0, tested = 0, firstBad = null;
for (let t = 0; t < GROUPS; t++) {
  const n = 1 + Math.floor(Math.random() * 7); // 1..7
  const pushed = rndPerm(n);
  const poped = rndPerm(n);
  tested++;
  const g = solveGreedy(pushed, poped), b = solveBrute(pushed, poped);
  if (g !== b) { bad++; if (!firstBad) firstBad = { pushed, poped, g, b }; }
}
if (firstBad) console.log(`第一组不一致 pushed=${firstBad.pushed} poped=${firstBad.poped} 贪心=${firstBad.g} 暴力=${firstBad.b}`);
console.log(`共 ${tested} 组，不一致 ${bad} 组`);

// ---------- 压力：n=1e5 x 5 组，验证正解不超时 ----------
{
  const n = 100000;
  const pushed = rndPerm(n);
  const poped = [...pushed].reverse(); // 最坏连弹情形
  const t0 = Date.now();
  const r = solveGreedy(pushed, poped);
  console.log(`--- 压力：n=1e5 反序，结果=${r ? 'Yes' : 'No'}，耗时 ${Date.now() - t0}ms`);
}

// ---------- 真·C++ 程序逐组复核 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0, cn = 0;
  for (let batch = 0; batch < 40; batch++) { // 40 批 x 5 组 = 200 组
    const cases = [];
    for (let k = 0; k < 5; k++) { const n = 1 + Math.floor(Math.random() * 7); cases.push([rndPerm(n), rndPerm(n)]); }
    const out = execFileSync(exe, { input: inputText(cases), encoding: 'utf8' }).trim().split(/\r?\n/);
    for (let k = 0; k < cases.length; k++) {
      const exp = solveBrute(cases[k][0], cases[k][1]) ? 'Yes' : 'No';
      cn++;
      if ((out[k] || '').trim() !== exp) { cbad++; if (cbad <= 5) console.log(`C++ 不一致 pushed=${cases[k][0]} poped=${cases[k][1]} 程序=${out[k]} 暴力=${exp}`); }
    }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${cn} 组，不一致 ${cbad} 组`);
  if (cbad > 0) process.exit(1);
}

if (!sampleOK || bad > 0) process.exit(1);
