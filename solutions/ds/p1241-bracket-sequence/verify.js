// P1241 括号序列 对拍工具（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 正解：栈模拟（左括号入下标；右括号只看栈顶，类型不符则右括号作废且【栈顶不弹】）
// 暴力：完全按题面定义，用数组两两扫描找"左侧最近的未匹配左括号"
// 用法：node verify.js              —— 官方样例 + 随机对拍 200000 组
//       node verify.js --exe PATH   —— 额外用编译好的 C++ 程序复核 500 组
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const isOpen = c => c === '(' || c === '[';
const sameType = (a, b) => (a === '(' && b === ')') || (a === '[' && b === ']');

// 把"补全规则"套到 matched 标记上，得到答案串（正解/暴力共用）
function complete(s, matched) {
  let ans = '';
  for (let i = 0; i < s.length; i++) {
    const c = s[i];
    if (matched[i]) ans += c;
    else if (isOpen(c)) ans += c + (c === '(' ? ')' : ']'); // 未匹配左括号：右边补右
    else ans += (c === ')' ? '(' : '[') + c;                // 未匹配右括号：左边补左
  }
  return ans;
}

// 正解：栈
function solveStack(s) {
  const n = s.length, matched = new Array(n).fill(false), st = [];
  for (let i = 0; i < n; i++) {
    const c = s[i];
    if (isOpen(c)) st.push(i);
    else if (st.length && sameType(s[st[st.length - 1]], c)) {
      matched[st[st.length - 1]] = true; matched[i] = true; st.pop();
    }
    // 类型不符 / 栈空：右括号作废，栈顶不弹
  }
  return complete(s, matched);
}

// 暴力：不用栈，逐右括号向回线性扫描找最近未匹配左括号
function solveBrute(s) {
  const n = s.length;
  const matched = new Array(n).fill(false);
  const usedLeft = new Array(n).fill(false); // 该左括号是否已被配对
  for (let i = 0; i < n; i++) {
    if (isOpen(s[i])) continue;
    let j = i - 1;
    while (j >= 0) {
      if (isOpen(s[j]) && !usedLeft[j]) break; // 找到"左侧最近的未匹配左括号"
      j--;
    }
    if (j >= 0 && sameType(s[j], s[i])) { usedLeft[j] = true; matched[j] = true; matched[i] = true; }
    // 否则右括号作废（不回退去找更早的左括号）
  }
  return complete(s, matched);
}

function rndStr(len) {
  const alph = '()[]';
  let s = '';
  for (let i = 0; i < len; i++) s += alph[Math.floor(Math.random() * 4)];
  return s;
}

// ---------- 官方样例 ----------
console.log('--- 官方样例 ---');
const samples = [['([()', '()[]()'], ['([)', '()[]()']];
let sampleOK = true;
for (const [inp, exp] of samples) {
  const a = solveStack(inp), b = solveBrute(inp);
  const ok = a === exp && b === exp;
  if (!ok) sampleOK = false;
  console.log(`${inp} -> 栈=${a} 暴力=${b} 期望=${exp} ${ok ? 'OK' : 'FAIL'}`);
}

// ---------- 定向边界 ----------
console.log('--- 定向边界（含空串 / 全左 / 全右 / 交错）---');
const edge = ['', '(', ')', '[]', '([)]', '(]', '[(])', '(((', ')))', '[]()()', '()[()]', '(())'];
for (const s of edge) {
  const a = solveStack(s), b = solveBrute(s);
  console.log(`"${s}" -> 栈="${a}" 暴力="${b}" ${a === b ? 'OK' : 'FAIL'}`);
}

// ---------- 随机对拍：栈贪心 vs 数组扫描暴力 ----------
console.log('--- 随机对拍：栈 vs 暴力 ---');
let bad = 0, tested = 0, firstBad = null;
for (let t = 0; t < 200000; t++) {
  const s = rndStr(Math.floor(Math.random() * 13)); // 长度 0..12
  tested++;
  const a = solveStack(s), b = solveBrute(s);
  if (a !== b) { bad++; if (!firstBad) firstBad = { s, a, b }; }
}
if (firstBad) console.log(`第一组不一致 输入="${firstBad.s}" 栈="${firstBad.a}" 暴力="${firstBad.b}"`);
console.log(`共 ${tested} 组，不一致 ${bad} 组`);

// ---------- 真·C++ 程序逐组复核 ----------
const exeIdx = process.argv.indexOf('--exe');
if (exeIdx >= 0) {
  const exe = path.resolve(process.argv[exeIdx + 1]);
  if (!fs.existsSync(exe)) { console.log('找不到 exe：' + exe); process.exit(1); }
  let cbad = 0, cn = 0;
  for (let t = 0; t < 500; t++) {
    const s = rndStr(1 + Math.floor(Math.random() * 12));
    const out = execFileSync(exe, { input: s + '\n', encoding: 'utf8' }).replace(/\r?\n$/, '');
    const b = solveBrute(s);
    cn++;
    if (out !== b) { cbad++; if (cbad <= 5) console.log(`C++ 不一致 输入="${s}" 程序="${out}" 暴力="${b}"`); }
  }
  console.log(`--- C++ 程序 vs 暴力：共 ${cn} 组，不一致 ${cbad} 组`);
  if (cbad > 0) process.exit(1);
}

if (!sampleOK || bad > 0) process.exit(1);
