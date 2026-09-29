// 自编模拟卷的验证驱动：编译 → 跑官方样例 → 与暴力随机对拍 → 极限数据计时。
//
//   node notes/mock-cspj-round2/verify.js set1              # 只跑样例 + 对拍
//   node notes/mock-cspj-round2/verify.js set1 --rounds 800 # 指定对拍组数
//   node notes/mock-cspj-round2/verify.js all --perf        # 四套全跑，并计时
//
// 结果一律写进 notes/mock-cspj-round2/verify/RESULTS.md（题解里出现的每个数字都来自这里）。
// 生成器、样例、极限数据都写在本文件的 SPEC 里，一题一条。

const fs = require('fs');
const path = require('path');
const { execFileSync } = require('child_process');

const ROOT = path.resolve(__dirname);
const GXX = 'g++';
const FLAGS = ['-static', '-O2', '-std=c++14'];

function arg(name, dflt) {
  const i = process.argv.indexOf('--' + name);
  return i > 0 && process.argv[i + 1] && !process.argv[i + 1].startsWith('--') ? process.argv[i + 1] : dflt;
}
const has = (n) => process.argv.indexOf('--' + n) > 0;

// 可复现随机数（mulberry32）。注意：不能用 x*A+B 的朴素 LCG——
// JS 的 double 在 x*A 超过 2^53 时会丢掉低位，随机数会严重退化。
function rng(seed) {
  let a = seed >>> 0;
  return function (n) { // [0, n)
    a = (a + 0x6d2b79f5) >>> 0;
    let t = Math.imul(a ^ (a >>> 15), 1 | a);
    t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
    return ((t ^ (t >>> 14)) >>> 0) % n;
  };
}
const ri = (r, lo, hi) => lo + r(hi - lo + 1);
const shuffle = (r, a) => { for (let i = a.length - 1; i > 0; i--) { const j = r(i + 1);[a[i], a[j]] = [a[j], a[i]]; } return a; };
const digits = (r, len) => Array.from({ length: len }, () => String(ri(r, 0, 9))).join('');
function mixed(r, len, dmin, dmax) {
  const nd = ri(r, dmin, Math.min(dmax, len));
  let s = digits(r, nd);
  for (let i = 0; i < len - nd; i++) s += String.fromCharCode(97 + r(26));
  return shuffle(r, s.split('')).join('');
}

const SPEC = {
  'set1/t1-plate': {
    samples: [
      ['a1b0c2\n', '102\n'],
      ['x000y5\n', '5000\n'],
      ['9z8k7\n', '789\n'],
    ],
    gen(r) {
      const nd = ri(r, 1, 7);
      let d = digits(r, nd);
      if (!/[1-9]/.test(d)) d = String(ri(r, 1, 9)) + d.slice(0, nd - 1); // 保证有非零数字
      const noise = Array.from({ length: ri(r, 0, 12) }, () => String.fromCharCode(97 + r(26)));
      return shuffle(r, noise.concat(d.split(''))).join('') + '\n';
    },
    rounds: 500,
    perf: () => mixed(rng(7), 1000000, 500000, 900000) + '1\n',
  },
  'set1/t2-badge': {
    samples: [
      ['3 3 40\n50 60 70 80 90 10 20 30 40\n', '3 2\n'],
      ['2 3 8\n5 6 7 8 9 10\n', '1 3\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 8), m = ri(r, 1, 8), tot = n * m;
      const vals = shuffle(r, Array.from({ length: tot }, (_, i) => i + 1)).map((v) => v * 7 + ri(r, 0, 3));
      const uniq = [];
      const seen = new Set();
      for (const v of vals) if (!seen.has(v)) { seen.add(v); uniq.push(v); }
      while (uniq.length < tot) uniq.push(1000 + uniq.length);
      const x = uniq[r(tot)];
      return `${n} ${m} ${x}\n${uniq.join(' ')}\n`;
    },
    rounds: 400,
    perf: () => {
      const n = 30, m = 30, tot = n * m;
      const v = Array.from({ length: tot }, (_, i) => i * 3 + 1);
      return `${n} ${m} ${v[tot - 1]}\n${v.join(' ')}\n`;
    },
  },
  'set1/t3-combo': {
    samples: [
      ['6 5\n2 3 1 4 2 3\n', '3\n'],
      ['3 7\n3 3 3\n', '0\n'],
      ['8 3\n1 2 1 2 1 1 1 3\n', '4\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 60), k = ri(r, 1, 40);
      const a = Array.from({ length: n }, () => ri(r, 1, 8));
      return `${n} ${k}\n${a.join(' ')}\n`;
    },
    rounds: 600,
    perf: () => {
      const g = rng(11);
      const n = 1000000, k = 1000;
      const a = Array.from({ length: n }, () => ri(g, 1, 1000));
      return `${n} ${k}\n${a.join(' ')}\n`;
    },
  },
  'set1/t4-clip': {
    samples: [
      ['3 3\n1 2 3\n', '3\n'],
      ['4 5\n2 2 2 2\n', '5\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 16), S = ri(r, 0, 60);
      const a = Array.from({ length: n }, () => ri(r, 1, 15));
      return `${n} ${S}\n${a.join(' ')}\n`;
    },
    rounds: 500,
    perf: () => {
      const g = rng(13);
      const n = 300, S = 45000;
      const a = Array.from({ length: n }, () => ri(g, 1, 300));
      return `${n} ${S}\n${a.join(' ')}\n`;
    },
  },

  'set2/t1-codex': {
    samples: [
      ['5 8\n1 2 2 3 1\n', '5\n'],
      ['3 3\n1 2 3\n', '0\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 25), m = ri(r, 1, 25);
      const c = Array.from({ length: n }, () => ri(r, 1, m));
      return `${n} ${m}\n${c.join(' ')}\n`;
    },
    rounds: 500,
    perf: () => {
      const g = rng(17);
      const n = 1000000, m = 1000000;
      const c = Array.from({ length: n }, () => ri(g, 1, m));
      return `${n} ${m}\n${c.join(' ')}\n`;
    },
  },
  'set2/t2-robot': {
    samples: [
      ['3 3 5\n...\n.x.\n...\n1 1 0\nFFFRF\n', '1 2 2 3\n'],
      ['3 3 6\n..x\n...\nx..\n1 1 0\nFFFFFF\n', '3 2 3 4\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 7), m = ri(r, 1, 7), k = ri(r, 1, 30);
      const g = [];
      for (let i = 0; i < n; i++) {
        let s = '';
        for (let j = 0; j < m; j++) s += r(4) === 0 ? 'x' : '.';
        g.push(s);
      }
      const free = [];
      for (let i = 0; i < n; i++) for (let j = 0; j < m; j++) if (g[i][j] === '.') free.push([i + 1, j + 1]);
      if (!free.length) return `1 1 1\n.\n1 1 0\nF\n`;
      const [sr, sc] = free[r(free.length)];
      let op = '';
      for (let t = 0; t < k; t++) op += r(3) === 0 ? 'R' : 'F';
      return `${n} ${m} ${k}\n${g.join('\n')}\n${sr} ${sc} ${r(4)}\n${op}\n`;
    },
    rounds: 600,
    perf: () => {
      const g = rng(19);
      const n = 1000, m = 1000, k = 1000000;
      const rows = Array.from({ length: n }, () => Array.from({ length: m }, () => (ri(g, 1, 5) === 1 ? 'x' : '.')).join(''));
      const op = Array.from({ length: k }, () => (g(3) === 0 ? 'R' : 'F')).join('');
      return `${n} ${m} ${k}\n${rows.join('\n')}\n1 1 0\n${op}\n`;
    },
  },
  'set2/t3-light': {
    samples: [
      ['5\n', '2\n'],
      ['8\n', '10\n'],
      ['1\n', '-1\n'],
    ],
    gen(r) {
      return `${ri(r, 1, 120)}\n`;
    },
    rounds: 400,
    perf: () => `${100000}\n`,
  },
  'set2/t4-vote': {
    samples: [
      ['3 6\n2 1 2\n2 2 3\n1 3\n', '1\n'],
      ['2 4\n2 1 3\n2 1 3\n', '2\n'],
      ['1 5\n3 5 5 2\n', '2\n'],
    ],
    gen(r) {
      const m = ri(r, 1, 6), S = ri(r, 1, 40);
      const rows = [];
      for (let i = 0; i < m; i++) {
        const k = ri(r, 1, 5);
        const v = Array.from({ length: k }, () => ri(r, 1, 12));
        rows.push([k, ...v].join(' '));
      }
      return `${m} ${S}\n${rows.join('\n')}\n`;
    },
    rounds: 400,
    perf: () => {
      const g = rng(23);
      const m = 100, S = 5000;
      const rows = Array.from({ length: m }, () => {
        const k = 100;
        const v = Array.from({ length: k }, () => ri(g, 1, 100));
        return [k, ...v].join(' ');
      });
      return `${m} ${S}\n${rows.join('\n')}\n`;
    },
  },

  'set3/t1-lineup': {
    samples: [
      ['6 1\n', '4 1\n'],
      ['6 5\n', '4 4\n'],
      ['1 1\n', '1 1\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 400);
      return `${n} ${ri(r, 1, n)}\n`;
    },
    rounds: 400,
    perf: () => `1000000000 999999937\n`,
  },
  'set3/t2-potion': {
    samples: [
      ['5 10\n10 10 10 10\n9 8 9 6 5\n', '31\n'],
      ['3 5\n10 5\n3 10 2\n', '9\n'],
    ],
    gen(r) {
      const n = ri(r, 2, 30), d = ri(r, 1, 10);
      const v = Array.from({ length: n - 1 }, () => d * ri(r, 1, 6));
      const a = Array.from({ length: n }, () => ri(r, 1, 100));
      return `${n} ${d}\n${v.join(' ')}\n${a.join(' ')}\n`;
    },
    rounds: 400,
    perf: () => {
      const g = rng(29);
      const n = 100000, d = 7;
      const v = Array.from({ length: n - 1 }, () => d * ri(g, 1, 14000));
      const a = Array.from({ length: n }, () => ri(g, 1, 100000));
      return `${n} ${d}\n${v.join(' ')}\n${a.join(' ')}\n`;
    },
  },
  'set3/t3-dmg': {
    samples: [
      ['8\n1 0 -2\n1 1 -1\n1 -3 1\n1 -2 -1\n1 -3 2\n1 0 1\n-1 1 1\n1 0 0\n',
        '√2\n(-1+√5)/2\n(3+√5)/2\n1+√2\n2\nNO\n(1+√5)/2\n0\n'],
      ['4\n1 0 -12\n1 2 -1\n3 -6 1\n2 0 -1\n', '2√3\n-1+√2\n(3+√6)/3\n√2/2\n'],
    ],
    gen(r) {
      const T = ri(r, 1, 30);
      const rows = [];
      for (let i = 0; i < T; i++) {
        let a = 0;
        while (a === 0) a = ri(r, -20, 20);
        rows.push(`${a} ${ri(r, -20, 20)} ${ri(r, -20, 20)}`);
      }
      return `${T}\n${rows.join('\n')}\n`;
    },
    rounds: 150,
    perf: () => {
      const g = rng(31);
      const T = 5000;
      const rows = [];
      for (let i = 0; i < T; i++) {
        let a = 0;
        while (a === 0) a = ri(g, -1000, 1000);
        rows.push(`${a} ${ri(g, -1000, 1000)} ${ri(g, -1000, 1000)}`);
      }
      return `${T}\n${rows.join('\n')}\n`;
    },
  },
  'set3/t4-expo': {
    samples: [
      ['3 3 2\n1 2 0\n2 3 1\n1 3 5\n', '3\n'],
      ['2 1 3\n1 2 1\n', '4\n'],
      ['4 2 2\n1 2 0\n2 3 0\n', '-1\n'],
    ],
    gen(r) {
      const n = ri(r, 2, 6), m = ri(r, 1, 10), k = ri(r, 2, 4);
      const rows = [];
      for (let i = 0; i < m; i++) {
        let u = ri(r, 1, n), v = ri(r, 1, n);
        if (u === v) v = (u % n) + 1;
        rows.push(`${u} ${v} ${ri(r, 0, 20)}`);
      }
      return `${n} ${m} ${k}\n${rows.join('\n')}\n`;
    },
    rounds: 500,
    perf: () => {
      const g = rng(37);
      const n = 10000, m = 20000, k = 100;
      const rows = [];
      for (let i = 0; i < m; i++) {
        const u = ri(g, 1, n);
        let v = ri(g, 1, n);
        if (u === v) v = (u % n) + 1;
        rows.push(`${u} ${v} ${ri(g, 0, 1000000)}`);
      }
      return `${n} ${m} ${k}\n${rows.join('\n')}\n`;
    },
  },

  'set4/t1-power': {
    samples: [
      ['10 9\n', '1000000000\n'],
      ['10 10\n', 'over\n'],
      ['2 29\n', '536870912\n'],
      ['1 1000000000\n', '1\n'],
    ],
    gen(r) {
      return `${ri(r, 1, 40)} ${ri(r, 0, 45)}\n`;
    },
    rounds: 400,
    perf: () => `2 1000000000\n`,
  },
  'set4/t2-decode': {
    samples: [
      ['8\n', '4\n'],
      ['100\n', '86\n'],
      ['1\n', '-1\n'],
    ],
    gen(r) {
      return `${ri(r, 1, 3000)}\n`;
    },
    rounds: 500,
    perf: () => `1000000000\n`,
  },
  'set4/t3-expr': {
    samples: [
      ['1+2*3\n', '7\n0\n'],
      ['(1+2)*3\n', '9\n1\n'],
      ['((1+2)*(3+4))*2\n', '42\n2\n'],
      ['1+(2+3*(4+(5+6)))*7\n', '330\n3\n'],
    ],
    gen(r) {
      const build = (dep) => {
        if (dep <= 0 || r(4) === 0) return String(ri(r, 0, 1000000000));
        const kind = r(3);
        if (kind === 2) return `(${build(dep - 1)})`;
        return `${build(dep - 1)}${kind === 0 ? '+' : '*'}${build(dep - 1)}`;
      };
      return build(ri(r, 1, 5)) + '\n';
    },
    rounds: 500,
    perf: () => '('.repeat(20000) + '1+2*3' + ')'.repeat(20000) + '\n',
  },
  'set4/t4-trend': {
    samples: [
      ['6 2\n3 1 2 1 3 4\n', '4 1\n'],
      ['5 5\n1 1 2 2 3\n', '3 4\n'],
      ['5 1\n1 5 2 3 4\n', '3 1\n'],
    ],
    gen(r) {
      const n = ri(r, 1, 14), d = ri(r, 1, n);
      const p = Array.from({ length: n }, () => ri(r, 1, 12));
      return `${n} ${d}\n${p.join(' ')}\n`;
    },
    rounds: 500,
    perf: () => {
      const g = rng(41);
      const n = 5000, d = 5000;
      const p = Array.from({ length: n }, () => ri(g, 1, 1000000000));
      return `${n} ${d}\n${p.join(' ')}\n`;
    },
  },
};

function exe(dir, base) { return path.join(ROOT, dir, base + (process.platform === 'win32' ? '.exe' : '')); }

function compile(dir, src) {
  const out = exe(dir, src);
  execFileSync(GXX, [...FLAGS, path.join(ROOT, dir, src + '.cpp'), '-o', out], { stdio: 'pipe' });
  return out;
}
function run(bin, input, ms) {
  // MinGW 在 Windows 下把 stdout 当文本流，\n 会变成 \r\n，统一换行符再比
  return execFileSync(bin, [], { input, encoding: 'utf8', maxBuffer: 1 << 28, timeout: ms || 20000 })
    .replace(/\r\n/g, '\n');
}

const results = [];
function record(name, ok, detail) {
  results.push({ name, ok, detail });
  console.log(`${ok ? 'PASS' : 'FAIL'}  ${name}${detail ? '  ' + detail : ''}`);
}

function checkDir(key, spec) {
  const dir = key;
  if (!fs.existsSync(path.join(ROOT, dir, 'solution.cpp'))) return false;
  compile(dir, 'solution');
  compile(dir, 'brute');
  // 样例
  for (let i = 0; i < spec.samples.length; i++) {
    const [inp, want] = spec.samples[i];
    const got = run(exe(dir, 'solution'), inp);
    record(`${dir} 样例${i + 1}`, got === want, got.trim() === want.trim() ? '' : `期望 ${want.trim()} 实得 ${got.trim()}`);
    const got2 = run(exe(dir, 'brute'), inp);
    record(`${dir} 样例${i + 1}(暴力)`, got2 === want, got2.trim() === want.trim() ? '' : `期望 ${want.trim()} 实得 ${got2.trim()}`);
  }
  // 对拍
  const R = Number(arg('rounds', spec.rounds || 300));
  let bad = 0, seedUsed = 0;
  for (let t = 1; t <= R; t++) {
    const r = rng(t * 7919 + 13);
    const inp = spec.gen(r);
    let a, b;
    try { a = run(exe(dir, 'solution'), inp); } catch (e) { a = '<runtime-error>'; }
    try { b = run(exe(dir, 'brute'), inp); } catch (e) { b = '<runtime-error>'; }
    if (a !== b) {
      bad++;
      if (!seedUsed) {
        seedUsed = t;
        fs.writeFileSync(path.join(ROOT, dir, 'fail.in'), inp, 'utf8');
        fs.writeFileSync(path.join(ROOT, dir, 'fail.out'), `solution:\n${a}\nbrute:\n${b}\n`, 'utf8');
      }
    }
  }
  record(`${dir} 对拍 ${R} 组`, bad === 0, bad ? `${bad} 组不一致，第 ${seedUsed} 组已存 fail.in` : '');
  // 极限计时
  if (has('perf') && spec.perf) {
    const inp = spec.perf();
    const t0 = Date.now();
    const out = run(exe(dir, 'solution'), inp, 60000);
    const ms = Date.now() - t0;
    record(`${dir} 极限数据`, ms < 5000, `输入 ${(inp.length / 1048576).toFixed(2)} MB，用时 ${ms} ms，输出 ${out.trim().slice(0, 24)}`);
  }
  return true;
}

const which = process.argv[2];
if (!which) { console.error('用法：node verify.js <set1|set2|set3|set4|all> [--rounds N] [--perf]'); process.exit(1); }
const keys = Object.keys(SPEC).filter((k) => which === 'all' || k.startsWith(which + '/'));
if (!keys.length) { console.error('没有匹配的题：' + which); process.exit(1); }
for (const k of keys) checkDir(k, SPEC[k]);

const fails = results.filter((x) => !x.ok);
const stamp = new Date().toISOString().slice(0, 19).replace('T', ' ');
const roundsNote = process.argv.indexOf('--rounds') > 0 ? `（对拍组数被 \`--rounds\` 统一覆盖为 ${arg('rounds', '?')} 组）` : '（对拍组数取每题 SPEC 里的 rounds）';
const md = ['# 模拟卷验证结果', '', `生成时间：${stamp}`, `命令：\`node notes/mock-cspj-round2/verify.js ${which}${has('perf') ? ' --perf' : ''}\`${roundsNote}`, '',
'| 项目 | 结果 | 说明 |', '| --- | --- | --- |',
...results.map((x) => `| ${x.name} | ${x.ok ? 'PASS' : '**FAIL**'} | ${x.detail.replace(/\|/g, '\\|') || '—'} |`),
'', `合计 ${results.length} 项，失败 ${fails.length} 项。`, ''].join('\n');
const vd = path.join(ROOT, 'verify');
fs.mkdirSync(vd, { recursive: true });
fs.writeFileSync(path.join(vd, 'RESULTS.md'), md, 'utf8');
console.log(`\n共 ${results.length} 项检查，失败 ${fails.length} 项 → verify/RESULTS.md`);
process.exit(fails.length ? 1 : 0);
