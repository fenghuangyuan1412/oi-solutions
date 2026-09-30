// 【验算脚本，不是题解代码】题解一律看 solution.cpp / solution_full.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp      -o sol_lean.exe    // 骗分档：只保证 NO + 完全平方 Δ
//   g++ -static -O2 -std=c++14 -Wall solution_full.cpp -o sol_full.exe    // 满分档：根式化简 + 四种输出格式
//   node verify.js [sol_full.exe] [sol_lean.exe]
// 参照实现是"把输出字符串解析回数值 + 逐条比对题面格式规则"的独立检查器，另加 Math.sqrt 浮点真值兜底。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const FULL = process.argv[2] || path.join(__dirname, 'sol_full.exe');
const LEAN = process.argv[3] || path.join(__dirname, 'sol_lean.exe');
if (!fs.existsSync(FULL)) { console.error(`找不到 ${FULL}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution_full.cpp -o sol_full.exe`); process.exit(1); }
const hasLean = fs.existsSync(LEAN);

const SAMPLE_IN = ['9 1000', '1 -1 0', '-1 -1 -1', '1 -2 1', '1 5 4', '4 4 1', '1 0 -432', '1 -3 1', '2 -4 1', '1 7 1'].join('\n');
const SAMPLE_OUT = ['1', 'NO', '1', '-1', '-1/2', '12*sqrt(3)', '3/2+sqrt(5)/2', '1+sqrt(2)/2', '-7/2+3*sqrt(5)/2'];

const runLines = (exe, inp) =>
  execFileSync(exe, { input: inp, encoding: 'utf8', maxBuffer: 1 << 26 }).replace(/\s+$/, '').split(/\r?\n/);

// ---- ① 官方样例 ----
{
  const got = runLines(FULL, SAMPLE_IN + '\n');
  let ok = true;
  for (let i = 0; i < SAMPLE_OUT.length; i++) if (got[i] !== SAMPLE_OUT[i]) { ok = false; console.log(`样例第 ${i + 1} 行：期望 ${SAMPLE_OUT[i]} 实际 ${got[i]}`); }
  console.log(`① 官方样例 9 行（满分档）：${ok ? '逐字全对 ✅' : '有出入 ❌'}`);
  if (!ok) process.exitCode = 1;
  if (hasLean) {
    const p = runLines(LEAN, SAMPLE_IN + '\n');
    const hit = p.filter((v, i) => v === SAMPLE_OUT[i]).length;
    console.log(`   官方样例 9 行（骗分档）：对 ${hit} 行，错 ${9 - hit} 行（错的全是无理根那几行）`);
  }
}

// ---- ② 独立检查器：格式规则逐条 + 数值回代 ----
const gcd = (x, y) => { x = Math.abs(x); y = Math.abs(y); while (y) { [x, y] = [y, x % y]; } return x; };
const squarefree = (r) => { for (let d = 2; d * d <= r; d++) if (r % (d * d) === 0) return false; return true; };
const rat = (s) => { const p = s.split('/'); return p.length === 1 ? Number(p[0]) : Number(p[0]) / Number(p[1]); };

function analyze(str) {
  const fmt = [];
  if (/\s/.test(str)) fmt.push('含空格');                                   // 题面：字符串中间不应包含任何空格
  if (str === 'NO') return { val: NaN, fmt };
  if (!str.includes('sqrt(')) {
    const m = str.match(/^(-?\d+)(?:\/(\d+))?$/);
    if (!m) { fmt.push('非法格式'); return { val: NaN, fmt }; }
    if (m[2]) {
      if (gcd(Number(m[1]), Number(m[2])) !== 1) fmt.push('分数未约分');     // gcd(p,q)=1 且 q>0
      if (m[1].startsWith('-0')) fmt.push('负零');
    }
    return { val: rat(str), fmt };
  }
  const plus = str.lastIndexOf('+');
  let q1 = 0, tail = str;
  if (plus > 0) {
    q1 = rat(str.slice(0, plus));
    tail = str.slice(plus + 1);
    const fr = str.slice(0, plus).match(/^(-?\d+)\/(\d+)$/);
    if (fr && gcd(Number(fr[1]), Number(fr[2])) !== 1) fmt.push('q1 未约分');
  } else if (str.startsWith('-')) {
    fmt.push('q1<0 却没有把 q1 写在 + 号前面');
  }
  const m = tail.match(/^(\d+\*)?sqrt\((\d+)\)(\/(\d+))?$/);
  if (!m) { fmt.push('sqrt 部分非法'); return { val: NaN, fmt }; }
  const c = m[1] ? Number(m[1].replace('*', '')) : 1, r = Number(m[2]), d = m[4] ? Number(m[4]) : 1;
  if (r <= 1 || !squarefree(r)) fmt.push('r 不是 >1 的无平方因子数');
  if (d !== 1 && gcd(c, d) !== 1) fmt.push('q2 未约分');
  if (c === 1 && d === 1 && !/^sqrt\(\d+\)$/.test(tail)) fmt.push('q2=1 应写 sqrt(r)');
  if (c !== 1 && d === 1 && m[1] === undefined) fmt.push('q2 为整数时缺 *');
  if (c === 1 && d !== 1 && m[1] !== undefined) fmt.push('q2=1/d 不该带系数');
  return { val: q1 + (c / d) * Math.sqrt(r), fmt };
}

let bad = 0, partialDiff = 0, cases = 0, noCnt = 0, ratCnt = 0, sqrtCnt = 0, leanOk = 0, leanBad = 0;
function checkBatch(abc, M, tag) {
  const inp = `${abc.length} ${M}\n` + abc.map((v) => v.join(' ')).join('\n') + '\n';
  const F = runLines(FULL, inp);
  const P = hasLean ? runLines(LEAN, inp) : null;
  for (let i = 0; i < abc.length; i++) {
    const [a, b, c] = abc[i];
    const D = b * b - 4 * a * c;
    const out = F[i];
    cases++;
    if (D < 0) {
      noCnt++;
      if (out !== 'NO') { bad++; console.log('应 NO：', a, b, c, '->', out); }
      if (P && P[i] === 'NO') leanOk++; else if (P) leanBad++;
      continue;
    }
    const t = Math.floor(Math.sqrt(D) + 1e-9);
    const isRat = t * t === D;
    if (isRat) ratCnt++; else sqrtCnt++;
    const { val, fmt } = analyze(out);
    if (fmt.length) { bad++; if (bad <= 12) console.log(tag, '格式问题[' + fmt.join(',') + ']:', a, b, c, '->', out); }
    else {
      const trueLarge = a > 0 ? (-b + Math.sqrt(D)) / (2 * a) : (-b - Math.sqrt(D)) / (2 * a);
      if (!isFinite(val) || Math.abs(val - trueLarge) > 1e-8 * Math.max(1, Math.abs(trueLarge))) {
        bad++; if (bad <= 12) console.log(tag, '数值不符:', a, b, c, '输出', out, '应为', trueLarge);
      }
    }
    if (P) {
      if (isRat) { if (P[i] !== out) { partialDiff++; if (partialDiff <= 5) console.log('完全平方却不一致:', a, b, c, 'full=', out, 'lean=', P[i]); } leanOk++; }
      else { if (P[i] === out) leanOk++; else leanBad++; }
    }
  }
}

const R = (lo, hi) => lo + Math.floor(Math.random() * (hi - lo + 1));
const nonzeroA = (M) => { let a = 0; while (a === 0) a = R(-M, M); return a; };

// 1) 随机系数（M<=20 与 M<=1000 各 15 轮）
for (let round = 0; round < 30; round++) {
  const M = round < 15 ? 20 : 1000;
  const abc = [];
  for (let i = 0; i < 500; i++) abc.push([nonzeroA(M), R(-M, M), R(-M, M)]);
  checkBatch(abc, M, '随机');
}
// 2) 特殊性质 C：先给整数根再回推系数
for (let round = 0; round < 20; round++) {
  const abc = [];
  for (let i = 0; i < 500; i++) {
    const a = (Math.random() < 0.5 ? -1 : 1) * (1 + Math.floor(Math.random() * 12));
    const r1 = R(-4, 4), r2 = R(-4, 4);
    const b = -a * (r1 + r2), c = a * r1 * r2;
    if (Math.abs(b) <= 1000 && Math.abs(c) <= 1000) abc.push([a, b, c]);
  }
  checkBatch(abc, 1000, '整数根');
}
// 3) 特殊性质 A（b=0）/ B（c=0）
for (let round = 0; round < 20; round++) {
  const abc = [];
  for (let i = 0; i < 500; i++) {
    const a = nonzeroA(1000);
    abc.push(round % 2 === 0 ? [a, 0, R(-1000, 1000)] : [a, R(-1000, 1000), 0]);
  }
  checkBatch(abc, 1000, '性质A/B');
}
// 4) 分数根：根取 p/q 与 r/s，令 a = 2qs 保证整系数
for (let round = 0; round < 20; round++) {
  const abc = [];
  for (let i = 0; i < 500; i++) {
    const d1 = R(1, 6), n1 = R(-10, 10), d2 = R(1, 6), n2 = R(-10, 10);
    const a = 2 * d1 * d2;
    const b = -a * (n1 / d1 + n2 / d2);
    const c = a * (n1 / d1) * (n2 / d2);
    if (Number.isInteger(b) && Number.isInteger(c) && a <= 1000 && Math.abs(b) <= 1000 && Math.abs(c) <= 1000) abc.push([a, b, c]);
  }
  checkBatch(abc, 1000, '分数根');
}

console.log(`② 共 ${cases} 个方程：Δ<0 ${noCnt} / 有理根 ${ratCnt} / 无理根 ${sqrtCnt}`);
console.log(`   满分档错误（数值或格式）${bad}`);
if (bad) process.exitCode = 1;
if (hasLean) {
  console.log(`③ 骗分档：有理根分支与满分档逐字一致（不一致 ${partialDiff} 条）；整体判对 ${leanOk} / ${cases}（${(leanOk / cases * 100).toFixed(1)}%），无理根判错 ${leanBad} 条`);
  if (partialDiff) process.exitCode = 1;
}
