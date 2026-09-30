// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 用法：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp          -o sol.exe
//   g++ -static -O2 -std=c++14 -Wall solution_partial.cpp  -o sol_partial.exe   // 反面教材（可选）
//   node verify.js [sol.exe] [sol_partial.exe]
// 参照实现与题解完全独立：不推公式，而是逐秒做 DP —— 状态是"当前魔法值"，值是该秒能到达的最远距离。
// 三个动作（跑 17m / 闪 60m 耗 10 魔法 / 原地休息回 4 魔法）每秒各转移一次，等价于穷举一切策略。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
const PEXE = process.argv[3] || path.join(__dirname, 'sol_partial.exe');
if (!fs.existsSync(EXE)) { console.error(`找不到 ${EXE}\n请先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`); process.exit(1); }
const norm = (s) => s.replace(/\r/g, '').replace(/\s+$/, '');
const run = (exe, input) => norm(execFileSync(exe, { input, encoding: 'utf8' }));

// 逐秒 DP：dp[魔法] = 这一秒结束时能到达的最远距离
function brute(M, S, T) {
  let cur = new Map([[M, 0]]);
  let bestAt = -1, maxDist = 0;
  for (let t = 1; t <= T; t++) {
    const nxt = new Map();
    for (const [mana, dist] of cur) {
      const upd = (m2, d2) => { const old = nxt.get(m2); if (old === undefined || d2 > old) nxt.set(m2, d2); };
      upd(mana, dist + 17);                        // 跑步
      if (mana >= 10) upd(mana - 10, dist + 60);   // 闪烁
      upd(mana + 4, dist);                         // 原地休息（只有休息才回魔法）
    }
    cur = nxt;
    let mx = 0;
    for (const d of cur.values()) if (d > mx) mx = d;
    if (mx > maxDist) maxDist = mx;
    if (bestAt < 0 && mx >= S) bestAt = t;
  }
  return bestAt > 0 ? `Yes\n${bestAt}` : `No\n${maxDist}`;
}

// ---- ① 官方样例 ----
const SAMPLES = [
  { in: '39 200 4', out: 'No\n197' },
  { in: '36 255 10', out: 'Yes\n6' },
];
for (const s of SAMPLES) {
  const got = run(EXE, s.in + '\n');
  console.log(`样例 ${s.in}：期望 ${JSON.stringify(s.out)} 实际 ${JSON.stringify(got)} ${got === s.out ? '✅' : '❌'}`);
  if (got !== s.out) process.exitCode = 1;
}

// ---- ② 与逐秒 DP 对拍 ----
{
  let bad = 0;
  const ROUNDS = Number(process.env.ROUNDS || 1200);
  for (let t = 0; t < ROUNDS; t++) {
    const M = Math.floor(Math.random() * 41);
    const S = 1 + Math.floor(Math.random() * 3000);
    const T = 1 + Math.floor(Math.random() * 22);
    const inp = `${M} ${S} ${T}\n`;
    const got = run(EXE, inp), want = brute(M, S, T);
    if (got !== want) { bad++; if (bad <= 5) console.log(`不一致 M=${M} S=${S} T=${T} 枚举k=${JSON.stringify(got)} DP=${JSON.stringify(want)}`); }
  }
  console.log(`P1095 枚举 k 版 vs 逐秒 DP：${ROUNDS} 组（M<=40, S<=3000, T<=22），不一致 ${bad}`);
  if (bad) process.exitCode = 1;
}

// ---- ③ 反面教材：漏掉"休息"这条规则，到底值多少分 ----
if (fs.existsSync(PEXE)) {
  let ok = 0;
  const TOT = Number(process.env.PTOT || 500);
  for (let t = 0; t < TOT; t++) {
    const M = Math.floor(Math.random() * 41);
    const S = 1 + Math.floor(Math.random() * 3000);
    const T = 1 + Math.floor(Math.random() * 22);
    const inp = `${M} ${S} ${T}\n`;
    if (run(PEXE, inp) === brute(M, S, T)) ok++;
  }
  console.log(`不休息贪心版（solution_partial.cpp）在 ${TOT} 组随机数据上与正确答案一致 ${ok} 组（正确率 ${(ok / TOT * 100).toFixed(1)}%）`);
  const s2 = run(PEXE, '36 255 10\n');
  console.log(`它在官方样例 2 上输出 ${JSON.stringify(s2)}，正确答案 ${JSON.stringify('Yes\n6')} —— 样例都过不去`);
} else {
  console.log(`（跳过反面教材：没找到 ${PEXE}）`);
}

// ---- ④ 极限规模计时：T=3e5, S=1e8, M=1000 ----
{
  const ts = [];
  let out = '';
  for (let i = 0; i < 3; i++) { const t0 = Date.now(); out = run(EXE, '1000 100000000 300000\n'); ts.push(Date.now() - t0); }
  console.log(`极限 M=1000 S=1e8 T=3e5：输出 ${JSON.stringify(out.replace('\n', ' '))}，耗时 ${ts.join('/') } ms`);
}
