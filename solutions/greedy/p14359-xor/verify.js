// P14359 [CSP-J 2025] 异或和
// 对拍：greedy（移植 solution.cpp 逻辑） vs O(n^2) 区间调度 DP（真最优）
// 用法: node verify.js

function greedy(a, k) {
  const n = a.length;
  const sum = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) sum[i] = sum[i - 1] ^ a[i - 1];
  const d = new Map();          // 原代码用大数组，这里等价：值 -> 最近一次出现的前缀下标
  d.set(0, 0);
  let lst = 0, ans = 0;
  for (let i = 1; i <= n; i++) {
    const x = sum[i] ^ k;
    const j = d.has(x) ? d.get(x) : -1;
    if (j >= lst) { ans++; lst = i; }
    d.set(sum[i], i);
  }
  return ans;
}

// f[i] = 前 i 个元素里最多能选出多少个不相交区间
function dpBrute(a, k) {
  const n = a.length;
  const sum = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) sum[i] = sum[i - 1] ^ a[i - 1];
  const f = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) {
    f[i] = f[i - 1];
    for (let l = 1; l <= i; l++) {
      if ((sum[i] ^ sum[l - 1]) === k) f[i] = Math.max(f[i], f[l - 1] + 1);
    }
  }
  return f[n];
}

// f[i] = max(f[i-1], f[j]+1)，j 为 sum[j]==sum[i]^k 的最大下标。
// f 单调不减 => 取最近的 j 即取到最大 f[j]，所以这仍是真最优；用它反证贪心等价。
function dpLinear(a, k) {
  const n = a.length;
  const sum = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) sum[i] = sum[i - 1] ^ a[i - 1];
  const last = new Map();
  last.set(0, 0);
  const f = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) {
    f[i] = f[i - 1];
    const x = sum[i] ^ k;
    if (last.has(x)) f[i] = Math.max(f[i], f[last.get(x)] + 1);
    last.set(sum[i], i);
  }
  return f[n];
}

const samples = [
  { n: 4, k: 2, a: [2, 1, 0, 3], out: 2 },
  { n: 4, k: 3, a: [2, 1, 0, 3], out: 2 },
  { n: 4, k: 0, a: [2, 1, 0, 3], out: 1 },
];
console.log('--- 官方样例 ---');
for (const s of samples) {
  const g = greedy(s.a, s.k), b = dpBrute(s.a, s.k), l = dpLinear(s.a, s.k);
  console.log(`k=${s.k} a=[${s.a}] 贪心=${g} O(n^2)DP=${b} O(n)DP=${l} 期望=${s.out} ${g === s.out && b === s.out ? 'OK' : 'FAIL'}`);
}

console.log('--- 随机对拍（含 k=0 与特殊性质 A/B/C）---');
let bad = 0, tested = 0;
const rnd = (V) => Math.floor(Math.random() * V);
for (let t = 0; t < 60000; t++) {
  const n = 1 + rnd(11);
  const mode = rnd(4); // 0 一般, 1 性质A(a_i=1), 2 性质B(0/1), 3 性质C(0..255)
  let V = 8;
  if (mode === 1) V = 2;
  if (mode === 2) V = 2;
  if (mode === 3) V = 256;
  const a = Array.from({ length: n }, () => mode === 1 ? 1 : rnd(V));
  const k = rnd(4) === 0 ? 0 : rnd(mode === 1 ? 2 : Math.min(V, 256));
  const g = greedy(a, k), b = dpBrute(a, k), l = dpLinear(a, k);
  tested++;
  if (g !== b || g !== l) {
    bad++;
    if (bad <= 5) console.log(`不一致 mode=${mode} k=${k} a=[${a}] 贪心=${g} O(n^2)DP=${b} O(n)DP=${l}`);
  }
}
console.log(`共 ${tested} 组，贪心 / O(n^2) DP / O(n) DP 三方不一致 ${bad} 组`);

console.log('--- 刻意构造：贪心是否会"早收割"导致变差 ---');
const tricky = [
  { k: 0, a: [0] },
  { k: 0, a: [1, 1, 0] },
  { k: 0, a: [5, 5, 5, 5] },
  { k: 1, a: [1, 0, 1, 0, 1] },
  { k: 3, a: [3, 0, 0, 3, 3] },
  { k: 2, a: [2, 2, 2] },
  { k: 7, a: [7, 7, 7, 7, 7] },
];
for (const s of tricky) {
  const g = greedy(s.a, s.k), b = dpBrute(s.a, s.k), l = dpLinear(s.a, s.k);
  console.log(`k=${s.k} a=[${s.a}] 贪心=${g} 最优=${b} 线性DP=${l} ${g === b && g === l ? '一致' : '不一致'}`);
}

console.log('--- 原代码里 d 数组越界风险扫描（sum 与 x 的上界）---');
let maxIdx = 0;
for (let t = 0; t < 3000; t++) {
  const n = 1 + rnd(12);
  const a = Array.from({ length: n }, () => rnd(1 << 20));
  const k = rnd(1 << 20);
  const sum = new Array(n + 1).fill(0);
  for (let i = 1; i <= n; i++) sum[i] = sum[i - 1] ^ a[i - 1];
  for (let i = 1; i <= n; i++) maxIdx = Math.max(maxIdx, sum[i], sum[i] ^ k);
}
console.log(`随机数据中 d[] 下标最大值 = ${maxIdx}（< 2^20 = ${(1 << 20) - 1}）`);
