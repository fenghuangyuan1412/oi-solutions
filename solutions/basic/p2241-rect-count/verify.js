// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                     （默认跑同目录 sol.exe，*.exe 已被 .gitignore 忽略）
// 参照实现走的是闭式公式（BigInt 精确整数），与题解"按尺寸枚举"的思路完全不同，可当第二意见。
//   正方形：边长 k 有 (n+1-k)(m+1-k) 个，k=1..min(n,m) 求和
//           Σ (n+1-k)(m+1-k) = s(n+1)(m+1) - (n+m+2)·s(s+1)/2 + s(s+1)(2s+1)/6
//           其中 n+m+2 = (n+1)+(m+1)
//   矩形总数：C(n+1,2)·C(m+1,2) = n(n+1)/2 · m(m+1)/2
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) { console.error(`找不到可执行文件 ${EXE}\n请先在本目录编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));
const rnd = (k) => Math.floor(Math.random() * k);

function ref(n, m) {
  const s = BigInt(Math.min(n, m)), A = BigInt(n + 1), B = BigInt(m + 1);
  const sq = s * A * B - (A + B) * s * (s + 1n) / 2n + s * (s + 1n) * (2n * s + 1n) / 6n;
  const total = BigInt(n) * BigInt(n + 1) / 2n * (BigInt(m) * BigInt(m + 1) / 2n);
  return `${sq} ${total - sq}`;
}

{
  const got = run('2 3\n');
  console.log(`官方样例 2 3：期望 8 10 实际 ${got} ${got === '8 10' ? '✅' : '❌'}`);
  if (got !== '8 10') process.exitCode = 1;
}

let bad = 0;
const ROUNDS = Number(process.env.ROUNDS || 400);
for (let t = 0; t < ROUNDS; t++) {
  const n = 1 + rnd(60), m = 1 + rnd(60);
  const got = run(`${n} ${m}\n`), want = ref(n, m);
  if (got !== want) { bad++; if (bad === 1) console.log(`首个不一致：n=${n} m=${m} 期望 ${want} 实际 ${got}`); }
}
console.log(`P2241 与闭式公式（BigInt）对拍 ${ROUNDS} 组（n,m<=60），不一致 ${bad}`);
if (bad) process.exitCode = 1;

// 再拍一组大范围（BigInt 仍是真值），确认 long long 没有溢出
{
  let bad2 = 0;
  for (let t = 0; t < 40; t++) {
    const n = 1 + rnd(5000), m = 1 + rnd(5000);
    if (run(`${n} ${m}\n`) !== ref(n, m)) bad2++;
  }
  console.log(`P2241 与闭式公式对拍 40 组（n,m<=5000，答案可达 1.5e14），不一致 ${bad2}`);
  if (bad2) process.exitCode = 1;
}

// 极限规模计时
{
  const inp = '5000 5000\n';
  const ts = [];
  let out = '';
  for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(inp); ts.push(Date.now() - t0); }
  console.log(`极限 5000x5000：输出 ${out}（真值 ${ref(5000, 5000)}），耗时 ${ts.join('/') } ms`);
}
