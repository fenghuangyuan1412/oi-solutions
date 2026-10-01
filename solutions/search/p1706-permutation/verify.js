// 【验算脚本，仅用于验证，非讲解代码】AGENTS.md §3：讲解代码一律 C++（见 solution.cpp），
// 这里用 Node 写一份"机制完全不同"的参照实现对拍：C++ 版是 DFS + vis[] 回溯，
// JS 参照版是从 1..n 出发反复调用字典序 next_permutation 生成全部排列。
//
// 复现命令（本机两套 MinGW 路径冲突，-static 必须加）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                         # 默认找同目录 sol.exe
//   node verify.js ../../_work/search-a/sol_1706.exe    # 也可以显式指定 exe
//
// 做三件事：① 官方样例逐字比对（场宽 / 行末空格 / 末尾换行一个都不放过）
//          ② 与 next_permutation 参照实现随机对拍（默认 120 组，n <= 7）
//          ③ 极限规模 n = 9（362880 行）跑一遍并计时
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`);
  process.exit(1);
}
const MAX_BUF = 1 << 28;                       // n=9 时输出约 16MB，默认 1MB 会被截断
const rnd = (k) => Math.floor(Math.random() * k);
// Windows 下 C++ 的 stdout 会把 '\n' 换成 '\r\n'，比对前统一去掉 '\r'
const run = (input) => execFileSync(EXE, { input, encoding: 'utf8', maxBuffer: MAX_BUF }).replace(/\r/g, '');

// ---- 参照实现：字典序下一个排列（与 DFS 完全无关的机制）----
function nextPermutation(a) {
  let i = a.length - 2;
  while (i >= 0 && a[i] >= a[i + 1]) i--;
  if (i < 0) return false;                      // 已经是降序 = 最后一个排列
  let j = a.length - 1;
  while (a[j] <= a[i]) j--;
  [a[i], a[j]] = [a[j], a[i]];
  for (let l = i + 1, r = a.length - 1; l < r; l++, r--) [a[l], a[r]] = [a[r], a[l]];
  return true;
}
const pad = (v, w) => String(v).padStart(w, ' ');   // 等价于 C++ 的 setw(w) << v
function refPermutations(n, width = 5) {
  const a = [];
  for (let i = 1; i <= n; i++) a.push(i);
  const lines = [];
  do { lines.push(a.map((v) => pad(v, width)).join('')); } while (nextPermutation(a));
  return lines.join('\n') + '\n';
}

// ---- 逐字比对工具：把差异定位到具体行/列，并显示行长度 ----
function verbatim(name, got, want) {
  if (got === want) { console.log(`${name}：逐字一致 ✅（${want.split('\n').length - 1} 行）`); return true; }
  const g = got.split('\n'), w = want.split('\n');
  console.log(`${name}：逐字不一致 ❌（输出 ${g.length} 段 / 期望 ${w.length} 段）`);
  for (let i = 0; i < Math.max(g.length, w.length); i++) {
    if (g[i] !== w[i]) {
      console.log(`  首个差异在第 ${i + 1} 行：实际 [${g[i]}] 长度 ${g[i] === undefined ? 'N/A' : g[i].length}，`
        + `期望 [${w[i]}] 长度 ${w[i] === undefined ? 'N/A' : w[i].length}`);
      break;
    }
  }
  return false;
}
let ok = true;

// ===================== ① 官方样例（n = 3）=====================
{
  const want = '    1    2    3\n' + '    1    3    2\n' + '    2    1    3\n'
             + '    2    3    1\n' + '    3    1    2\n' + '    3    2    1\n';   // 题面原样：每数 5 场宽，末尾有换行
  const got = run('3\n');
  ok = verbatim('官方样例 n=3', got, want) && ok;
  const lines = got.replace(/\n$/, '').split('\n');
  console.log(`  格式体检：行数 ${lines.length}，各行长度 [${lines.map((l) => l.length).join(',')}]（应为 15 = 3×5）`
    + `，行末多余空格的行数 ${lines.filter((l) => / $/.test(l)).length}，末尾换行 ${got.endsWith('\n') ? '有' : '无'}`);
  console.log(`  与参照实现（next_permutation）比：${got === refPermutations(3) ? '一致' : '不一致'} ✅对照`);
}

// ===================== ② 随机对拍 =====================
{
  let bad = 0;
  const ROUNDS = Number(process.env.ROUNDS || 120);        // 用户要求：不必测特别多
  const dist = {};
  for (let t = 0; t < ROUNDS; t++) {
    const n = 1 + rnd(7);                                   // 1 <= n <= 7（n=8 起输出 40320 行，交给极限测试）
    dist[n] = (dist[n] || 0) + 1;
    const inp = `${n}\n`;
    const got = run(inp), want = refPermutations(n);
    if (got !== want) {
      bad++;
      if (bad === 1) {
        console.log(`首个不一致 n=${n}\n实际前 3 行:\n${got.split('\n').slice(0, 3).join('\n')}\n`
          + `参照前 3 行:\n${want.split('\n').slice(0, 3).join('\n')}`);
      }
    }
  }
  const fact = (n) => { let f = 1; for (let i = 2; i <= n; i++) f *= i; return f; };
  console.log(`随机对拍 ${ROUNDS} 组（n 分布 ${JSON.stringify(dist)}），逐字比对（含场宽/行末/末行换行）不一致 ${bad} 组`);
  console.log(`  交叉验证：组数应等于 n!，实测 n=1..7 的行数 ${[1, 2, 3, 4, 5, 6, 7].map((n) => run(`${n}\n`).replace(/\n$/, '').split('\n').length).join(',')} vs ${[1, 2, 3, 4, 5, 6, 7].map(fact).join(',')}`);
  if (bad) ok = false;
}

// ===================== ③ 极限规模 n = 9 =====================
{
  const n = 9;
  const t0 = Date.now();
  const got = run(`${n}\n`);
  const ms = Date.now() - t0;                                // 含进程启动 + 安全软件扫描的开销
  const t1 = Date.now();
  const want = refPermutations(n);
  const jsMs = Date.now() - t1;
  const lines = got.replace(/\n$/, '').split('\n');
  console.log(`极限 n=${n}：${lines.length} 行（= 9! = 362880），字符数 ${got.length}，C++ 侧耗时 ${ms} ms，`
    + `JS 参照生成耗时 ${jsMs} ms，逐字比对 ${got === want ? '一致 ✅' : '不一致 ❌'}`);
  console.log(`  行长度全部为 ${5 * n}？${lines.every((l) => l.length === 5 * n) ? '是' : '否'}；`
    + `首行 [${lines[0]}]，末行 [${lines[lines.length - 1]}]`);
  if (got !== want) ok = false;
}

console.log(ok ? '结论：全部通过（本地验算，未提交洛谷）' : '结论：存在差异，见上方定位');
process.exit(ok ? 0 : 1);
