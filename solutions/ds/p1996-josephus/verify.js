// P1996 约瑟夫问题 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js             —— 官方样例 + 随机对拍 200000 组 + 定向边界
//       node verify.js --exe PATH  —— 额外把编译好的 C++ 程序逐组跑 300 组（含满规模 n=m=100）
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// 「正解」：队列模拟圈 —— 前 m-1 个人出队再入队到队尾，第 m 个人（队头）出圈
function queueSolve(n, m) {
  const q = [];
  for (let i = 1; i <= n; i++) q.push(i);
  const out = [];
  for (let k = 0; k < n; k++) {
    for (let t = 1; t < m; t++) q.push(q.shift());
    out.push(q.shift());
  }
  return out;
}

// 「暴力」：独立思路 —— 一圈编号摆成数组，vis[i] 标记是否已出圈；
// 每轮从当前位置往后一格一格走，跳过 vis 为真的人，数满 m 个活人就把他淘汰。
function markSolve(n, m) {
  const vis = new Array(n + 1).fill(false);
  const out = [];
  let pos = n;              // 记住"上一个被数到的人"；初值取 n，下一格恰好回绕到 1
  for (let k = 0; k < n; k++) {
    let cnt = 0;
    while (cnt < m) {
      pos = pos % n + 1;    // 环形前进一格：1..n 循环，走到 n 之后再走就回到 1
      if (!vis[pos]) cnt++; // 已出圈的人不占报数，直接跳过
    }
    // 循环退出时 pos 正好停在第 m 个活人身上（只有数到他才让 cnt 达到 m）
    vis[pos] = true;
    out.push(pos);
  }
  return out;
}

const rnd = (lo, hi) => lo + Math.floor(Math.random() * (hi - lo + 1));
let badTotal = 0;

// ---------- ① 官方样例 ----------
console.log('--- 官方样例 ---');
{
  const expect = '3 6 9 2 7 1 8 5 10 4';
  const a = queueSolve(10, 3).join(' ');
  const b = markSolve(10, 3).join(' ');
  const ok = a === expect && b === expect;
  console.log(`n=10 m=3  队列=${a}`);
  console.log(`          vis 数组暴力=${b}`);
  console.log(`          题面期望=${expect}   ${ok ? 'OK' : 'FAIL'}`);
  if (!ok) { badTotal++; process.exitCode = 1; }
}

// ---------- ② 随机对拍 ----------
console.log('--- 随机对拍：队列模拟 vs vis 数组暴力 ---');
const N = 200000;
let firstBad = null;
for (let t = 0; t < N; t++) {
  const n = rnd(1, 12), m = rnd(1, 12);
  const a = queueSolve(n, m), b = markSolve(n, m);
  if (a.join(',') !== b.join(',')) {
    badTotal++;
    if (!firstBad) firstBad = { n, m, a: a.join(','), b: b.join(',') };
  }
}
if (firstBad) {
  console.log(`第一组不一致 n=${firstBad.n} m=${firstBad.m} 队列=${firstBad.a} 暴力=${firstBad.b}`);
  fs.writeFileSync('badcase.txt', `${firstBad.n} ${firstBad.m}\n`);
} else {
  console.log(`共 ${N} 组（n,m ∈ 1..12），不一致 0 组`);
}

// ---------- ③ 定向边界 ----------
console.log('--- 定向边界 ---');
const edges = [
  [1, 1], [1, 100], [5, 1], [100, 1], [3, 3], [5, 5], [6, 7], [10, 100], [100, 100], [2, 100],
];
for (const [n, m] of edges) {
  const a = queueSolve(n, m), b = markSolve(n, m);
  const isPerm = a.length === n && new Set(a).size === n && a.every(v => v >= 1 && v <= n);
  const ok = a.join(',') === b.join(',') && isPerm;
  console.log(`n=${n} m=${m} -> ${a.join(' ')} ${ok ? 'OK' : 'FAIL(暴力=' + b.join(' ') + ')'}`);
  if (!ok) { badTotal++; process.exitCode = 1; }
}
{ // m=1 必须原序出圈，这是暴露 off-by-one 最快的边界
  const a = queueSolve(5, 1).join(' ');
  console.log(`m=1 应当不移动任何人：${a} ${a === '1 2 3 4 5' ? 'OK' : 'FAIL'}`);
  if (a !== '1 2 3 4 5') { badTotal++; process.exitCode = 1; }
}

// ---------- ④ 与真实 C++ 程序逐组对拍 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0;
  const CN = 300;
  for (let t = 0; t < CN; t++) {
    const n = rnd(1, 100), m = rnd(1, 100);          // 直接按题面满值域取
    const out = execFileSync(exe, { input: `${n} ${m}\n`, encoding: 'utf8' })
      .trim().split(/\s+/).map(Number);
    const b = markSolve(n, m);
    if (out.join(',') !== b.join(',')) {
      cbad++;
      if (cbad === 1) {
        console.log(`C++ 不一致 n=${n} m=${m} 程序=${out.join(' ')} 暴力=${b.join(' ')}`);
        fs.writeFileSync('badcase_cpp.txt', `${n} ${m}\n`);
      }
    }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${CN} 组（n,m ∈ 1..100），不一致 ${cbad} 组`);
  if (cbad) { badTotal += cbad; process.exitCode = 1; }
}

console.log(badTotal === 0 ? '=== 全部通过：0 处不一致 ===' : `=== 有 ${badTotal} 处不一致 ===`);
