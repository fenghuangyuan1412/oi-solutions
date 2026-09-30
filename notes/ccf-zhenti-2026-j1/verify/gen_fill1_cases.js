// 生成完善程序(1) 的对拍数据：n,m ∈ [2,10]，mn 进制、位数 d ∈ [1,18]，值 < 2^63
const fs = require('fs');
function rnd(a, b) { return a + Math.floor(Math.random() * (b - a + 1)); }
const T = Number(process.argv[2] || 300);
for (let t = 1; t <= T; t++) {
  const n = rnd(2, 10), m = rnd(2, 10), base = n * m;
  const d = rnd(1, 18);
  const digs = [];
  for (let i = 0; i < d; i++) digs.push(rnd(0, base - 1));
  // 保证 A < 2^63：超了就砍掉高位
  let v = 0n, ok = true;
  for (const x of digs) { v = v * BigInt(base) + BigInt(x); if (v >= (1n << 63n)) { ok = false; break; } }
  if (!ok) { t--; continue; }
  fs.writeFileSync(`cases1/in${String(t).padStart(4, '0')}.txt`, `${n} ${m} ${d}\n${digs.join(' ')}\n`);
}
console.log('cases1 生成 ' + T + ' 组');
