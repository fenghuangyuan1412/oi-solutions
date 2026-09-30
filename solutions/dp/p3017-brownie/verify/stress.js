#!/usr/bin/env node
'use strict';
/* 对拍驱动（仅验证用，非讲解代码）。
 * 用法：
 *   node stress.js <sol.exe> <brute.exe> <gen.py> [组数=2000] [最小规模=1] [--use-gen]
 * 逻辑：随机生成小规模数据 -> 分别喂给正解与暴力 -> 逐字节比较输出。
 * 不一致则打印该组数据并以退出码 1 结束。
 * 默认用内置的 JS 生成器（快，进程开销小）；带 --use-gen 时改为每轮 spawn
 * `python gen.py <seed>`，用于复核生成器本身与数据格式一致。
 */
const { spawnSync } = require('child_process');
const fs = require('fs');
const os = require('os');
const path = require('path');

const argv = process.argv.slice(2);
const positional = argv.filter(s => !s.startsWith('--'));
const useGen = argv.includes('--use-gen');
const solExe = positional[0], bruteExe = positional[1], genPy = positional[2];
const rounds = parseInt(positional[3] || '2000', 10);
const minRC = parseInt(positional[4] || '1', 10);
const maxRC = parseInt(positional[5] || '6', 10);

if (!solExe || !bruteExe || !genPy || !Number.isFinite(rounds) || rounds <= 0) {
  console.error('用法: node stress.js <sol.exe> <brute.exe> <gen.py> [组数] [最小规模] [--use-gen]');
  process.exit(2);
}

const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'p3017-stress-'));
const inPath = path.join(dir, 'in.txt');

function randInt(lo, hi) { return lo + Math.floor(Math.random() * (hi - lo + 1)); }

function genData(seed) {
  const R = randInt(minRC, 6), C = randInt(minRC, 6);
  const A = randInt(1, R), B = randInt(1, C);
  const vmax = Math.random() < 0.25 ? 1 : 9;   // 掺一批 0/1 数据，逼出并列与全零
  const lines = [`${R} ${C} ${A} ${B}`];
  for (let i = 0; i < R; i++) {
    const row = [];
    for (let j = 0; j < C; j++) row.push(randInt(0, vmax));
    lines.push(row.join(' '));
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
  let data;
  if (useGen) {
    const g = spawnSync('python', [genPy, String(k * 7919 + 1)], { encoding: 'utf8', maxBuffer: 1 << 26 });
    if (g.error) throw g.error;
    data = g.stdout;
  } else {
    data = genData(k);
  }
  fs.writeFileSync(inPath, data);
  const a = run(solExe, data);
  const b = run(bruteExe, data);
  if (a !== b) {
    bad++;
    console.log('=== 不一致，第 ' + (k + 1) + ' 组 ===');
    console.log(data);
    console.log('sol   = ' + a);
    console.log('brute = ' + b);
    break;
  }
}
fs.rmSync(dir, { recursive: true, force: true });
console.log(`共 ${rounds} 组，不一致 ${bad} 组`);
process.exit(bad ? 1 : 0);
