// 把 c-split-avg.html 里的 JS 模拟器抽出来在 Node 里跑，逐组和 fill2.exe（本机 -static 编译）对表。
// 页面里的柱子、递归树、答案全部来自同一套 simulate()，所以这一步对上就说明演示没有跑偏。
//   node notes/ccf-zhenti-2026-j1/verify/splitcheck.js
const fs = require('fs');
const path = require('path');
const { spawnSync } = require('child_process');

const src = fs.readFileSync(path.join(__dirname, '..', 'c-split-avg.html'), 'utf8');
const js = [...src.matchAll(/<script>([\s\S]*?)<\/script>/g)].map((x) => x[1]).join('\n');
const lines = js.split(/\r?\n/);
// 页面脚本尾部是顶层 DOM 绑定（^document. 开头），截掉之后剩下的都是纯计算函数
const cut = lines.findIndex((l) => /^document\.|^buildCode\(\);|^loadFrom\(/.test(l));
if (cut < 0) throw new Error('没找到页面脚本里顶层 DOM 绑定的起点，页面结构改过了？');
const core = lines.slice(0, cut).join('\n');

const stub = `
const document = { getElementById: mk, createElement: mk, querySelectorAll: () => [], addEventListener: () => {}, createTextNode: () => mk(), body: mk() };
function mk(){ return { style:{}, classList:{add(){},remove(){},toggle(){}}, appendChild(){}, textContent:'', innerHTML:'', value:'', dataset:{}, addEventListener(){}, onclick:null, querySelectorAll:()=>[], firstChild:null, lastChild:null }; }
const window = { addEventListener: () => {} };
`;

function pageAns(nn, str) {
  return new Function(stub + core + `
n = ${nn}; str = ${JSON.stringify(str)};
rebuild(true);
const fin = frames.filter(f => f.kind === 'final')[0];
let best = null;
for (const f of frames) if (f.kind === 'leaf' && f.newAns === fin.ans) best = f.path;
return { ans: fin.ans, blown: fin.blown, frames: frames.length,
  best: best ? best.map(s => '[' + s.l + ',' + s.r + ']=' + s.avg).join(' ') : '（无叶帧）' };
`)();
}

const exeAns = (nn, str) =>
  spawnSync(path.join(__dirname, 'fill2.exe'), { input: `${nn}\n${str}\n`, encoding: 'utf8' }).stdout.trim();

const cases = [[4, '016A'], [5, '11111'], [5, '0A0A0'], [2, '1F'], [3, '0F0'], [6, '13579B'], [4, '9999']];
let bad = 0;
for (const [nn, s] of cases) {
  const p = pageAns(nn, s);
  const e = exeAns(nn, s);
  const same = !p.blown && p.ans.toFixed(6) === e;
  if (!same) bad++;
  console.log(`${same ? 'OK ' : '!! '} ${nn}/${s}  页面 ${p.ans.toFixed(6)}（${p.frames} 帧）  fill2.exe ${e}  最优分法 ${p.best}`);
}
console.log(bad ? `${bad} 组不一致` : `${cases.length} 组全部与 fill2.exe 实测输出一致`);
process.exitCode = bad ? 1 : 0;
