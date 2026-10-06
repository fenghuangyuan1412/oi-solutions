// 从洛谷题目页抓取题面，机械生成 problem.txt（题面唯一来源，禁止手抄）
// 用法: node scripts/fetch_luogu.js P11227 [输出目录]
//   不带输出目录时把 problem.txt 打到 stdout
// 说明：洛谷题目页现在可匿名访问，SSR HTML 里带完整题面 JSON。
const fs = require('fs');
const https = require('https');

function get(url, depth) {
  depth = depth || 0;
  // 浏览器 UA 会被 302 到同一个 URL（洛谷要先发 cookie），curl UA 才直接给 200
  return new Promise((resolve, reject) => {
    https.get(url, { headers: { 'User-Agent': 'curl/8.6.0', 'Accept': '*/*' } }, res => {
      if (res.statusCode >= 300 && res.statusCode < 400 && res.headers.location && depth < 4) {
        res.resume();
        resolve(get(new URL(res.headers.location, url).href, depth + 1));
        return;
      }
      if (res.statusCode !== 200) { res.resume(); return reject(new Error('HTTP ' + res.statusCode + ' @ ' + url)); }
      let body = '';
      res.setEncoding('utf8');
      res.on('data', c => body += c);
      res.on('end', () => resolve(body));
    }).on('error', reject);
  });
}

// 从 startIdx 处取一个配平的 JSON 值，跳过字符串里的括号
function sliceJson(h, startIdx) {
  const open = h[startIdx];
  const close = open === '{' ? '}' : ']';
  let depth = 0, end = -1, inStr = false, esc = false;
  for (let j = startIdx; j < h.length; j++) {
    const c = h[j];
    if (inStr) {
      if (esc) esc = false;
      else if (c === '\\') esc = true;
      else if (c === '"') inStr = false;
      continue;
    }
    if (c === '"') inStr = true;
    else if (c === open) depth++;
    else if (c === close) { depth--; if (!depth) { end = j + 1; break; } }
  }
  return end < 0 ? null : h.slice(startIdx, end);
}

function getProblem(html) {
  const k = html.indexOf('"problem":');
  if (k < 0) throw new Error('页面上没有 problem 字段（可能被跳转或需要登录）');
  let i = k + '"problem":'.length;
  while (i < html.length && html[i] !== '{') i++;
  const raw = sliceJson(html, i);
  if (!raw) throw new Error('题面 JSON 括号不配对');
  return JSON.parse(raw);
}

function unesc(s) {
  return s
    .replace(/<br\s*\/?>/gi, '\n')
    .replace(/<\/(p|div|li|tr|h\d)>/gi, '\n')
    .replace(/<li[^>]*>/gi, '- ')
    .replace(/<\/td>\s*<td[^>]*>/gi, ' | ')
    .replace(/<\/th>\s*<th[^>]*>/gi, ' | ')
    .replace(/<tr[^>]*>/g, '| ')
    // 只剥已知标签名：题面里的数学式会出现裸 `<`（如 $0 \leq d_i < 10^5$），
    // 用 /<[^>]+>/ 会把从 `<` 到下一个 `>` 之间的正文整段吃掉。
    .replace(/<\/?(?:p|div|span|br|li|ul|ol|table|tbody|thead|tfoot|tr|td|th|caption|strong|em|b|i|u|s|code|pre|kbd|a|img|center|h[1-6]|sup|sub|font|section|article|details|summary|blockquote|hr|input|label|script|style|form|button|svg|path|g|defs)\b[^>]*>/gi, '')
    .replace(/&nbsp;/g, ' ')
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&quot;/g, '"')
    .replace(/&#(\d+);/g, (m, d) => String.fromCodePoint(+d))
    .replace(/&amp;/g, '&')
    .replace(/^::+[a-z-]+\{[^\n]*\}[ \t]*$/gmi, '')
    .replace(/[ \t]+\n/g, '\n')
    .replace(/\n{3,}/g, '\n\n')
    .trim();
}

function section(title, htmlText) {
  if (!htmlText) return '';
  return '【' + title + '】\n' + unesc(htmlText) + '\n';
}

(async () => {
  const pid = process.argv[2];
  const outDir = process.argv[3];
  if (!pid) { console.error('用法: node scripts/fetch_luogu.js <P题号> [输出目录]'); process.exit(1); }
  const html = await get('https://www.luogu.com.cn/problem/' + pid);
  const p = getProblem(html);
  const c = p.content || {};
  const lim = p.limits || {};
  const lines = [];
  lines.push('洛谷 ' + (p.pid || pid) + ' ' + (c.name || ''));
  lines.push('原题链接: https://www.luogu.com.cn/problem/' + (p.pid || pid));
  lines.push('来源: ' + ((p.provider && p.provider.name) || '-') +
    '　难度: difficulty=' + p.difficulty +
    '　分值: ' + p.fullScore);
  lines.push('时限: ' + ((lim.time || [])[0] || '-') + ' ms　内存: ' + ((lim.memory || [])[0] || '-') + ' KB');
  lines.push('标签 id: ' + (p.tags || []).join(','));
  lines.push('（本文件由 scripts/fetch_luogu.js 从题面页机械生成，未人工改动）');
  lines.push('='.repeat(60));
  lines.push(section('题目描述', c.description));
  lines.push(section('输入格式', c.formatI));
  lines.push(section('输出格式', c.formatO));
  (p.samples || []).forEach((s, k) => {
    // 洛谷把样例给成 [输入, 输出] 二元数组
    const inp = Array.isArray(s) ? s[0] : s.input;
    const outp = Array.isArray(s) ? s[1] : s.output;
    lines.push('【样例 ' + (k + 1) + '】');
    lines.push('输入:\n' + String(inp).replace(/\n$/, ''));
    lines.push('输出:\n' + String(outp).replace(/\n$/, ''));
  });
  lines.push(section('数据范围与提示', c.hint));
  if (c.background) lines.push(section('背景', c.background));
  const text = lines.join('\n').replace(/\n{3,}/g, '\n\n') + '\n';
  if (outDir) {
    fs.mkdirSync(outDir, { recursive: true });
    fs.writeFileSync(outDir + '/problem.txt', text);
    console.error('已写入 ' + outDir + '/problem.txt (' + text.length + ' 字符)');
  } else {
    process.stdout.write(text);
  }
})().catch(e => { console.error('失败: ' + e.message); process.exit(1); });
