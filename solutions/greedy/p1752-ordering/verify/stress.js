#!/usr/bin/env node
'use strict';
/* 对拍驱动（仅验证用，非讲解代码）。
 * 用法：node stress.js <sol.exe> <brute.exe> [组数=5000] [最大人数=6] [最大菜数=8]
 * 逻辑：内置 JS 生成器随机出小数据（值域刻意压小，制造大量"恰好等于下限/上限"的边界），
 *       分别喂给正解与网络流暴力，逐字节比较输出；不一致则打印该组并以退出码 1 结束。
 */
const { spawnSync } = require('child_process');
const fs = require('fs');
const os = require('os');
const path = require('path');

const argv = process.argv.slice(2);
const solExe = argv[0], bruteExe = argv[1];
const rounds = parseInt(argv[2] || '5000', 10);
const maxN = parseInt(argv[3] || '6', 10);
const maxM = parseInt(argv[4] || '8', 10);

if (!solExe || !bruteExe) {
  console.error('用法: node stress.js <sol.exe> <brute.exe> [组数] [最大人数] [最大菜数]');
  process.exit(2);
}

const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'p1752-stress-'));
const inPath = path.join(dir, 'in.txt');

function randInt(lo, hi) { return lo + Math.floor(Math.random() * (hi - lo + 1)); }

function genData() {
  const n = randInt(1, maxN), m = randInt(1, maxM);
  let p, q;
  const shape = Math.random();
  if (shape < 0.15) { p = n; q = 0; }                       // 全挑剔
  else if (shape < 0.30) { p = 0; q = n; }                  // 全贫穷
  else if (shape < 0.55) { p = randInt(1, n); q = n - p; }  // 无普通人
  else { p = randInt(0, n); q = randInt(0, n - p); }        // 有普通人
  // 值域压小：约 1/3 的组值域只有 0..2，逼出并列、恰好等于、谁都吃不起等边界
  const vmax = Math.random() < 0.33 ? 2 : (Math.random() < 0.6 ? 5 : 20);
  const lines = [`${n} ${m} ${p} ${q}`];
  for (let j = 0; j < m; j++) lines.push(`${randInt(0, vmax)} ${randInt(0, vmax)}`);
  const low = [], cap = [];
  for (let i = 0; i < p; i++) low.push(randInt(0, vmax));
  for (let i = 0; i < q; i++) cap.push(randInt(0, vmax));
  lines.push(low.join(' '));
  lines.push(cap.join(' '));
  return lines.join('\n') + '\n';
}

function run(exe, input) {
  const r = spawnSync(exe, [], { input, encoding: 'utf8', maxBuffer: 1 << 26 });
  if (r.error) throw r.error;
  return (r.stdout || '').trim();
}

let bad = 0;
for (let k = 0; k < rounds; k++) {
  const data = genData();
  fs.writeFileSync(inPath, data);
  const a = run(solExe, data);
  const b = run(bruteExe, data);
  if (a !== b) {
    bad++;
    console.log(`不一致 第 ${k + 1} 组：\n${data}正解输出: ${a}\n暴力输出: ${b}`);
    if (bad >= 3) break;
  }
}
console.log(`共 ${rounds} 组，不一致 ${bad} 组${bad ? '（详见上方）' : ''}`);
process.exit(bad ? 1 : 0);
