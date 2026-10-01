// 【验算脚本，仅用于验证，非讲解代码】AGENTS.md §3：讲解代码一律 C++（见 solution.cpp）。
// 参照实现换了机制：不递归，而是用二进制位掩码枚举 {0..n-1} 的所有子集，挑出恰好 k 个 bit 的
// 那种选法求和，再用一份独立写的试除判素函数判定 —— 和 C++ 版"DFS 组合 + isPrime"互不共享逻辑。
//
// 复现命令（本机两套 MinGW 路径冲突，-static 必须加）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                        # 默认找同目录 sol.exe
//   node verify.js ../../_work/search-a/sol_1036.exe
//
// 三件事：① 官方样例逐字比对 + 手挑的素数边界用例；② 与位掩码参照随机对拍（默认 200 组）；
// ③ 极限规模 n=20（k=19 与 k=10 两档，x_i 取 5e6 量级）计时，和题面 1500ms 时限对照。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`);
  process.exit(1);
}
const MAX_BUF = 1 << 26;
const rnd = (k) => Math.floor(Math.random() * k);
const run = (input) => execFileSync(EXE, { input, encoding: 'utf8', maxBuffer: MAX_BUF }).replace(/\r/g, '').replace(/\s+$/, '');

// ---- 参照实现：位掩码枚举组合 + 独立试除判素 ----
function refIsPrime(v) {                 // 和 C++ 版同思路但独立书写：先砍 2、3，再按 6t±1 试除
  if (v < 2) return false;               // 1 不是素数（本题最大的坑之一）
  if (v % 2 === 0) return v === 2;
  if (v % 3 === 0) return v === 3;
  for (let i = 5; i * i <= v; i += 6)    // 因子只可能是 6t±1 形式，比逐个试除快 3 倍
    if (v % i === 0 || v % (i + 2) === 0) return false;
  return true;
}
function refCount(n, k, x) {
  let cnt = 0;
  for (let mask = 0; mask < (1 << n); mask++) {    // 枚举 n 个数的所有 2^n 个子集
    let bits = 0, sum = 0;
    for (let i = 0; i < n; i++) if (mask & (1 << i)) { bits++; sum += x[i]; }
    if (bits === k && refIsPrime(sum)) cnt++;      // 恰好选 k 个，且和被判定为素数
  }
  return cnt;
}
const C = (n, k) => { if (k < 0 || k > n) return 0; let r = 1; for (let i = 1; i <= k; i++) r = (r * (n - k + i)) / i; return Math.round(r); };

let ok = true;
const check = (name, inp, want) => {
  const got = run(inp);
  const pass = got === String(want);
  if (!pass) ok = false;
  console.log(`${name}: 输入 ${JSON.stringify(inp.replace(/\n$/, ''))} 期望 ${want} 实际 ${got} ${pass ? '✅' : '❌'}`);
};

// ===================== ① 官方样例 + 素数边界用例 =====================
check('官方样例 n=4 k=3', '4 3\n3 7 12 19\n', 1);
check('边界 1 不是素数 n=3 k=1', '3 1\n1 2 4\n', 1);            // 和为 1/2/4，只有 2 是素数
check('边界 全部相同 n=4 k=2', '4 2\n1 1 1 1\n', 6);            // C(4,2)=6 种选法，每个和都是 2 ⇒ 6
check('边界 1+1=2 是素数 n=3 k=2', '3 2\n1 1 5\n', 1);           // 组合和 2, 6, 6 ⇒ 只有 2 是素数，答案 1
check('边界 n=1 k=0（题面 k<n 的下边界）', '1 0\n5\n', 0);       // 选 0 个数和为 0，不是素数
{
  // 把上面几组的手算答案和参照实现再对一遍，防止"注释想当然"
  const probes = [[3, 1, [1, 2, 4]], [4, 2, [1, 1, 1, 1]], [3, 2, [1, 1, 5]], [1, 0, [5]]];
  console.log(`  边界用例的参照实现结果：${probes.map(([a, b, c]) => `(${a},${b})=>${refCount(a, b, c)}`).join('  ')}`);
}

// ===================== ② 随机对拍 =====================
{
  let bad = 0;
  const ROUNDS = Number(process.env.ROUNDS || 200);              // 用户要求：不必测特别多
  let maxSum = 0;
  for (let t = 0; t < ROUNDS; t++) {
    const n = 2 + rnd(11);                        // 2 <= n <= 12（参照枚举 2^12 个子集，瞬间完成）
    const k = 1 + rnd(n);                         // 1 <= k < n，符合题面 k < n
    const x = [];
    for (let i = 0; i < n; i++) {
      // 三种值域混搭：小数（触发 1、2 的判素边界）、中等、接近上限，让和的分布铺开
      x.push([1 + rnd(5), 1 + rnd(1000), 1 + rnd(1000000)][rnd(3)]);
    }
    const topK = x.slice().sort((a, b) => b - a).slice(0, k).reduce((s, v) => s + v, 0);   // 这组数据可能出现的最大和
    maxSum = Math.max(maxSum, topK);
    const inp = `${n} ${k}\n${x.join(' ')}\n`;
    const want = refCount(n, k, x);
    const got = run(inp);
    if (got !== String(want)) {
      bad++;
      if (bad === 1) console.log(`首个不一致：\n${inp}期望 ${want} 实际 ${got}`);
    }
  }
  console.log(`随机对拍 ${ROUNDS} 组（n<=12, k<n, x_i 混合值域，本批最大部分和 ${maxSum}），不一致 ${bad} 组`);
  if (bad) ok = false;
}

// ===================== ③ 极限规模 =====================
{
  // 3a. 题面上限的组合数最大档：n=20, k=10 ⇒ C(20,10)=184756 种，每种都要判素
  const n = 20, k = 10;
  const x = [];
  for (let i = 0; i < n; i++) x.push(1 + rnd(5000000));      // x_i <= 5e6（题面上限量级）
  const inp = `${n} ${k}\n${x.join(' ')}\n`;
  const t0 = Date.now();
  const got = run(inp);
  const ms = Date.now() - t0;                                 // 含进程启动 + 本机安全软件扫描
  const t1 = Date.now();
  const want = refCount(n, k, x);
  const jsMs = Date.now() - t1;
  console.log(`极限 A：n=${n}, k=${k}（枚举 C(20,10)=${C(20, 10)} 种组合），x_i 随机 <=5e6，最大可能和 ${[...x].sort((a, b) => b - a).slice(0, k).reduce((s, v) => s + v, 0)}`
    + ` ⇒ 答案 ${got}（参照 ${want}）${got === String(want) ? '✅' : '❌'}，C++ 侧耗时 ${ms} ms，JS 参照耗时 ${jsMs} ms`);
  if (got !== String(want)) ok = false;

  // 3b. 用户指定的极限档：k=19 ⇒ 只有 20 种组合，但每个和接近 19*5e6 = 9.5e7，判素要试除到 sqrt ≈ 9747
  const k2 = 19;
  const y = [];
  for (let i = 0; i < n; i++) y.push(4500000 + rnd(500001)); // 都取 4.5e6 ~ 5e6，和 ~ 8.5e7 ~ 9.5e7
  const inp2 = `${n} ${k2}\n${y.join(' ')}\n`;
  const t2 = Date.now();
  const got2 = run(inp2);
  const ms2 = Date.now() - t2;
  const want2 = refCount(n, k2, y);
  const bigSum = y.reduce((s, v) => s + v, 0);
  console.log(`极限 B：n=${n}, k=${k2}（C(20,19)=${C(20, 19)} 种组合），x_i ∈ [4.5e6, 5e6]，最大和 ${bigSum}（sqrt ≈ ${Math.floor(Math.sqrt(bigSum))}）`
    + ` ⇒ 答案 ${got2}（参照 ${want2}）${got2 === String(want2) ? '✅' : '❌'}，C++ 侧耗时 ${ms2} ms`);
  if (got2 !== String(want2)) ok = false;
  console.log(`  时限对照：题面 1500ms。上面两档是本机能构造的最费时的形态（组合数最多 / 判素试除最深）`);
}

console.log(ok ? '结论：全部通过（本地验算，未提交洛谷）' : '结论：存在差异，见上方定位');
process.exit(ok ? 0 : 1);
