// 把卷面 problem.txt（从 PDF 抄下来的机器可读原文）转成好读的 paper.md。
//
//   node scripts/paper_from_text.js notes/zhenti-2026-j1 --num-at start --pdf "SCP2026 J1 全卷（附答案）.pdf" --paper SCP-J1 --level 入门级
//   node scripts/paper_from_text.js notes/zhenti-2026-s1 --num-at end   --pdf "SCP2026 S1 全卷（附答案）.pdf" --paper SCP-S1 --level 提高级
//
// --num-at 说明两份卷子的行号写法不同：J 卷写在行首（`01  #include ...`），
// S 卷写在行尾（`#include <bits/stdc++.h> 1`）。
// 这个脚本只做排版，不改一个字的题目文字；重跑一次就会覆盖 paper.md。

const fs = require('fs');
const path = require('path');

function arg(name, dflt) {
  const i = process.argv.indexOf('--' + name);
  return i > 0 && process.argv[i + 1] && !process.argv[i + 1].startsWith('--') ? process.argv[i + 1] : dflt;
}

function convert(src, numAt) {
  const raw = fs.readFileSync(src, 'utf8').split(/\r?\n/);
  const out = [];
  let code = [];

  const isCode = (l) => /^\d{2}(\s\s|\s*$)/.test(l);
  const codeEndNum = (l) => Number(/(\d{1,2})\s*$/.exec(l)[1]);
  const codeEndBody = (l) => l.replace(/\s*\d{1,2}\s*$/, '');
  const flushCode = () => {
    if (!code.length) return;
    out.push('```text', ...code.map((c) => c.replace(/\s+$/, '')), '```', '');
    code = [];
  };

  const skipRe = new RegExp(
    '^(' +
      '<<<PAGE[^>]*>>>' +
      '|LUOGU SCP-[JS][^\\n]*' +
      '|第\\s*\\d+\\s*页，共\\s*\\d+\\s*页' +
      '|（SCP-[JS]1）[^\\n]*' +
      '|\\d{4} LUOGU[^\\n]*' +
      '|认证时间[^\\n]*' +
      ')$'
  );

  let lastNum = 0, section = '', inAnswers = false, inAd = false;
  for (let i = 0; i < raw.length; i++) {
    const l = raw[i].replace(/\u00a0/g, ' ').replace(/\s+$/g, '');

    // 卷尾的洛谷课程广告整段不属于试题；参考答案在它后面，遇到就恢复正常处理
    if (inAd) {
      if (/参考答案\s*$/.test(l.trim())) inAd = false;
      else continue;
    }
    if (/^广告\s/.test(l.trim())) { flushCode(); inAd = true; continue; }

    if (numAt === 'end') {
      if (code.length && (skipRe.test(l.trim()) || !l.trim())) continue; // 程序被分页切断：跳过页眉继续
      const startsProg = /\s1\s*$/.test(l) && /^#include/.test(codeEndBody(l));
      const cont = code.length && /\d{1,2}$/.test(l) && codeEndNum(l) === lastNum + 1 &&
        (/^[\x00-\x7F①-⑤\s]*$/.test(codeEndBody(l)) || /\/\//.test(codeEndBody(l)));
      if (startsProg || cont) {
        lastNum = codeEndNum(l);
        code.push(String(lastNum).padStart(2, '0') + '  ' + codeEndBody(l));
        continue;
      }
      if (code.length) { flushCode(); lastNum = 0; }
    } else {
      const numStart = (s) => Number(/^(\d{2})/.exec(s)[1]);
      if (isCode(l)) { lastNum = numStart(l); code.push(l); continue; }
      const junk = (s) => !s.trim() || skipRe.test(s.trim());
      // 往后看：下一段实质行（跳过空行/页眉）是不是接着编号的代码行
      const nextCode = (from) => {
        let j = from;
        while (j < raw.length && junk(raw[j].replace(/\u00a0/g, ' ').replace(/\s+$/g, ''))) j++;
        if (j >= raw.length) return null;
        const s = raw[j].replace(/\u00a0/g, ' ').replace(/\s+$/g, '');
        return isCode(s) ? numStart(s) : null;
      };
      if (code.length && junk(l)) {
        // 卷面里真正的空行也带行号（如 "48"）；不带行号的空行只是 PDF 分页留下的排版噪声
        const nx = nextCode(i + 1);
        if (nx !== null && nx === lastNum + 1) continue;
      }
      if (code.length && /^[ -~]+$/.test(l) && nextCode(i + 1) === lastNum + 1) {
        // PDF 把一条带行号的语句折成了两行：后一行没有行号，并进同一个代码块并与语句起点对齐
        const m = /^(\d{2})(\s*)/.exec(code[code.length - 1]);
        code.push(' '.repeat(2 + (m ? m[2].length : 2)) + l.trim()); continue;
      }
      if (code.length && skipRe.test(l.trim())) continue;
      flushCode(); lastNum = 0;
    }

    if (!/参考答案\s*$/.test(l.trim()) && skipRe.test(l.trim())) continue;
    if (!l.trim()) { if (out.length && out[out.length - 1] !== '') out.push(''); continue; }

    if (/^[一二三]、/.test(l)) {
      section = l[0];
      out.push('', inAnswers ? '**' + l + '**' : '## ' + l, '');
      continue;
    }
    if (/^（\d+）\s*$/.test(l.trim()) || /^（\d+）（/.test(l.trim())) {
      const n = /\d+/.exec(l.trim())[0];
      const name = section === '三' ? '完善程序' : '阅读程序';
      const rest = l.trim().replace(/^（\d+）\s*/, '');
      out.push('', '### ' + name + '（' + n + '）', '');
      if (rest) out.push(rest);
      continue;
    }
    if (/^·\s*(判断题|单选题)/.test(l.trim())) { out.push('', '**' + l.trim().slice(1) + '**', ''); continue; }
    if (/参考答案\s*$/.test(l.trim())) { inAnswers = true; out.push('', '---', '', '## ' + l.trim(), ''); continue; }
    if (/^(考生注意事项|一、单项选择题|二、阅读程序|三、完善程序)/.test(l.trim()) && !/^[一二三]、/.test(l.trim())) {
      out.push('', '### ' + l.trim(), ''); continue;
    }

    const q = /^(\d{1,2})\.\s*(.*)$/.exec(l.trim());
    if (q && Number(q[1]) >= 1 && Number(q[1]) <= 43) {
      out.push('', '**' + Number(q[1]) + '.** ' + q[2]);
      continue;
    }
    if (/^([A-D])\.\s/.test(l.trim())) {
      for (const p of l.trim().split(/\s+(?=[A-D]\.\s)/)) out.push('- ' + p);
      continue;
    }
    out.push(l.trim());
  }
  flushCode();
  return out.join('\n').replace(/\n{3,}/g, '\n\n') + '\n';
}

const dir = process.argv[2];
if (!dir) { console.error('用法：node scripts/paper_from_text.js <notes/某套卷> --num-at start|end --pdf <PDF 名> --paper <SCP-J1> --level <入门级|提高级>'); process.exit(1); }
const numAt = arg('num-at', 'start');
const pdf = arg('pdf', 'PDF');
const paper = arg('paper', path.basename(dir).toUpperCase());
const body = convert(path.join(dir, 'problem.txt'), numAt);
const head =
  '# ' + paper + ' 2026 第一轮 · 全卷题面原文\n\n' +
  '> 逐字抄自卷面 PDF `' + pdf + '`。**题目文字一个字都没改**，' +
  '只做了三件排版上的事：去掉每页重复的页眉页脚和卷尾的课程广告、把程序包成代码块、' +
  (numAt === 'end'
    ? '把卷面写在行尾的行号统一挪到行首。'
    : '把卷面折行的语句并回原行（卷面排版太宽，PDF 会把半行甩到下一行）。') + '\n' +
  '> 卷面里的下标在纯文本里会变平，例如 `(1C)16` 就是 $(1C)_{16}$、`(31)8` 就是 $(31)_8$。\n' +
  '> 代码块里的**行号就是题目里说的"第几行"**。\n' +
  '> 边看题边看讲解：[`README.md`](README.md)；机器实测证据：[`verify/RESULTS.md`](verify/RESULTS.md)。\n' +
  '> 卷面参考答案在本页最下方。\n\n---\n\n';
fs.writeFileSync(path.join(dir, 'paper.md'), head + body, 'utf8');
console.log(dir + '/paper.md  ' + (head + body).split('\n').length + ' 行, 代码块=' + ((head + body).match(/^```text/gm) || []).length);
