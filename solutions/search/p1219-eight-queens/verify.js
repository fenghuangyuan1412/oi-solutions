// 【验算脚本，不是题解代码】题解一律看 solution.cpp（AGENTS.md §3：讲解代码只用 C++）。
// 复现命令（在本目录）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js
// 想把 exe 挪出题目目录（*.exe 已在 .gitignore 里）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o ../../../_work/search-c/sol.exe
//   node verify.js ../../../_work/search-c/sol.exe
//
// 三件事：
//  ① 官方样例 n=6 逐字比对（先把 Windows 的 \r 去掉再整串比较，输出行末不允许有空格）。
//  ② 对拍参照实现换完全不同于"按行 DFS + 剪枝"的机制：**枚举 1..n 的全排列**
//     （next_permutation 天然按字典序生成，正合题面"解按字典序输出"），
//     逐个检查任意两子是否同对角线。排列数 n! 增长极快，只对 n=6..9 使用；
//     本脚本的"组数"就是这几个 n，全部远小于 200 组上限。
//  ③ 极限规模 n=12 与 n=13（题面上限，真正的压力点），打印实测耗时与解数。
//     注意：node verify.js 会跑到 n=13，本机全程约需数秒~十几秒（其中 n=13 占大头）。
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) {
  console.error(`找不到 ${EXE}\n请先按文件头注释用 g++ -static -O2 -std=c++14 编译。`);
  process.exit(1);
}

const norm = (s) => s.replace(/\r/g, '');
const run = (input) => norm(execFileSync(EXE, { input, encoding: 'utf8' }));

// ---- 参照实现：全排列 + 双对角线检查，返回 {count, first3[]} ----
function refPermutation(n) {
  const p = Array.from({ length: n }, (_, i) => i + 1);   // 初始排列 1..n
  let count = 0;
  const first3 = [];
  do {
    let ok = true;
    // 列已互不相同（排列保证），只需查两条对角线：|p[i]-p[j]| == j-i 即冲突
    outer: for (let i = 0; i < n; i++)
      for (let j = i + 1; j < n; j++)
        if (Math.abs(p[i] - p[j]) === j - i) { ok = false; break outer; }
    if (ok) {
      count++;
      if (count <= 3) first3.push(p.join(' '));
    }
  } while (nextPerm(p));
  return { count, first3 };
}
function nextPerm(a) {                                   // 手写 next_permutation，字典序下一个
  let i = a.length - 2;
  while (i >= 0 && a[i] > a[i + 1]) i--;
  if (i < 0) return false;
  let j = a.length - 1;
  while (a[j] < a[i]) j--;
  [a[i], a[j]] = [a[j], a[i]];
  for (let l = i + 1, r = a.length - 1; l < r; l++, r--) [a[l], a[r]] = [a[r], a[l]];
  return true;
}

// ---- ① 官方样例 n=6 逐字比对 ----
{
  const want = '2 4 6 1 3 5\n3 6 2 5 1 4\n4 1 5 2 6 3\n4\n';
  const got = run('6\n');
  const ok = got === want;
  console.log(`① 官方样例 n=6 逐字比对：${ok ? '✅' : '❌'}${ok ? '' : `\n期望:\n${want}实际:\n${got}`}`);
  if (!ok) process.exit(1);
}

// ---- ② n=6..9：输出整串 vs 全排列参照 ----
{
  let bad = 0, rounds = 0;
  for (let n = 6; n <= 9; n++) {
    rounds++;
    const ref = refPermutation(n);
    const want = ref.first3.join('\n') + '\n' + ref.count + '\n';
    const got = run(`${n}\n`);
    if (got !== want) {
      bad++;
      console.log(`n=${n} 不一致:\nsol=\n${got}ref=\n${want}`);
    } else {
      console.log(`② n=${n}：前3解+总数 ${ref.count} 与全排列参照一致 ✅`);
    }
  }
  console.log(`② 对拍共 ${rounds} 组（n=6..9），不一致 ${bad} 组`);
  if (bad) process.exit(1);
}

// ---- ③ 极限规模 n=12 / n=13 ----
for (const n of [12, 13]) {
  const ts = [];
  let out = '';
  for (let r = 0; r < 2; r++) { const t0 = Date.now(); out = run(`${n}\n`); ts.push(Date.now() - t0); }
  const lines = out.trim().split('\n');
  console.log(`③ n=${n}（题面上限档）：解总数 ${lines[lines.length - 1]}，耗时 ${ts.join('/')} ms（跑两轮的区间；n=13 是压力点）`);
}
