// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                     （默认跑同目录 sol.exe，*.exe 已被 .gitignore 忽略）
//   node verify.js 路径/别的.exe
// 参照实现不复用题解的"累加器/偏移数组"写法：对每个格子，把 8 个邻居方向一个个显式数一遍。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) { console.error(`找不到可执行文件 ${EXE}\n请先在本目录编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

const SAMPLES = [
  { in: '3 3\n*??\n???\n?*?', out: '*10\n221\n1*1' },
  { in: '2 3\n?*?\n*??', out: '2*1\n*21' },
];
for (const s of SAMPLES) {
  const got = run(s.in);
  console.log(`样例 ${JSON.stringify(s.in.split('\n')[0])} 期望 ${JSON.stringify(s.out)} 实际 ${JSON.stringify(got)} ${got === s.out ? '✅' : '❌'}`);
  if (got !== s.out) process.exitCode = 1;
}

function refCount(grid, n, m) {
  const out = [];
  for (let i = 0; i < n; i++) {
    let row = '';
    for (let j = 0; j < m; j++) {
      if (grid[i][j] === '*') { row += '*'; continue; }          // 雷格原样输出 *，不算数字
      let cnt = 0;
      for (let di = -1; di <= 1; di++) for (let dj = -1; dj <= 1; dj++) {
        if (di === 0 && dj === 0) continue;
        const ni = i + di, nj = j + dj;
        if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] === '*') cnt++;
      }
      row += cnt;
    }
    out.push(row);
  }
  return out.join('\n');
}

let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 600);
for (let t = 0; t < ROUNDS; t++) {
  const n = 1 + rnd(9), m = 1 + rnd(9);
  const g = [];
  let inp = `${n} ${m}\n`;
  for (let i = 0; i < n; i++) { let r = ''; for (let j = 0; j < m; j++) r += Math.random() < 0.4 ? '*' : '?'; g.push(r); inp += r + '\n'; }
  const want = refCount(g, n, m), got = run(inp);
  if (got !== want) { bad++; if (bad === 1) console.log('首个不一致:\n' + inp + '\n期望\n' + want + '\n实际\n' + got); }
}
console.log(`P2670 与逐格八方向计数参照对拍 ${ROUNDS} 组（n,m<=9），不一致 ${bad}`);
if (bad) process.exitCode = 1;

// 极限规模 100x100：题面上限
{
  const n = 100, m = 100;
  let inp = `${n} ${m}\n`;
  for (let i = 0; i < n; i++) { let r = ''; for (let j = 0; j < m; j++) r += Math.random() < 0.5 ? '*' : '?'; inp += r + '\n'; }
  const t0 = Date.now();
  run(inp);
  console.log(`极限 100x100：耗时 ${Date.now() - t0} ms`);
}
