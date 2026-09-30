// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                     （默认跑同目录 sol.exe，*.exe 已被 .gitignore 忽略）
// 参照实现刻意不用"一次取模跳转"，而是 s 次逐格挪，把方向换算独立写一遍。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) { console.error(`找不到可执行文件 ${EXE}\n请先在本目录编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

const SAMPLES = [
  { in: '7 3\n0 singer\n0 reader\n0 mengbier\n1 thinker\n1 archer\n0 writer\n1 mogician\n0 3\n1 1\n0 2', out: 'writer' },
  { in: '10 10\n1 C\n0 r\n0 P\n1 d\n1 e\n1 m\n1 t\n1 y\n1 u\n0 V\n1 7\n1 1\n1 4\n0 5\n0 3\n0 1\n1 6\n1 2\n0 8\n0 4', out: 'y' },
];
for (const s of SAMPLES) {
  const got = run(s.in);
  console.log(`样例：期望 ${s.out} 实际 ${got} ${got === s.out ? '✅' : '❌'}`);
  if (got !== s.out) process.exitCode = 1;
}

// 输入约定：小人按"逆时针"顺序给出，下标 +1 就是逆时针挪一格。
// 面向圆心（dir=0）时，他的左手边是顺时针 ⇒ 下标 -1；面向外（dir=1）时左手边是逆时针 ⇒ 下标 +1。
function refWalk(people, cmds, n) {
  let cur = 0;
  for (const [a, s] of cmds) {
    const inward = people[cur].dir === 0;
    const leftward = a === 0;
    const stepSign = inward ? (leftward ? -1 : +1) : (leftward ? +1 : -1);
    for (let k = 0; k < s; k++) cur = (cur + stepSign + n) % n;   // 逐格走，不用闭式跳转
  }
  return people[cur].job;
}

let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 800);
const jobs = 'abcdefgh';
for (let t = 0; t < ROUNDS; t++) {
  const n = 2 + rnd(9), m = 1 + rnd(9);
  const people = [];
  let inp = `${n} ${m}\n`;
  for (let i = 0; i < n; i++) { const d = rnd(2), jb = jobs[rnd(jobs.length)]; people.push({ dir: d, job: jb }); inp += `${d} ${jb}\n`; }
  const cmds = [];
  for (let i = 0; i < m; i++) { const a = rnd(2), s = 1 + rnd(n - 1); cmds.push([a, s]); inp += `${a} ${s}\n`; }
  const want = refWalk(people, cmds, n), got = run(inp);
  if (got !== want) { bad++; if (bad === 1) console.log('首个不一致:\n' + inp + '期望 ' + want + ' 实际 ' + got); }
}
console.log(`P1563 与逐格走的参照对拍 ${ROUNDS} 组（n<=10, m<=9），不一致 ${bad}`);
if (bad) process.exitCode = 1;

// 极限规模：n=m=1e5，s 全取 n-1（最坏 1e10 步的"逐格模拟"会炸，闭式跳转应当瞬时）
{
  const n = 100000, m = 100000;
  let inp = `${n} ${m}\n`;
  for (let i = 0; i < n; i++) inp += `${rnd(2)} job${i}\n`;
  for (let i = 0; i < m; i++) inp += `${rnd(2)} ${n - 1}\n`;
  const t0 = Date.now();
  const got = run(inp);
  console.log(`极限 n=m=1e5（最坏指令长度）：输出 ${got}，耗时 ${Date.now() - t0} ms`);
}
