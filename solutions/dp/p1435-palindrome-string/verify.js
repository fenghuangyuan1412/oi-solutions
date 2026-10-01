// P1435 回文字串 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js                —— 官方样例 + JS 双实现随机对拍 400 组
//       node verify.js --exe PATH     —— 改成把编译好的 C++ 程序当正解，与 l - LCS 参考实现交叉对拍 400 组
//       node verify.js --build        —— 先 g++ -static -O2 -std=c++14 编译 solution.cpp 再跑
const { execFileSync } = require('child_process');
const path = require('path');

const SAMPLE_IN = 'Ab3bd\n';
const SAMPLE_OUT = '2';

// 正解思路：区间 DP，两端相等取 f[i+1][j-1]，否则 min(f[i+1][j], f[i][j-1]) + 1（与 solution.cpp 同一递推，独立实现）
// 只依赖更短的区间 ⇒ 按长度从小到大填表；长度 <= 1 的区间天然是 0
function intervalDP(s) {
  const L = s.length;
  const f = Array.from({ length: L }, () => new Array(L).fill(0));   // 下三角的 0 就是"空区间"的 0，和 C++ 全局零初始化等价
  for (let len = 2; len <= L; len++)
    for (let i = 0; i + len - 1 < L; i++) {
      const j = i + len - 1;
      if (s[i] === s[j]) f[i][j] = f[i + 1][j - 1];                 // 两端配对免费；len == 2 时读到的正是空区间 0
      else f[i][j] = Math.min(f[i + 1][j], f[i][j - 1]) + 1;
    }
  return L ? f[0][L - 1] : 0;
}

// 暴力思路：另一个独立结论 —— 最少插入数 = l - LCS(s, reverse(s))（区分大小写，同一份字符集逐字符比较）
function lcsSolve(s) {
  const L = s.length;
  const t = s.split('').reverse().join('');
  const g = Array.from({ length: L + 1 }, () => new Array(L + 1).fill(0));
  for (let i = 1; i <= L; i++)
    for (let j = 1; j <= L; j++)
      g[i][j] = s[i - 1] === t[j - 1] ? g[i - 1][j - 1] + 1 : Math.max(g[i - 1][j], g[i][j - 1]);
  return L - g[L][L];
}

let seed = 20261001;
const rnd = () => ((seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296);
const ri = (a, b) => a + Math.floor(rnd() * (b - a + 1));

const ALPHABET = 'abAB3';      // 含大小写同形字符，专门压"区分大小写"这条题意
function genCase() {
  const L = ri(1, 11);
  let s = '';
  for (let i = 0; i < L; i++) s += ALPHABET[ri(0, ALPHABET.length - 1)];
  return s;
}
const strip = (x) => String(x).replace(/[\r\s]+$/g, '');

const exeIdx = process.argv.indexOf('--exe');
const exe = exeIdx > 0 ? path.resolve(process.argv[exeIdx + 1]) : null;
if (process.argv.includes('--build')) {
  execFileSync('g++', ['-static', '-O2', '-std=c++14', path.join(__dirname, 'solution.cpp'), '-o', path.join(__dirname, 'solution.exe')]);
}
const runExe = (input) => strip(execFileSync(exe, [], { input }));
const solve = (s) => (exe ? runExe(s + '\n') : String(intervalDP(s)));

// —— 官方样例 ——
if (exe) {
  const got = runExe(SAMPLE_IN);
  console.log('官方样例：C++ 程序输出 [' + got + ']，期望 [' + SAMPLE_OUT + '] ' + (got === SAMPLE_OUT ? 'OK' : 'MISMATCH'));
  if (got !== SAMPLE_OUT) process.exit(1);
}

const ROUNDS = Number(process.argv.find((a) => /^--rounds=\d+$/.test(a))?.split('=')[1] || 400);
let bad = 0;
for (let t = 0; t < ROUNDS; t++) {
  const s = genCase();
  const want = lcsSolve(s);
  const got = solve(s);
  if (String(want) !== got) {
    bad++;
    if (bad <= 3) console.log("MISMATCH s='" + s + "' l-LCS=" + want + ' got=' + got);
  }
}
console.log('--- ' + (exe ? 'C++ solution.exe' : 'JS 区间 DP') + " vs l - LCS(s, reverse(s)) 参考实现 ---");
console.log('共 ' + ROUNDS + ' 组（串长 <=11，字母表 abAB3），不一致 ' + bad + ' 组');

// —— 定向边界：单字符、已是回文、全无配对、大小写不同形 ——
const EDGE = [
  { s: 'a', want: 0, why: '单字符天然是回文' },
  { s: 'aa', want: 0, why: '两端相等直接取空区间的 0（len == 2 的分支）' },
  { s: 'ab', want: 1, why: '两个不同字符，补 1 个' },
  { s: 'aA', want: 1, why: '区分大小写：a 与 A 不算相同字符' },
  { s: 'abc', want: 2, why: '三个互不相同的字符要补 2 个' },
  { s: 'abba', want: 0, why: '已经是回文' },
  { s: 'abca', want: 1, why: '两端相等白捡中间的 f[1][2] = 1' },
  { s: 'aab', want: 1, why: '补一个 b 到最前面成 baab' },
  { s: 'Ab3bd', want: 2, why: '官方样例' },
];
let edgeBad = 0;
for (const e of EDGE) {
  const got = solve(e.s);
  if (String(e.want) !== got) { edgeBad++; console.log('EDGE FAIL ' + e.s + ' (' + e.why + ') 期望 ' + e.want + ' 得 ' + got); }
}
console.log('定向边界 ' + EDGE.length + ' 项，失败 ' + edgeBad + ' 项');
