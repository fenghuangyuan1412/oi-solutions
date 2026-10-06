// verify.js —— 仅用于验证，非讲解代码。
// 确定性扫 n=1..60（共 60 组，≤100 组上限内）：比较正解 solution.cpp（逐位贪心）
// 与骗分版 solution_partial.cpp（DFS 回溯）的输出是否逐字一致。
// 用法：node verify.js   （需 g++；产物写到仓库外的 .raw/bin，题解目录不留 exe）
"use strict";
const { execSync } = require("child_process");
const fs = require("fs"), path = require("path");

const dir = __dirname;
const raw = path.resolve(dir, "../../../..", ".raw");
fs.mkdirSync(path.join(raw, "bin"), { recursive: true });
fs.mkdirSync(path.join(raw, "tests"), { recursive: true });

function build(src, out) {
  execSync(`g++ -static -O2 -std=c++14 "${path.join(dir, src)}" -o "${path.join(raw, "bin", out)}"`, { stdio: "pipe" });
  return path.join(raw, "bin", out);
}
function run(exe, input) {
  const inf = path.join(raw, "tests", "verify_in.txt");
  fs.writeFileSync(inf, input);
  return execSync(`"${exe}" < "${inf}"`, { shell: true }).toString().trim().split(/\r?\n/);
}

const g = build("solution.cpp", "p11229_greedy.exe");
const d = build("solution_partial.cpp", "p11229_dfs.exe");

// 1) 官方样例
const sample = run(g, "5\n1\n2\n3\n6\n18\n");
const expect = ["-1", "1", "7", "6", "208"];
console.log("样例:", sample.join(" "), sample.join(" ") === expect.join(" ") ? "OK" : "FAIL");

// 2) n=1..60 全量对比
let inp = "60\n";
for (let n = 1; n <= 60; ++n) inp += n + "\n";
const a = run(g, inp), b = run(d, inp);
let bad = 0;
for (let i = 0; i < 60; ++i) if (a[i] !== b[i]) { bad++; console.log("不一致 n=" + (i + 1), a[i], b[i]); }
console.log("n=1..60 对比:", bad === 0 ? "60/60 一致" : bad + " 组不一致");
