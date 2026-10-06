/*
 * verify.js —— 仅用于验证，非讲解代码
 * 把 solution.cpp（排序+二分的正解）与 solution_partial.cpp（O(nm) 暴力）
 * 在多组随机小数据上对拍，确认"整数判据 + 单调二分求超速区间"与暴力枚举等价。
 *
 * 前置：先按 README 的编译命令把两个 exe 生成到 E:/ai/suanfa_study/.raw/bin/
 *   g++ -static -O2 -std=c++14 solution.cpp        -o E:/ai/suanfa_study/.raw/bin/p11232_full.exe
 *   g++ -static -O2 -std=c++14 solution_partial.cpp -o E:/ai/suanfa_study/.raw/bin/p11232_partial.exe
 * 运行： node verify.js [组数]（默认 300）
 */
const { execFileSync } = require('child_process');
const BIN = 'E:/ai/suanfa_study/.raw/bin/';
const N = Number(process.argv[2] || 300);

function run(exe, inp) {
    return execFileSync(BIN + exe, [], { input: inp, encoding: 'utf8' }).trim();
}

let bad = 0;
for (let t = 0; t < N; ++t) {
    const n = 1 + Math.floor(Math.random() * 8);
    const m = 1 + Math.floor(Math.random() * 8);
    const L = 1 + Math.floor(Math.random() * 30);
    const V = 1 + Math.floor(Math.random() * 5);
    let s = `1\n${n} ${m} ${L} ${V}\n`;
    for (let i = 0; i < n; ++i) {
        const d = Math.floor(Math.random() * (L + 1));
        const v = 1 + Math.floor(Math.random() * 8);
        const a = Math.floor(Math.random() * 11) - 5;
        s += `${d} ${v} ${a}\n`;
    }
    const ps = [];
    for (let j = 0; j < m; ++j) ps.push(Math.floor(Math.random() * (L + 1)));
    s += ps.join(' ') + '\n';

    const A = run('p11232_full.exe', s);
    const B = run('p11232_partial.exe', s);
    if (A !== B) { bad++; console.log('MISMATCH\n' + s + 'full=' + A + ' partial=' + B); }
}
console.log(`done ${N} 组, 不一致 ${bad} 组`);
