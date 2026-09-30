// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                     （默认跑同目录 sol.exe，*.exe 已被 .gitignore 忽略）
// 三件事：① 官方样例；② 与"数组 shift 模拟 FIFO"的参照实现对拍；
// ③ 把最常见的误读 —— 按 LRU（最久未使用）淘汰 —— 也算一遍，说明两种读法在官方样例上就不同。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) { console.error(`找不到可执行文件 ${EXE}\n请先在本目录编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

const SAMPLE_IN = '3 7\n1 2 1 5 4 4 1';
const SAMPLE_OUT = '5';
{
  const got = run(SAMPLE_IN + '\n');
  console.log(`官方样例：期望 ${SAMPLE_OUT} 实际 ${got} ${got === SAMPLE_OUT ? '✅' : '❌'}`);
  if (got !== SAMPLE_OUT) process.exitCode = 1;
}

// 参照一：题面原话"把最早的那个单词删去" ⇒ 先进先出，命中不动位置
function refFIFO(M, words) {
  const mem = [];
  let cnt = 0;
  for (const w of words) {
    if (mem.includes(w)) continue;                 // 命中：不查词典，内存内容也不变
    cnt++;
    if (mem.length === M) mem.shift();             // 满了：淘汰最先进来的
    mem.push(w);
  }
  return String(cnt);
}

// 参照二：误读成 LRU（命中就把该词挪到最新）——只用来对比，不是正确做法
function refLRU(M, words) {
  const mem = [];
  let cnt = 0;
  for (const w of words) {
    const at = mem.indexOf(w);
    if (at >= 0) { mem.splice(at, 1); mem.push(w); continue; }
    cnt++;
    if (mem.length === M) mem.shift();
    mem.push(w);
  }
  return String(cnt);
}

const words7 = [1, 2, 1, 5, 4, 4, 1];
console.log(`官方样例上：FIFO（题面）= ${refFIFO(3, words7)}，LRU（误读）= ${refLRU(3, words7)} —— 两者不同，说明读错题当场就会暴露`);

let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 800);
for (let t = 0; t < ROUNDS; t++) {
  const M = 1 + rnd(6), N = 1 + rnd(40);
  const w = [];
  for (let i = 0; i < N; i++) w.push(rnd(8));
  const inp = `${M} ${N}\n${w.join(' ')}\n`;
  const want = refFIFO(M, w), got = run(inp);
  if (got !== want) { bad++; if (bad === 1) console.log('首个不一致:\n' + inp + '期望 ' + want + ' 实际 ' + got); }
}
console.log(`P1540 与 FIFO 参照对拍 ${ROUNDS} 组（M<=6, N<=40, 词号<=7），不一致 ${bad}`);
if (bad) process.exitCode = 1;

// 顺带统计：随机数据上 FIFO 与 LRU 的差异频率（说明"读错题"不是小事）
{
  let diff = 0;
  for (let t = 0; t < 2000; t++) {
    const M = 1 + rnd(6), N = 1 + rnd(40);
    const w = [];
    for (let i = 0; i < N; i++) w.push(rnd(8));
    if (refFIFO(M, w) !== refLRU(M, w)) diff++;
  }
  console.log(`2000 组随机数据里 FIFO 与 LRU 结果不同 ${diff} 组（${(diff / 20).toFixed(1)}%）`);
}

// 极限规模：M=100, N=1000（题面上限），应瞬时完成
{
  const M = 100, N = 1000;
  const w = [];
  for (let i = 0; i < N; i++) w.push(rnd(1001));
  const inp = `${M} ${N}\n${w.join(' ')}\n`;
  const t0 = Date.now();
  const got = run(inp);
  console.log(`极限 M=100 N=1000：输出 ${got}，耗时 ${Date.now() - t0} ms（参照 ${refFIFO(M, w)}）`);
}
