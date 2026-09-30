// 完善程序(2) 平衡分割：n ∈ [2,20]，每位是 0-9 / A-F
const fs = require('fs');
const HEX = '0123456789ABCDEF';
const T = Number(process.argv[2] || 300);
fs.mkdirSync('cases2', { recursive: true });
for (let t = 1; t <= T; t++) {
  const n = 2 + Math.floor(Math.random() * 19);
  let s = '';
  for (let i = 0; i < n; i++) s += HEX[Math.floor(Math.random() * 16)];
  fs.writeFileSync(`cases2/in${String(t).padStart(4, '0')}.txt`, `${n} ${s}\n`);
}
console.log('cases2 生成 ' + T + ' 组');
