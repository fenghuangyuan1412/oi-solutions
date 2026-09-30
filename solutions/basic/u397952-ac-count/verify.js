// U397952 对拍：一遍扫描 O(n) vs 双重循环枚举下标对 O(n^2)
// 用法：node verify.js            —— 官方样例 + 随机对拍
//       node verify.js --exe PATH —— 额外把 PATH 里的真·C++ 程序逐组跑一遍（400 组）
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

function brute(s) {  // 定义直译：数有多少对 (i<j) 满足 s[i]='A' 且 s[j]='C'
  let ans = 0;
  for (let i = 0; i < s.length; i++)
    for (let j = i + 1; j < s.length; j++)
      if (s[i] === 'A' && s[j] === 'C') ans++;
  return String(ans);
}

function fast(s) {  // solution.cpp 的等价移植
  let cntA = 0, ans = 0;
  for (const ch of s) {
    if (ch === 'A') cntA++;
    else if (ch === 'C') ans += cntA;
  }
  return String(ans);
}

function rndStr(n, alphabet) {
  let s = '';
  for (let i = 0; i < n; i++) s += alphabet[Math.floor(Math.random() * alphabet.length)];
  return s;
}

console.log('--- 官方样例（题面截图）---');
for (const [s, exp] of [['ACCA', '2'], ['CA', '0'], ['CAAC', '2']]) {
  console.log(`${s} -> 一遍扫描=${fast(s)} 暴力=${brute(s)} 期望=${exp} ${fast(s) === exp && brute(s) === exp ? 'OK' : 'FAIL'}`);
}

console.log('--- 随机对拍：一遍扫描 vs 双重循环 ---');
const alphabets = ['AC', 'ACG', 'ACGT', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'];
let bad = 0, tested = 0;
for (let t = 0; t < 200000; t++) {
  const ab = alphabets[t % 4];
  const s = rndStr(1 + Math.floor(Math.random() * 30), ab);
  tested++;
  const f = fast(s), b = brute(s);
  if (f !== b) {
    bad++;
    if (bad <= 5) console.log(`不一致 s=${s} 扫描=${f} 暴力=${b}`);
  }
}
console.log(`共 ${tested} 组，不一致 ${bad} 组`);

console.log('--- 定向边界 ---');
for (const s of ['A', 'C', 'AC', 'CA', 'AA', 'CC', 'ACACAC', 'CCCCAAAA', 'AAAACCCC', 'B', 'ACB']) {
  console.log(`${s.padEnd(9)} 扫描=${fast(s)} 暴力=${brute(s)} ${fast(s) === brute(s) ? 'OK' : 'FAIL'}`);
}

console.log('--- 最坏值（|S|=1e4，前 5000 个 A 后 5000 个 C）---');
{
  const s = 'A'.repeat(5000) + 'C'.repeat(5000);
  const v = fast(s);
  console.log(`答案 = ${v}，5000*5000 = ${5000 * 5000}，int 上限 2147483647，${v < 2147483647 ? '不溢出' : '溢出'}`);
}

const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0, cn = 0;
  for (let t = 0; t < 400; t++) {
    const ab = alphabets[t % 4];
    const s = rndStr(1 + Math.floor(Math.random() * 40), ab);
    const out = execFileSync(exe, { input: s + '\n', encoding: 'utf8' }).trim();
    const b = brute(s);
    cn++;
    if (out !== b) { cbad++; if (cbad <= 5) console.log(`C++ 不一致 s=${s} 程序=${out} 暴力=${b}`); }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${cn} 组，不一致 ${cbad} 组`);
}
