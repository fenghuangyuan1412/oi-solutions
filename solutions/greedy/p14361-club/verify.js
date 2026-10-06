/*
 * verify.js —— 仅用于验证，非讲解代码（讲课只看 README / solution*.cpp）
 *
 * 作用：
 *   1) 官方样例逐字断言（solution.cpp / solution_partial.cpp 都要输出 18 4 13）；
 *   2) 随机小数据把「正解贪心」「骗分 DP」「JS 全枚举 3^n 参照」三者对拍。
 *      JS 参照实现直接按题面定义枚举每一种分配方案，不含任何算法技巧，可信度最高。
 *   组数：默认 100 组（用户要求对拍从简），n 取 2/4/6/8，满意度取 0~9。
 *
 * 先编译（本机两套 MinGW 冲突，必须 -static）：
 *   g++ -static -O2 -std=c++14 solution.cpp          -o E:/ai/suanfa_study/.raw/bin/p14361_sol.exe
 *   g++ -static -O2 -std=c++14 solution_partial.cpp  -o E:/ai/suanfa_study/.raw/bin/p14361_partial.exe
 * 再运行：node verify.js [组数]
 */
const cp = require('child_process');
const BIN = 'E:/ai/suanfa_study/.raw/bin/';
const SOL = BIN + 'p14361_sol.exe';
const PAR = BIN + 'p14361_partial.exe';

function run(exe, input) {
  return cp.execFileSync(exe, [], { input, encoding: 'utf8' }).trim().split(/\s+/).map(Number);
}

// 参照实现：按题面枚举所有 3^n 种分配，取满足"没有部门多于 n/2 人"的最大满意度
function brute(a, n) {
  const half = n / 2;
  let best = -1;
  const total = Math.pow(3, n);
  const cnt = [0, 0, 0];
  for (let mask = 0; mask < total; ++mask) {
    let x = mask, sum = 0, ok = true;
    cnt[0] = cnt[1] = cnt[2] = 0;
    for (let i = 0; i < n; ++i) {
      const d = x % 3; x = (x - d) / 3;
      cnt[d]++;
      if (cnt[d] > half) { ok = false; break; }   // 提前淘汰
      sum += a[i][d];
    }
    if (ok) best = Math.max(best, sum);
  }
  return best;
}

function genCase(n, maxV) {
  const rows = [];
  for (let i = 0; i < n; ++i) {
    const r = [0, 0, 0].map(() => Math.floor(Math.random() * (maxV + 1)));
    rows.push(r);
  }
  return rows;
}

// ---- ① 官方样例 ----
const SAMPLE_IN = `3\n4\n4 2 1\n3 2 4\n5 3 4\n3 5 1\n4\n0 1 0\n0 1 0\n0 2 0\n0 2 0\n2\n10 9 8\n4 0 0\n`;
const WANT = [18, 4, 13];
const gotSol = run(SOL, SAMPLE_IN);
const gotPar = run(PAR, SAMPLE_IN);
console.log('官方样例 期望 =', WANT.join(' '));
console.log('  solution.cpp        =>', gotSol.join(' '));
console.log('  solution_partial.cpp=>', gotPar.join(' '));
if (gotSol.join(',') !== WANT.join(',') || gotPar.join(',') !== WANT.join(',')) {
  console.log('样例不一致，终止');
  process.exit(1);
}

// ---- ② 随机对拍 ----
const REP = Number(process.argv[2] || 100);
let bad = 0;
const NS = [2, 4, 6, 8];
for (let round = 1; round <= REP; ++round) {
  const T = 1 + Math.floor(Math.random() * 3);
  const cases = [];
  const lines = [String(T)];
  for (let tc = 0; tc < T; ++tc) {
    const n = NS[Math.floor(Math.random() * NS.length)];
    const a = genCase(n, 9);
    cases.push({ n, a });
    lines.push(String(n));
    for (const r of a) lines.push(r.join(' '));
  }
  const input = lines.join('\n') + '\n';
  const want = cases.map(c => brute(c.a, c.n));
  const got1 = run(SOL, input);
  const got2 = run(PAR, input);
  if (want.join(',') !== got1.join(',') || want.join(',') !== got2.join(',')) {
    bad++;
    console.log('不一致 round=' + round + '\n输入:\n' + input +
      '参照(3^n 全枚举)=' + want.join(' ') + '\n贪心=' + got1.join(' ') + '\nDP=' + got2.join(' '));
    break;
  }
}
console.log('随机对拍 ' + REP + ' 组（每组 1~3 个用例，n <= 8）：不一致 ' + bad + ' 组');
process.exit(bad ? 1 : 0);
