# fill1_cnt.py -- 计数版：证明 36 选 B / 38 选 C 的理由是复杂度
import subprocess, os, sys, re
D = os.path.dirname(os.path.abspath(__file__))
tpl = open(os.path.join(D, "fill1_cnt_tpl.cpp"), encoding="utf-8").read()
OPTS = {
 "B1": {"A": "a = 1ull * a * a % mod;", "B": "a = 1ull * a * b % mod;",
        "C": "res = 1ull * a * a % mod;", "D": "a = 1ull * res * res % mod;"},
 "B2": {"A": "bsgs2[i][j] = 1ull * bsgs2[i][j - 1] * qpow(i, B) % mod;",
        "B": "bsgs2[i][j] = 1ull * bsgs1[i][j - 1] * bsgs2[i][B - 1] % mod;",
        "C": "bsgs2[i][j] = 1ull * bsgs1[i][j - 1] * qpow(f[i], B) % mod;",
        "D": "bsgs2[i][j] = 1ull * bsgs2[i][j - 1] * bsgs1[i][B] % mod;"},
 "B3": {"A": "for(int ex = 1; ex <= now; ex++)", "B": "for(int ex = gap + 1; ex <= now; ex++)",
        "C": "for(int ex = 1; ex <= now; ex += B)", "D": "for(int ex = gap + 1; ex <= now; ex += B)"},
 "B4": {"A": "f[pr[j] * i] = 1ull * bsgs2[pr[j] * i][i % B] * bsgs1[pr[j] * i][i / B] % mod * cur % mod;",
        "B": "f[pr[j] * i] = 1ull * bsgs1[pr[j]][i % B] * bsgs2[pr[j]][i / B] % mod * cur % mod;",
        "C": "f[pr[j] * i] = 1ull * bsgs2[pr[j]][i % B] * bsgs1[pr[j]][i / B] % mod * cur % mod;",
        "D": "f[pr[j] * i] = 1ull * bsgs1[pr[j] * i][i % B] * bsgs2[pr[j] * i][i / B] % mod * cur % mod;"},
 "B5": {"A": "pr[j] > i", "B": "!(pr[j] % i)", "C": "!(i % pr[j])", "D": "i > pr[j]"},
}
ANS = {"B1": "A", "B2": "D", "B3": "B", "B4": "B", "B5": "C"}


def build(tag):
    """tag 形如 'B3A'：只在该空代入指定选项，其余空代入官方答案。"""
    blank, opt = tag[:-1], tag[-1]
    s = tpl
    for k in ("B1", "B2", "B3", "B4", "B5"):
        s = s.replace("@@%s@@" % k, OPTS[k][opt if k == blank else ANS[k]])
    p = os.path.join(D, "fill1_cnt_%s.cpp" % tag)
    open(p, "w", encoding="utf-8").write(s)
    e = os.path.join(D, "fill1_cnt_%s.exe" % tag)
    r = subprocess.run(["g++", "-static", "-O2", "-std=c++14", p, "-o", e], capture_output=True, text=True)
    return (e, r.stderr) if r.returncode == 0 else (None, r.stderr)


def run(exe, n):
    r = subprocess.run([exe], input=("%d\n" % n).encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    txt = r.stderr.decode("utf-8", errors="replace")
    d = {}
    for key, pat in [("inner", r"inner=(\d+)"), ("pow", r"powers=(\d+)"),
                     ("qpow", r"qpow=(\d+)")]:
        m = re.search(pat, txt)
        d[key] = int(m.group(1)) if m else None
    d["B"] = int(re.search(r"B=(\d+)", txt).group(1)) if "B=" in txt else None
    return d


# python 复现：同一个内层循环到底跑多少次
def py_inner(n):
    pr, vis, cnt = [], [0] * (n + 2), 0
    for i in range(2, n + 1):
        if not vis[i]:
            pr.append(i)
        for p in pr:
            if i * p > n:
                break
            cnt += 1
            vis[i * p] = 1
            if i % p == 0:
                break
    return cnt


if __name__ == "__main__":
    # 注意：B5A/B5B 的 break 条件会让内层次数变成 Θ(n^2/ln n)，只在小 n 上跑
    tags_all = ["B3A", "B3B", "B5C", "B4A", "B4B", "B4C", "B4D", "B2A", "B2B", "B2C", "B2D"]
    tags_small = ["B5A", "B5B", "B5D"]
    maxn = {"B5A": 10000, "B5B": 10000, "B5D": 10000000}
    cache = {}
    ns = [10, 100, 1000, 10000, 100000, 1000000, 10000000]
    print("== python 复现（与 C++ 同结构的欧拉筛内层次数） ==")
    for n in ns:
        print("  n=%-9d inner=%d" % (n, py_inner(n)))
    print()
    print("== C++ 计数版实测 ==")
    print("%-10s %-6s %12s %12s %10s" % ("n", "变体", "inner", "powers赋值", "qpow调用"))
    for n in ns:
        for t in tags_all + tags_small:
            lim = maxn.get(t, 10000000)
            if n > lim:
                continue
            if t not in cache:
                cache[t] = build(t)
            exe, err = cache[t]
            if not exe:
                print("  build fail", t, err[:100])
                continue
            d = run(exe, n)
            print("%-10d %-6s %12s %12s %10s" % (n, t, d["inner"], d["pow"], d["qpow"]))
        print()
