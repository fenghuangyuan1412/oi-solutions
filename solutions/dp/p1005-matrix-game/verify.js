// 【验算脚本，不是题解代码】题解一律看 solution.cpp / solution_full.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp      -o sol_ll.exe      // 60 分档（long long）
//   g++ -static -O2 -std=c++14 -Wall solution_full.cpp -o sol_i128.exe    // 满分档（__int128）
//   node verify.js [sol_ll.exe] [sol_i128.exe]
// 参照实现：逐行枚举 2^m 种"左/右"取法（BigInt 精确整数），既验证区间 DP，也验证"各行独立"这个前提。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const LLEXE = process.argv[2] || path.join(__dirname, 'sol_ll.exe');
const IEXE = process.argv[3] || path.join(__dirname, 'sol_i128.exe');
if (!fs.existsSync(LLEXE)) { console.error(`找不到 ${LLEXE}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol_ll.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (exe, input) => norm(execFileSync(exe, { input, encoding: 'utf8', maxBuffer: 1 << 26 }));

// 一行枚举 2^m 种取法的最优值（BigInt）。**只适用于 m ≤ 20**：
// JS 的 `1 << m` 与 `mask >> k` 都按 m % 32 计算，m ≥ 32 时会静默alias成小指数，
// 枚举规模既不对、又会慢到跑不完 —— 所以这里直接拦下来，别让它假装算完了。
function rowBrute(row) {
  const m = row.length;
  if (m > 20) throw new Error(`rowBrute 只支持 m<=20（传进来 m=${m}）；大 m 请改用闭式 rowAllEqual()`);
  let best = -1n;
  for (let mask = 0; mask < (1 << m); mask++) {          // 第 k 次取：0=拿左端，1=拿右端
    let l = 0, r = m - 1, sum = 0n;
    for (let k = 1; k <= m; k++) {
      const take = ((mask >> (k - 1)) & 1) ? row[r--] : row[l++];
      sum += BigInt(take) * (2n ** BigInt(k));
    }
    if (sum > best) best = sum;
  }
  return best;
}
const refTotal = (rows) => rows.reduce((s, r) => s + rowBrute(r), 0n);
// 整行取值都相同时，无论从哪里取，第 k 次拿到的都是 v ⇒ 总和恒为 v*(2^(m+1)-2)。
// 溢出扫描那一节用的是"全 1000 的一行"，所以可以直接套闭式，不用 2^m 枚举。
const rowAllEqual = (v, m) => BigInt(v) * ((2n ** BigInt(m + 1)) - 2n);

function mkRows(n, m, maxV) {
  const rows = [];
  let inp = `${n} ${m}\n`;
  for (let i = 0; i < n; i++) {
    const row = [];
    for (let j = 0; j < m; j++) row.push(Math.floor(Math.random() * (maxV + 1)));
    rows.push(row);
    inp += row.join(' ') + '\n';
  }
  return { rows, inp };
}

// ---- ① 官方样例 ----
{
  const inp = '2 3\n1 2 3\n3 4 2\n';
  for (const [tag, exe] of [['long long 版', LLEXE], ['__int128 版', fs.existsSync(IEXE) ? IEXE : null]]) {
    if (!exe) continue;
    const got = run(exe, inp);
    console.log(`${tag} 官方样例：期望 82 实际 ${got} ${got === '82' ? '✅' : '❌'}`);
    if (got !== '82') process.exitCode = 1;
  }
}

// ---- ② 与 2^m 暴力对拍 ----
{
  let bad = 0;
  const ROUNDS = Number(process.env.ROUNDS || 800);
  for (let t = 0; t < ROUNDS; t++) {
    const { rows, inp } = mkRows(1 + Math.floor(Math.random() * 3), 1 + Math.floor(Math.random() * 5), 1000);
    const got = run(LLEXE, inp), want = String(refTotal(rows));
    if (got !== want) { bad++; if (bad === 1) console.log('不一致\n' + inp + 'dp=' + got + ' 暴力=' + want); }
  }
  console.log(`P1005 区间 DP vs 逐行 2^m 枚举：${ROUNDS} 组（n<=3, m<=5, 值 0..1000），不一致 ${bad}`);
  if (bad) process.exitCode = 1;
}

// ---- ③ long long 从哪一列开始爆（骗分档的分数来源就在这里）----
for (const m of [40, 45, 50, 58, 60, 62, 64, 80]) {
  const row = Array.from({ length: m }, () => 1000);
  const inp = `1 ${m}\n${row.join(' ')}\n`;
  const got = run(LLEXE, inp);
  const want = rowAllEqual(1000, m).toString();
  console.log(`  m=${String(m).padStart(2)}：long long 版输出 ${got.padStart(28)}，真值 ${want} ${got === want ? 'OK' : '❌ 溢出'}`);
}

// ---- ④ __int128 版在题面上限 80x80 上与 BigInt 真值逐位比对 ----
if (fs.existsSync(IEXE)) {
  const n = 80, m = 80;
  const rows = Array.from({ length: n }, () => Array.from({ length: m }, () => 1000));
  const inp = `${n} ${m}\n` + rows.map(r => r.join(' ')).join('\n') + '\n';
  // 全 1000 时每行最优值 = 1000*(2^(m+1)-2)，直接闭式算，避免 2^80 枚举
  const per = 1000n * ((2n ** BigInt(m + 1)) - 2n);
  const want = (per * BigInt(n)).toString();
  const t0 = Date.now();
  const got = run(IEXE, inp);
  console.log(`__int128 版 80x80 全 1000：输出 ${got}`);
  console.log(`BigInt 真值              ：期望 ${want}`);
  console.log(`  ${got === want ? '✅ 逐位相等' : '❌ 不等'}，耗时 ${Date.now() - t0} ms`);
  if (got !== want) process.exitCode = 1;

  // 与 long long 版互拍（小数据必须一致）
  let bad = 0;
  for (let t = 0; t < 400; t++) {
    const { rows, inp } = mkRows(1 + Math.floor(Math.random() * 2), 1 + Math.floor(Math.random() * 4), 1000);
    if (run(IEXE, inp) !== run(LLEXE, inp)) bad++;
  }
  console.log(`__int128 版与 long long 版互拍 400 组（m<=4，不会溢出）：不一致 ${bad}`);
  if (bad) process.exitCode = 1;
} else {
  console.log(`（跳过满分档：没找到 ${IEXE}，编译 solution_full.cpp 后再跑）`);
}
