// 【验算脚本，仅用于验证，非讲解代码】AGENTS.md §3：讲解代码一律 C++（见 solution.cpp）。
// 这里的参照实现用的是和 DFS 完全不同的机制：把 {1..n} 的所有子集用二进制位掩码枚举出来，
// 只保留恰好 r 个 bit 为 1 的，再显式按字典序排序输出 —— 这样"每层从上一个数 + 1 开始"
// 这个递归设计就被独立地验证了一遍（顺序和集合内容都拍）。
//
// 复现命令（本机两套 MinGW 路径冲突，-static 必须加）：
//   g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe
//   node verify.js                        # 默认找同目录 sol.exe
//   node verify.js ../../_work/search-a/sol_1157.exe
//
// 三件事：① 官方样例逐字比对（每元素 3 场宽、行末无空格、末行换行差异单独报告）
//        ② 与位掩码参照实现随机对拍（默认 120 组，n <= 14，含 r = 0 / r = n 边界）
//        ③ 极限规模 n = 20, r = 10（184756 行）计时
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const EXE = process.argv[2] || path.join(__dirname, 'sol.exe');
if (!fs.existsSync(EXE)) {
  console.error(`找不到可执行文件 ${EXE}\n先编译：g++ -static -O2 -std=c++14 -Wall solution.cpp -o sol.exe`);
  process.exit(1);
}
const MAX_BUF = 1 << 28;                        // n=20,r=10 时输出约 5.7MB
const rnd = (k) => Math.floor(Math.random() * k);
const run = (input) => execFileSync(EXE, { input, encoding: 'utf8', maxBuffer: MAX_BUF }).replace(/\r/g, '');
const pad = (v, w) => String(v).padStart(w, ' ');         // 等价 C++ 的 setw(w) << v
const popcount = (m) => { let c = 0; while (m) { c += m & 1; m >>= 1; } return c; };

// ---- 参照实现：位掩码枚举子集 + 显式字典序排序 ----
function refCombinations(n, r, width = 3) {
  const combos = [];
  for (let mask = 0; mask < (1 << n); mask++) {           // 枚举 {1..n} 的全部 2^n 个子集
    if (popcount(mask) !== r) continue;                   // 只要恰好 r 个元素的
    const c = [];
    for (let i = 0; i < n; i++) if (mask & (1 << i)) c.push(i + 1);
    combos.push(c);
  }
  combos.sort((a, b) => {                                 // 关键：掩码从小到大的顺序 NOT 字典序，必须显式排序
    for (let i = 0; i < a.length; i++) if (a[i] !== b[i]) return a[i] - b[i];
    return 0;
  });
  if (combos.length === 0) return '';                     // r > n 的兜底（题面保证 r <= n，走不到）
  return combos.map((c) => c.map((v) => pad(v, width)).join('')).join('\n') + '\n';
}
// 二项式系数，用来核对"行数应该等于 C(n,r)"
const C = (n, k) => { if (k < 0 || k > n) return 0; let r = 1; for (let i = 1; i <= k; i++) r = (r * (n - k + i)) / i; return Math.round(r); };

function verbatim(name, got, want) {
  if (got === want) { console.log(`${name}：逐字一致 ✅`); return true; }
  const g = got.split('\n'), w = want.split('\n');
  console.log(`${name}：逐字不一致 ❌（输出 ${g.length} 段 / 期望 ${w.length} 段）`);
  for (let i = 0; i < Math.max(g.length, w.length); i++) {
    if (g[i] !== w[i]) { console.log(`  首个差异第 ${i + 1} 行：实际 [${g[i]}](${g[i] === undefined ? 'N/A' : g[i].length} 字符) 期望 [${w[i]}](${w[i] === undefined ? 'N/A' : w[i].length} 字符)`); break; }
  }
  return false;
}
let ok = true;

// ===================== ① 官方样例 n=5 r=3 =====================
{
  // 题面原样 10 行，每行 9 字符（3 个元素 × 3 场宽），官方页面最后一行后面没有换行
  const wantNoNL = ['  1  2  3', '  1  2  4', '  1  2  5', '  1  3  4', '  1  3  5',
                    '  1  4  5', '  2  3  4', '  2  3  5', '  2  4  5', '  3  4  5'].join('\n');
  const got = run('5 3 \n');                              // 样例输入 "5 3" 后面本来就带一个空格
  ok = verbatim('官方样例内容（去掉末尾换行后）', got.replace(/\n$/, ''), wantNoNL) && ok;
  const lines = got.replace(/\n$/, '').split('\n');
  console.log(`  格式体检：行数 ${lines.length}（应 = C(5,3) = ${C(5, 3)}），各行长度 [${lines.map((l) => l.length).join(',')}]（应为 9 = 3×3）`
    + `，行末多余空格行数 ${lines.filter((l) => / $/.test(l)).length}，程序输出末尾换行：${got.endsWith('\n') ? '有' : '无'}`
    + `（官方样例末行无换行；洛谷 checker 忽略行末空白与末尾换行，故按"内容逐字一致"判定）`);
  // 顺手证明"位掩码自然顺序 != 字典序"，这是 README 里那条易错点的实测依据
  const raw = [];
  for (let m = 0; m < 32; m++) {
    if (popcount(m) !== 3) continue;
    const c = []; for (let i = 0; i < 5; i++) if (m & (1 << i)) c.push(i + 1);
    raw.push(c.join(''));
  }
  console.log(`  位掩码从小到大直接输出的前 6 行：${raw.slice(0, 6).join(' | ')}（字典序应为 123|124|125|134|135|145）`
    + ` ⇒ 掩码顺序确实不是字典序，参照实现必须显式排序`);
}

// ===================== ② 随机对拍 =====================
{
  let bad = 0;
  const ROUNDS = Number(process.env.ROUNDS || 120);        // 用户要求：不必测特别多
  const seen = {};
  for (let t = 0; t < ROUNDS; t++) {
    const n = 2 + rnd(13);                                 // 2 <= n <= 14（2^14 个子集，参照实现瞬时）
    const r = rnd(n + 1);                                  // 0 <= r <= n，故意覆盖 r=0 和 r=n 两个边界
    seen[`n=${n},r=${r}`] = 1;
    const inp = `${n} ${r}\n`;
    const got = run(inp), want = refCombinations(n, r);
    if (got !== want) {
      bad++;
      if (bad === 1) console.log(`首个不一致 ${inp.trim()}：实际行数 ${got.replace(/\n$/, '').split('\n').length}，`
        + `参照行数 ${want.replace(/\n$/, '').split('\n').length}\n实际前 3 行 [${got.split('\n').slice(0, 3).join(' | ')}]`);
    }
  }
  console.log(`随机对拍 ${ROUNDS} 组（覆盖 ${Object.keys(seen).length} 种 (n,r) 组合，含 r=0/r=n），逐字不一致 ${bad} 组`);
  if (bad) ok = false;
  // 边界单报：r=0 时按"空组合"输出一个空行
  const z = run('5 0\n');
  console.log(`  边界 n=5 r=0：输出 ${JSON.stringify(z)}（1 个空行 = 空组合唯一一个），参照 ${JSON.stringify(refCombinations(5, 0))} ⇒ ${z === refCombinations(5, 0) ? '一致' : '不一致'}`);
}

// ===================== ③ 极限规模 n=20, r=10 =====================
{
  const n = 20, r = 10;
  const t0 = Date.now();
  const got = run(`${n} ${r}\n`);
  const ms = Date.now() - t0;                               // 含进程启动与本机安全软件扫描
  const t1 = Date.now();
  const want = refCombinations(n, r);
  const jsMs = Date.now() - t1;
  const lines = got.replace(/\n$/, '').split('\n');
  console.log(`极限 n=${n}, r=${r}：行数 ${lines.length}（= C(20,10) = ${C(20, 10)}），字符数 ${got.length}，`
    + `C++ 侧耗时 ${ms} ms，JS 参照（枚举 2^20 个子集 + 排序）耗时 ${jsMs} ms，逐字比对 ${got === want ? '一致 ✅' : '不一致 ❌'}`);
  console.log(`  行长度全部为 ${3 * r}？${lines.every((l) => l.length === 3 * r) ? '是' : '否'}；`
    + `是否严格字典序：${lines.every((l, i) => i === 0 || l > lines[i - 1]) ? '是（按行字符串比较单调递增）' : '否'}；`
    + `首行 [${lines[0]}]，末行 [${lines[lines.length - 1]}]`);
  if (got !== want) ok = false;
}

console.log(ok ? '结论：全部通过（本地验算，未提交洛谷）' : '结论：存在差异，见上方定位');
process.exit(ok ? 0 : 1);
