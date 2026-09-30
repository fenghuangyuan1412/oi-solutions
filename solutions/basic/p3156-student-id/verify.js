// P3156 询问学号 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                —— 官方样例 + 随机对拍 200000 组 + 定向边界
//       node verify.js --exe PATH     —— 额外把编译好的 C++ 程序逐组跑 300 组
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// 「正解」：把进教室顺序写进 1 起头的数组，询问直接下标访问
function fast(ids, queries) {
  const a = [0];                      // a[1..n]
  for (let i = 0; i < ids.length; i++) a.push(ids[i]);
  return queries.map(i => a[i]);
}

// 「暴力」：独立思路 —— 不按下标取，每次询问都从头开始数"第 i 个人"
function brute(ids, queries) {
  const ans = [];
  for (const i of queries) {
    let cnt = 0, got = -1;
    for (let j = 0; j < ids.length; j++) {   // 一路往后数人头
      cnt++;
      if (cnt === i) { got = ids[j]; break; }
    }
    ans.push(got);
  }
  return ans;
}

const rnd = (lo, hi) => lo + Math.floor(Math.random() * (hi - lo + 1));
const toInput = (n, m, ids, queries) =>
  `${n} ${m}\n${ids.join(' ')}\n${queries.join(' ')}\n`;

let badTotal = 0;

// ---------- ① 官方样例（题面唯一的样例） ----------
console.log('--- 官方样例 ---');
{
  const ids = [1, 9, 2, 60, 8, 17, 11, 4, 5, 14];
  const queries = [1, 5, 9];
  const expect = '1\n8\n5';
  const f = fast(ids, queries).join('\n');
  const b = brute(ids, queries).join('\n');
  const ok = f === expect && b === expect;
  console.log(`10 3 / 1 9 2 60 8 17 11 4 5 14 / 1 5 9\n  下标直取 -> ${f.replace(/\n/g, '⏎')}`);
  console.log(`  从头数   -> ${b.replace(/\n/g, '⏎')}`);
  console.log(`  题面期望 -> ${expect.replace(/\n/g, '⏎')}   ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) { badTotal++; process.exitCode = 1; }
}

// ---------- ② 随机对拍：下标直取 vs 从头数人头 ----------
console.log('--- 随机对拍：a[i] vs 从头数第 i 个 ---');
const N = 200000;
let firstBad = null;
for (let t = 0; t < N; t++) {
  const n = rnd(1, 30);
  const m = rnd(1, 8);
  const ids = Array.from({ length: n }, () => rnd(1, 1e9)); // 学号值域 1..1e9
  const queries = Array.from({ length: m }, () => rnd(1, n)); // 1 <= i <= n
  const f = fast(ids, queries), b = brute(ids, queries);
  if (f.join(',') !== b.join(',')) {
    badTotal++;
    if (!firstBad) firstBad = { n, m, ids, queries, f, b, input: toInput(n, m, ids, queries) };
  }
}
if (firstBad) {
  console.log('第一组不一致：');
  console.log(firstBad.input);
  console.log(`下标直取 = ${firstBad.f.join(',')}  从头数 = ${firstBad.b.join(',')}`);
  fs.writeFileSync('badcase.txt', firstBad.input);
} else {
  console.log(`共 ${N} 组，不一致 0 组`);
}

// ---------- ③ 定向边界 ----------
console.log('--- 定向边界 ---');
const edges = [
  ['n=1 只问自己', [42], [1]],
  ['问第 1 个', [7, 8, 9], [1]],
  ['问最后一个（下标越界高发点）', [7, 8, 9], [3]],
  ['同一个 i 反复问', [7, 8, 9], [2, 2, 2]],
  ['学号取到 1e9 上界', [1, 1000000000], [2]],
  ['学号取到 1 下界', [1, 1], [1, 2]],
  ['逆序询问', [10, 20, 30, 40], [4, 1, 3, 2]],
];
for (const [name, ids, queries] of edges) {
  const f = fast(ids, queries).join(',');
  const b = brute(ids, queries).join(',');
  const ok = f === b && !f.includes(',-1');
  console.log(`${name}: 期望按 1-based 读出 -> ${f} ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) { badTotal++; process.exitCode = 1; }
}

// ---------- ④ 满规模压力（只跑正解路径，验证 2e6+1e5 不超时） ----------
{
  const n = 2000000, m = 100000;
  const a = new Array(n + 1);
  for (let i = 1; i <= n; i++) a[i] = (i * 2654435761) % 1000000000 + 1;
  const t0 = Date.now();
  let s = 0;
  for (let q = 0; q < m; q++) s += a[rnd(1, n)];   // 纯随机访问 1e5 次
  console.log(`--- 压力：n=2e6 建表 + 1e5 次随机下标访问，JS 侧耗时 ${Date.now() - t0}ms（校验和 ${s % 1000000007}）`);
}

// ---------- ⑤ 真·C++ 程序逐组复核 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0;
  const CN = 300;
  for (let t = 0; t < CN; t++) {
    const n = rnd(1, 60), m = rnd(1, 20);
    const ids = Array.from({ length: n }, () => rnd(1, 1e9));
    const queries = Array.from({ length: m }, () => rnd(1, n));
    const out = execFileSync(exe, { input: toInput(n, m, ids, queries), encoding: 'utf8' })
      .trim().split(/\s+/).map(Number);
    const b = brute(ids, queries);
    if (out.join(',') !== b.join(',')) {
      cbad++;
      if (cbad === 1) { console.log('C++ 不一致，数据已写到 badcase_cpp.txt'); fs.writeFileSync('badcase_cpp.txt', toInput(n, m, ids, queries)); }
    }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${CN} 组，不一致 ${cbad} 组`);
  if (cbad) { badTotal += cbad; process.exitCode = 1; }
}

console.log(badTotal === 0 ? '=== 全部通过：0 处不一致 ===' : `=== 有 ${badTotal} 处不一致 ===`);
