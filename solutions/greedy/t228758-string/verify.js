// T228758 对拍：贪心 O(n) vs 暴力枚举所有 (s1', s2') 前缀对
// 用法：node verify.js            —— 跑官方样例 + 随机对拍
//       node verify.js --exe PATH —— 额外把 PATH 里的真·C++ 程序逐组跑一遍（300 组）
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// 字典序比较：JS 的 < 就是按 UTF-16 码位逐位比，纯小写英文字母时与 C++ string 的 < 完全一致
function brute(s1, s2) {
  let best = null;
  for (let i = 1; i <= s1.length; i++) {
    for (let j = 1; j <= s2.length; j++) {
      const t = s1.slice(0, i) + s2.slice(0, j);
      if (best === null || t < best) best = t;
    }
  }
  return best;
}

function greedy(s1, s2) {
  const c = s2[0];
  let k = 1;
  while (k < s1.length && s1[k] < c) k++;
  return s1.slice(0, k) + c;
}

function rndStr(minLen, maxLen, alphabet) {
  const n = minLen + Math.floor(Math.random() * (maxLen - minLen + 1));
  let s = '';
  for (let i = 0; i < n; i++) s += alphabet[Math.floor(Math.random() * alphabet.length)];
  return s;
}

// ---------- 官方样例 ----------
const samples = [['happy birthday', 'hab'], ['abc defg', 'abcd']];
console.log('--- 官方样例（题面截图）---');
for (const [input, exp] of samples) {
  const [s1, s2] = input.split(' ');
  const g = greedy(s1, s2), b = brute(s1, s2);
  console.log(`${input} -> 贪心=${g} 暴力=${b} 期望=${exp} ${g === exp && b === exp ? 'OK' : 'FAIL'}`);
}

// ---------- 随机对拍（贪心 vs 暴力）----------
console.log('--- 随机对拍：贪心 vs 全前缀枚举暴力 ---');
const alphabets = ['ab', 'abc', 'abcdefghijklmnopqrstuvwxyz'];
let bad = 0, tested = 0, worst = 0;
for (let t = 0; t < 200000; t++) {
  const ab = alphabets[t % 3];
  const s1 = rndStr(1, 8, ab);
  const s2 = rndStr(1, 6, ab);
  tested++;
  const g = greedy(s1, s2), b = brute(s1, s2);
  if (g.length > worst) worst = g.length;
  if (g !== b) {
    bad++;
    if (bad <= 5) console.log(`不一致 s1=${s1} s2=${s2} 贪心=${g} 暴力=${b}`);
  }
}
console.log(`共 ${tested} 组，不一致 ${bad} 组`);

// ---------- 定向边界 ----------
console.log('--- 定向边界 ---');
const edge = [
  ['a', 'a'], ['a', 'z'], ['z', 'a'], ['aa', 'a'], ['ab', 'b'], ['ba', 'b'],
  ['happy', 'b'], ['hboy', 'a'], ['zzz', 'z'], ['mabc', 'm'], ['abca', 'b'],
];
for (const [s1, s2] of edge) {
  const g = greedy(s1, s2), b = brute(s1, s2);
  console.log(`s1=${s1} s2=${s2} 贪心=${g} 暴力=${b} ${g === b ? 'OK' : 'FAIL'}`);
}

// ---------- 长串压力（只跑贪心，验证 1e5 规模不超时）----------
{
  const s1 = rndStr(1, 100000, 'abcdefghijklmnopqrstuvwxyz');
  const s2 = rndStr(1, 100000, 'abcdefghijklmnopqrstuvwxyz');
  const t0 = Date.now();
  const g = greedy(s1, s2);
  console.log(`--- 压力：|s1|=|s2|=1e5，答案长度 ${g.length}，耗时 ${Date.now() - t0}ms`);
}

// ---------- 真·C++ 程序逐组复核 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0, cn = 0;
  for (let t = 0; t < 300; t++) {
    const ab = alphabets[t % 3];
    const s1 = rndStr(1, 8, ab);
    const s2 = rndStr(1, 6, ab);
    const out = execFileSync(exe, { input: s1 + ' ' + s2 + '\n', encoding: 'utf8' }).trim();
    const b = brute(s1, s2);
    cn++;
    if (out !== b) { cbad++; if (cbad <= 5) console.log(`C++ 不一致 s1=${s1} s2=${s2} 程序=${out} 暴力=${b}`); }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${cn} 组，不一致 ${cbad} 组`);
}
