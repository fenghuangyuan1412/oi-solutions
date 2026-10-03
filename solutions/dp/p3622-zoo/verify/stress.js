#!/usr/bin/env node
'use strict';
/* 对拍驱动（仅验证用，非讲解代码）。
 * 用法：node stress.js <sol.exe> <brute.exe> [组数=1200] [最大N=12] [最大C=8]
 * 生成器保证题面约束：X/Y 两两不同且都落在这个小朋友可见的 5 连围栏内。
 * 不一致则打印该组并以退出码 1 结束。
 */
const { spawnSync } = require('child_process');
const fs = require('fs');
const os = require('os');
const path = require('path');

const argv = process.argv.slice(2);
const solExe = argv[0], bruteExe = argv[1];
const rounds = parseInt(argv[2] || '1200', 10);
const maxN = parseInt(argv[3] || '12', 10);
const maxC = parseInt(argv[4] || '8', 10);
if (!solExe || !bruteExe) { console.error('用法: node stress.js <sol> <brute> [组数] [最大N] [最大C]'); process.exit(2); }

const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'p3622-stress-'));
const inPath = path.join(dir, 'in.txt');
const randInt = (lo, hi) => lo + Math.floor(Math.random() * (hi - lo + 1));

function genData() {
  const n = randInt(10, maxN), c = randInt(1, maxC);
  const lines = [`${n} ${c}`];
  for (let i = 0; i < c; i++) {
    const E = randInt(1, n);
    const win = Array.from({ length: 5 }, (_, k) => ((E - 1 + k) % n) + 1);
    const f = randInt(0, 3), l = randInt(0, Math.min(3, 5 - f)); // 题面：X/Y 都在 5 连窗口内且两两不同 ⇒ f+l ≤ 5
    const slots = [0, 1, 2, 3, 4].sort(() => Math.random() - 0.5).slice(0, f + l);
    const X = slots.slice(0, f).map(k => win[k]).sort((a, b) => a - b);
    const Y = slots.slice(f).map(k => win[k]).sort((a, b) => a - b);
    lines.push([E, f, l, ...X, ...Y].join(' '));
  }
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
  const a = run(solExe, data), b = run(bruteExe, data);
  if (a !== b) {
    bad++;
    console.log(`不一致 第 ${k + 1} 组：\n${data}正解输出: ${a}\n暴力输出: ${b}`);
    if (bad >= 3) break;
  }
}
console.log(`共 ${rounds} 组，不一致 ${bad} 组${bad ? '（详见上方）' : ''}`);
process.exit(bad ? 1 : 0);
