// verify.js —— 仅用于验证，非讲解代码。P9753 消消乐。
//
//   node verify.js            默认：穷举自检 + 编译正解 + 504 组 + 135 组对撞
//   node verify.js partial    再把 solution_partial.cpp（区间 DP）编出来对撞同一批 case
//
// 三层参照，全部只用"题面定义"，不借用正解的任何结论：
//   refDFS(s)    —— 严格按题意：枚举每个子串，记忆化 DFS 试删相邻相同对，看能否删空
//   refStack(s)  —— 每个子串单独做一次栈约简（不借用"前缀配对"）
//   jsCorrect(s) —— 正解的 JS 镜像（前缀栈状态树 + 按 (父编号, 字符) 发号），只为把 23490 组穷举跑完
//   jsDepthBug(s)—— 上一版错误实现（按"栈深"发编号）的镜像，用来量化"错一步会错多少组"
// 真正的 C++ 可执行文件只测抽样的 504 组（n<=35）与 135 组（n<=300）——那是 exe 逐次起进程的成本上限。
// 用法与产物：需要 g++；exe 写到仓库外的 .raw/bin，题解目录不留二进制。
"use strict";
const { execSync, spawnSync } = require("child_process");
const fs = require("fs"), path = require("path");

const dir = __dirname;
const raw = path.resolve(dir, "../../../..", ".raw");
fs.mkdirSync(path.join(raw, "bin"), { recursive: true });

// ---------- 参照实现 ----------
function canElim(t, memo) {
  if (t === "") return true;
  if (t.length % 2 === 1) return false;
  const v = memo.get(t);
  if (v !== undefined) return v;
  let ok = false;
  for (let i = 1; i < t.length && !ok; i++) {
    if (t[i] === t[i - 1] && canElim(t.slice(0, i - 1) + t.slice(i + 1), memo)) ok = true;
  }
  memo.set(t, ok);
  return ok;
}
function refDFS(s) {
  const memo = new Map();
  let a = 0;
  for (let l = 0; l < s.length; l++)
    for (let r = l + 1; r <= s.length; r++)
      if (canElim(s.slice(l, r), memo)) a++;
  return a;
}
function refStack(s) {
  let a = 0;
  for (let l = 0; l < s.length; l++) {
    const st = [];
    for (let r = l; r < s.length; r++) {
      if (st.length && st[st.length - 1] === s[r]) st.pop(); else st.push(s[r]);
      if (st.length === 0) a++;
    }
  }
  return a;
}
// 正解的 JS 镜像：栈内容构成一棵树，按 (父编号, 字符) 发号，数相同前缀状态的对数
function jsCorrect(s) {
  const child = new Map(), parent = [0], cnt = new Map();
  let next = 1, cur = 0, ans = 0;
  const st = [];
  cnt.set(0, 1);                       // 空前缀也是状态
  for (const c of s) {
    if (st.length && st[st.length - 1] === c) { st.pop(); cur = parent[cur]; }
    else {
      const k = cur + ">" + c;
      let id = child.get(k);
      if (id === undefined) { id = next++; child.set(k, id); parent[id] = cur; }
      cur = id; st.push(c);
    }
    const v = cnt.get(cur) || 0; ans += v; cnt.set(cur, v + 1);
  }
  return ans;
}
// 错误镜像：只按"栈深"发编号（把深度 1 的 a 和深度 1 的 b 当同一个状态）
function jsDepthBug(s) {
  const cnt = new Map();
  let depth = 0, ans = 0;
  const st = [];
  cnt.set(0, 1);
  for (const c of s) {
    if (st.length && st[st.length - 1] === c) { st.pop(); depth--; } else { st.push(c); depth++; }
    const v = cnt.get(depth) || 0; ans += v; cnt.set(depth, v + 1);
  }
  return ans;
}
const ref = (s) => (s.length <= 12 ? refDFS(s) : refStack(s));

// ---------- 数据：全部确定性生成，不读外部文件 ----------
function rnd(seed) { let x = seed >>> 0; return () => (x = (x * 1664525 + 1013904223) >>> 0) / 4294967296; }
const ALPHABET = "abcdefghijklmnopqrstuvwxyz";
const exhaust = [];
for (const [ab, maxLen] of [[["a", "b"], 12], [["a", "b", "c"], 8], [["a", "b", "c", "d"], 6]]) {
  const cur = [];
  (function walk(depth) {
    if (depth > 0) exhaust.push(cur.join(""));
    if (depth === maxLen) return;
    for (const c of ab) { cur.push(c); walk(depth + 1); cur.pop(); }
  })(0);
}
const r1 = rnd(4242);
const cases504 = [];
for (let i = 0; i < exhaust.length; i += 53) cases504.push(exhaust[i]);
for (let t = 0; t < 60; t++) {
  const n = 11 + Math.floor(r1() * 25);
  cases504.push(Array.from({ length: n }, () => "abc"[Math.floor(r1() * 3)]).join(""));
}
const r2 = rnd(777);
const cases135 = [];
for (let t = 0; t < 120; t++) {
  const n = 11 + Math.floor(r2() * 40);
  const A = t % 3 === 0 ? "ab" : (t % 3 === 1 ? "abc" : "abcdefghij");
  cases135.push(Array.from({ length: n }, () => A[Math.floor(r2() * A.length)]).join(""));
}
for (const n of [60, 100, 150, 200, 300]) {
  cases135.push("a".repeat(n));
  cases135.push(Array.from({ length: n }, (_, i) => (i % 2 ? "b" : "a")).join(""));
  cases135.push(Array.from({ length: n }, () => ALPHABET[Math.floor(r2() * 10)]).join(""));
}

// ---------- 编译与跑 exe ----------
function build(src, out) {
  execSync(`g++ -static -O2 -std=c++14 "${path.join(dir, src)}" -o "${path.join(raw, "bin", out)}"`, { stdio: "pipe" });
  return path.join(raw, "bin", out);
}
function runExe(exe, s) {
  const r = spawnSync(exe, [], { input: `${s.length}\n${s}\n`, encoding: "utf8", maxBuffer: 1 << 26, timeout: 60000 });
  if (r.status !== 0) throw new Error(`${exe} 退出码 ${r.status}: ${r.stderr.slice(0, 200)}`);
  return r.stdout.trim();
}
function vsRef(exe, cases, tag) {
  let bad = 0;
  for (const s of cases) {
    const got = runExe(exe, s), exp = String(ref(s));
    if (got !== exp) { bad++; if (bad <= 5) console.log(`  [MISMATCH ${tag}] n=${s.length} s=${s} exe=${got} ref=${exp}`); }
  }
  console.log(`${tag}: ${cases.length} 组，不一致 ${bad}`);
  return bad;
}

// ---------- 1) 纯 JS 的穷举自检（不起进程，23490 组全跑） ----------
let t0 = Date.now(), bugDiff = 0, refDiff = 0, mirrorDiff = 0;
for (const s of exhaust) {
  const a = refDFS(s), b = refStack(s), c = jsCorrect(s);
  if (a !== b) refDiff++;
  if (c !== a) mirrorDiff++;
  if (jsDepthBug(s) !== a) bugDiff++;
}
console.log(`穷举 ${exhaust.length} 组（DFS 定义 vs 逐子串栈约简）：不一致 ${refDiff}`);
console.log(`穷举 ${exhaust.length} 组（正解 JS 镜像 vs 定义）：不一致 ${mirrorDiff}`);
console.log(`错误版镜像（按栈深发号）vs 定义：不一致 ${bugDiff} 组`);
console.log(`样例核对：aab 正确 ${ref("aab")} / 错误版 ${jsDepthBug("aab")}；accabccb 正确 ${ref("accabccb")} / 错误版 ${jsDepthBug("accabccb")}  （耗时 ${Date.now() - t0} ms）`);
if (refDiff || mirrorDiff) { console.log("!! JS 参照之间就不一致，脚本或镜像写错了"); process.exit(1); }

// ---------- 2) 官方样例 + 抽样对撞真编译产物 ----------
const g = build("solution.cpp", "p9753_full.exe");
console.log("官方样例 8 / accabccb ->", runExe(g, "accabccb"), "（应为 5）");
vsRef(g, cases504, "正解 solution.cpp vs 定义，504 组 n<=35");
vsRef(g, cases135, "正解 solution.cpp vs 定义，135 组 n<=300");

if (process.argv[2] === "partial") {
  const p = build("solution_partial.cpp", "p9753_partial.exe");
  const small = cases504.filter((s) => s.length <= 40);
  vsRef(p, small, "骗分版 solution_partial.cpp（区间 DP）vs 定义，抽 n<=40 的 " + small.length + " 组");
}
