const fs = require('fs');
const h = fs.readFileSync('.p14358.html', 'utf8');
const k = h.indexOf('"problem":');

function grab(key) {
  const p = h.indexOf('"' + key + '":', k);
  if (p < 0) return null;
  let i = p + key.length + 3;
  if (h[i] === '"') {
    let out = '';
    i++;
    while (i < h.length) {
      const c = h[i];
      if (c === '"') break;
      if (c === '\\') {
        const n = h[i + 1];
        out += n === 'n' ? '\n' : n === 'u' ? String.fromCharCode(parseInt(h.substr(i + 2, 4), 16)) : n;
        i += n === 'u' ? 6 : 2;
      } else {
        out += c;
        i++;
      }
    }
    return out;
  }
  let d = 0, s = i;
  while (i < h.length) {
    if (h[i] === '{') d++;
    else if (h[i] === '}') { if (d === 0) break; d--; }
    i++;
  }
  return h.slice(s, i + 1);
}

for (const key of ['pid', 'title', 'difficulty', 'numOfSubmissions', 'numOfAcceptedUsers', 'background', 'description', 'inputFormat', 'outputFormat', 'hints', 'tags']) {
  const v = grab(key);
  if (v === null) continue;
  console.log('===== ' + key + ' =====');
  console.log(typeof v === 'string' ? v : JSON.stringify(v));
}
