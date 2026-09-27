// 对拍：user 逻辑（精确移植 P14358 代码） vs 暴力模拟蛇形按列排座
function userLogic(n, m, a) {
  const tot = n * m;
  const nodes = a.map((x, i) => ({ x, id: i + 1 }));
  nodes.sort((p, q) => q.x - p.x);
  let ans;
  for (let i = 0; i < tot; i++) if (nodes[i].id === 1) ans = i + 1;
  const a1 = Math.ceil((ans * 1.0) / n);
  if (a1 & 1) return [a1, ans - (a1 - 1) * n];
  return [a1, a1 * n - ans + 1];
}

function brute(n, m, a) {
  const order = a.map((x, i) => ({ x, i })).sort((p, q) => q.x - p.x);
  let col = 1, row = 1, dir = 1;
  const pos = new Array(order.length);
  for (let k = 0; k < order.length; k++) {
    pos[order[k].i] = [col, row];
    row += dir;
    if (row > n) { col++; row = n; dir = -1; }
    else if (row < 1) { col++; row = 1; dir = 1; }
  }
  return pos[0];
}

const samples = [
  [[2, 2, [99, 100, 97, 98]], '1 2'],
  [[2, 2, [98, 99, 100, 97]], '2 2'],
  [[3, 3, [94, 95, 96, 97, 98, 99, 100, 93, 92]], '3 1'],
];
console.log('--- 官方样例 ---');
for (const [[n, m, a], exp] of samples) {
  const u = userLogic(n, m, a).join(' ');
  const b = brute(n, m, a).join(' ');
  console.log(`n=${n} m=${m} 你的=${u} 暴力=${b} 期望=${exp} ${u === exp ? 'OK' : 'FAIL'}`);
}

console.log('--- 随机对拍 ---');
let bad = 0, tested = 0;
for (let t = 0; t < 200000; t++) {
  const n = 1 + Math.floor(Math.random() * 10);
  const m = 1 + Math.floor(Math.random() * 10);
  const tot = n * m;
  const pool = [];
  for (let v = 1; v <= 100; v++) pool.push(v);
  for (let i = pool.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [pool[i], pool[j]] = [pool[j], pool[i]];
  }
  const a = pool.slice(0, tot);
  const u = userLogic(n, m, a).join(' ');
  const b = brute(n, m, a).join(' ');
  tested++;
  if (u !== b) {
    bad++;
    if (bad <= 3) console.log(`不一致 n=${n} m=${m} a=[${a}] 你的=${u} 暴力=${b}`);
  }
}
console.log(`共 ${tested} 组，不一致 ${bad} 组`);

console.log('--- 特殊性质 A / B 边界 ---');
for (const nm of [[1, 1], [1, 10], [10, 1], [10, 10], [2, 2], [10, 2]]) {
  const [n, m] = nm, tot = n * m;
  const A = Array.from({ length: tot }, (_, i) => i + 1);
  const B = Array.from({ length: tot }, (_, i) => tot - i);
  console.log(`n=${n} m=${m} A: 你=${userLogic(n, m, A)} 暴=${brute(n, m, A)} | B: 你=${userLogic(n, m, B)} 暴=${brute(n, m, B)}`);
}
