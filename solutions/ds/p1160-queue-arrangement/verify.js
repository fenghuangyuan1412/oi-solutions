// P1160 队列安排 · 对拍脚本（仅用于验证，非讲解代码；讲解代码见 solution.cpp）
// 用法：node verify.js             —— 官方样例 + JS 双实现随机对拍 3000 组 + 定向边界
//       node verify.js --exe PATH  —— 额外把编译好的 C++ 程序逐组跑 300 组（含 n=m=1000 规模）
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// 「正解」思路：数组模拟双向链表 + 0 号哨兵，插入/删除只改指针，O(N+M)
function linkedSolve(n, ops, dels) {
  const L = new Int32Array(n + 1), R = new Int32Array(n + 1);
  const gone = new Uint8Array(n + 1);
  L[0] = R[0] = 0;
  L[1] = R[1] = 0; R[0] = 1; L[0] = 1;          // 先把 1 号挂到哨兵上
  for (const [i, k, p] of ops) {
    if (p === 0) { const x = L[k]; R[x] = i; L[i] = x; L[k] = i; R[i] = k; }
    else { const y = R[k]; L[y] = i; R[i] = y; R[k] = i; L[i] = k; }
  }
  for (const x of dels) {
    if (gone[x]) continue;                       // 重复删除指令要忽略
    gone[x] = 1; R[L[x]] = R[x]; L[R[x]] = L[x];
  }
  const out = [];
  for (let x = R[0]; x !== 0; x = R[x]) out.push(x);
  return out;
}

// 「暴力」思路：独立实现 —— 直接把队列存成数组，插入用 splice，删除用 indexOf+splice
// 完全按题面字面操作，复杂度 O(N^2 + N*M)，但绝不依赖指针技巧，适合当参照
function vectorSolve(n, ops, dels) {
  const v = [1];
  for (const [i, k, p] of ops) {
    const pos = v.indexOf(k);                    // 插入时 k 一定还在队列里（删除还没开始）
    v.splice(pos + (p === 1 ? 1 : 0), 0, i);
  }
  for (const x of dels) {
    const pos = v.indexOf(x);                    // 已经不在了就找不到，直接忽略
    if (pos >= 0) v.splice(pos, 1);
  }
  return v;
}

const rnd = (lo, hi) => lo + Math.floor(Math.random() * (hi - lo + 1));

// 生成一组数据：返回 { input, n }
function genCase(nMax, mMax) {
  const n = rnd(2, nMax);
  const ops = [];
  for (let i = 2; i <= n; i++) ops.push([i, rnd(1, i - 1), rnd(0, 1)]);
  const m = rnd(1, mMax);
  const dels = [];
  for (let t = 0; t < m; t++) dels.push(rnd(1, n));
  const lines = [String(n)];
  for (const [, k, p] of ops) lines.push(k + ' ' + p);
  lines.push(String(m));
  for (const x of dels) lines.push(String(x));
  return { input: lines.join('\n') + '\n', n, m, ops, dels };
}

function fmt(arr) { return arr.join(' '); }

// ---------- ① 官方样例 ----------
console.log('--- 官方样例（4 / 1 0 / 2 1 / 1 0 / M=2 / 3 / 3）---');
{
  const s = '4\n1 0\n2 1\n1 0\n2\n3\n3\n';
  const ops = [[2, 1, 0], [3, 2, 1], [4, 1, 0]], dels = [3, 3];
  const got = fmt(linkedSolve(4, ops, dels));
  console.log('链表=' + got + ' | vector暴力=' + fmt(vectorSolve(4, ops, dels)) + ' | 期望=2 4 1');
  console.log(got === '2 4 1' ? 'OK' : 'FAIL');
}

// ---------- ② 随机对拍 ----------
console.log('--- 随机对拍：数组双向链表 vs vector 直接插删 ---');
let bad = 0, rounds = 3000;
for (let r = 0; r < rounds; r++) {
  const c = genCase(12, 12);
  const a = fmt(linkedSolve(c.n, c.ops, c.dels));
  const b = fmt(vectorSolve(c.n, c.ops, c.dels));
  if (a !== b) { bad++; if (bad <= 3) console.log('不一致 n=' + c.n + ' m=' + c.m + '\n' + c.input + '链表=[' + a + '] 暴力=[' + b + ']'); }
}
console.log('共 ' + rounds + ' 组（n,m<=12，含重复删除、删空队列），不一致 ' + bad + ' 组');

// ---------- ③ 定向边界 ----------
console.log('--- 定向边界 ---');
const edge = [
  { name: 'n=2，把唯一的另一个人删光', n: 2, ops: [[2, 1, 0]], dels: [2], want: '1' },
  { name: 'n=2，删掉 1 号（队头）', n: 2, ops: [[2, 1, 0]], dels: [1], want: '2' },
  { name: '都插 1 号左边 → 2 一直在最前', n: 5, ops: [[2, 1, 0], [3, 1, 0], [4, 1, 0], [5, 1, 0]], dels: [], want: '2 3 4 5 1' },
  { name: '每次都插最前面那人的左边 → 逆序', n: 5, ops: [[2, 1, 0], [3, 2, 0], [4, 3, 0], [5, 4, 0]], dels: [], want: '5 4 3 2 1' },
  { name: '全部插右边（每次插在前一人右）→ 顺序', n: 5, ops: [[2, 1, 1], [3, 2, 1], [4, 3, 1], [5, 4, 1]], dels: [], want: '1 2 3 4 5' },
  { name: '都插 1 号右边 → 逆序套在 1 后', n: 5, ops: [[2, 1, 1], [3, 1, 1], [4, 1, 1], [5, 1, 1]], dels: [], want: '1 5 4 3 2' },
  { name: '同一个人重复删 5 次', n: 3, ops: [[2, 1, 1], [3, 2, 1]], dels: [2, 2, 2, 2, 2], want: '1 3' },
];
for (const e of edge) {
  const got = fmt(linkedSolve(e.n, e.ops, e.dels));
  const v = fmt(vectorSolve(e.n, e.ops, e.dels));
  console.log(e.name + ' -> ' + (got === e.want && v === e.want ? 'OK' : 'FAIL') + ' [' + got + ']');
}

// ---------- ④ 可选：与编译好的 C++ 程序交叉验证 ----------
const argIdx = process.argv.indexOf('--exe');
if (argIdx >= 0) {
  const exe = process.argv[argIdx + 1] || path.join(__dirname, 'sol.exe');
  console.log('--- C++ 交叉验证：' + exe + ' ---');
  if (!fs.existsSync(exe)) { console.log('找不到可执行文件，先跑 g++ -static -O2 -std=c++14 solution.cpp -o sol.exe'); process.exit(1); }
  let cbad = 0, crounds = 300;
  for (let r = 0; r < crounds; r++) {
    const big = r < 5;                              // 前 5 组上规模，防止只有小数在测
    const c = genCase(big ? 1000 : 30, big ? 1000 : 30);
    const got = execFileSync(exe, [], { input: c.input }).toString().replace(/\s+$/, '').replace(/\r/g, '');
    const want = fmt(vectorSolve(c.n, c.ops, c.dels));
    if (got !== want) { cbad++; if (cbad <= 3) console.log('C++ 与暴力不一致 n=' + c.n + ' m=' + c.m); }
  }
  console.log('共 ' + crounds + ' 组（含 5 组 n=m<=1000），不一致 ' + cbad + ' 组');
}
