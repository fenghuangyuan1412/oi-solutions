# fill2_run.py -- 静态 Top Tree 完善程序：官方答案验证 + 每个空四个选项逐个实测
# 暴力口径：询问子树 = 主链顶点 x..y + 它们挂的所有叶子；对它求带权直径（两次 DFS）
import subprocess, os, sys, random, heapq

D = os.path.dirname(os.path.abspath(__file__))
tpl = open(os.path.join(D, "fill2_tpl.cpp"), encoding="utf-8").read()

OPTS = {
 "B1": {"A": "(a.c == 1 || s == 1 ? a.l : a.r) + b.l",
        "B": "(a.c == 1 ? max(a.d, b.d) : (s == 0 ? a.l : a.r)) + b.l",
        "C": "(a.c == 1 || s == 0 ? a.l : a.w) + b.l",
        "D": "(a.c == 1 || s == 0 ? a.l : a.r) + b.l"},
 "B2": {"A": "y = merge(y, star(x), 'R', 0)",
        "B": "y = merge(y, star(x), 'R', 1)",
        "C": "y = merge(star(x), y, 'R', 0)",
        "D": "y = merge(y, star(x), 'C')"},
 "B3": {"A": "merge(q, p, 'C')", "B": "merge(p, q, 'R', 0)",
        "C": "merge(p, q, 'R', 1)", "D": "merge(p, q, 'C')"},
 "B4": {"A": "bel[x] < n", "B": "x < n", "C": "bel[x] <= n", "D": "st[bel[x]] != 0"},
 "B5": {"A": "x == y ? leaf(0) : query(1, 1, n - 1, x, y - 1)",
        "B": "x == y ? star(x) : query(1, 1, n - 1, x, y)",
        "C": "x == y ? star(x) : query(1, 1, n - 1, x, y - 1)",
        "D": "x == y ? star(x) : query(1, 1, n - 1, x + 1, y - 1)"},
}
ANS = {"B1": "D", "B2": "A", "B3": "D", "B4": "A", "B5": "C"}   # 官方: 39D 40A 41D 42A 43C


def build(tag):
    blank, opt = tag[:-1], tag[-1]
    s = tpl
    for k in ("B1", "B2", "B3", "B4", "B5"):
        s = s.replace("@@%s@@" % k, OPTS[k][opt if k == blank else ANS[k]])
    p = os.path.join(D, "fill2_%s.cpp" % tag)
    open(p, "w", encoding="utf-8").write(s)
    e = os.path.join(D, "fill2_%s.exe" % tag)
    r = subprocess.run(["g++", "-static", "-O2", "-std=c++14", p, "-o", e], capture_output=True, text=True)
    return (e, r.stderr) if r.returncode == 0 else (None, r.stderr)


def gen(seed, nmax=8, kmax=7, mmax=12, wmax=20):
    rnd = random.Random(seed)
    n = rnd.randint(2, nmax)
    k = rnd.randint(0, kmax)
    a = [0] + [rnd.randint(0, wmax) for _ in range(n - 1)]
    bel = [0] * (k + 1)
    lw = [0] * (k + 1)
    # 叶边按输入顺序编号：先顶点 1 的叶边，再顶点 2 的……
    cnt = [0] * (n + 1)
    left = k
    for i in range(1, n):
        c = rnd.randint(0, left)
        cnt[i] = c
        left -= c
    cnt[n] = left
    z = 0
    for i in range(1, n + 1):
        for _ in range(cnt[i]):
            z += 1
            bel[z] = i
            lw[z] = rnd.randint(0, wmax)
    lines = ["%d %d %d" % (n, k, mmax), " ".join(str(a[i]) for i in range(1, n))]
    for i in range(1, n + 1):
        ws = [str(lw[x]) for x in range(1, k + 1) if bel[x] == i]
        # 叶边按编号升序给出，与题意"按输入顺序编号"一致
        lines.append(" ".join([str(len(ws))] + ws))
    ops = []
    rndop = random.Random(seed + 7)
    for _ in range(mmax):
        choices = [1, 3, 3]
        if k:
            choices.append(2)
        o = rndop.choice(choices)
        if o == 1:
            ops.append("1 %d %d" % (rndop.randint(1, n - 1), rndop.randint(0, wmax)))
        elif o == 2:
            ops.append("2 %d %d" % (rndop.randint(1, k), rndop.randint(0, wmax)))
        else:
            l = rndop.randint(1, n); r = rndop.randint(1, n)
            if l > r:
                l, r = r, l
            ops.append("3 %d %d" % (l, r))
    lines += ops
    inp = "\n".join(lines) + "\n"

    # ---- 暴力求直径 ----
    def solve(a, bel, lw, n, k, ops):
        out = []
        for line in ops:
            p = line.split()
            o, x, y = int(p[0]), int(p[1]), int(p[2])
            if o == 1:
                a[x] = y
            elif o == 2:
                lw[x] = y
            else:
                adj = {}
                nodes = [str(v) for v in range(x, y + 1)]
                leaves = [i for i in range(1, k + 1) if x <= bel[i] <= y]
                nodes += ["L%d" % i for i in leaves]
                for v in nodes:
                    adj[v] = []
                for i in range(x, y):
                    adj[str(i)].append((str(i + 1), a[i]))
                    adj[str(i + 1)].append((str(i), a[i]))
                for i in leaves:
                    adj[str(bel[i])].append(("L%d" % i, lw[i]))
                    adj["L%d" % i].append((str(bel[i]), lw[i]))
                def far(src, collect=False):
                    dist = {src: 0}
                    pq = [(0, 0, src)]
                    cnt = 0
                    best = (0, src)
                    while pq:
                        dd, _, u = heapq.heappop(pq)
                        if dd > dist.get(u, 1 << 62):
                            continue
                        if dd > best[0]:
                            best = (dd, u)
                        for v, w in adj[u]:
                            nd = dd + w
                            if nd < dist.get(v, 1 << 62):
                                dist[v] = nd
                                cnt += 1
                                heapq.heappush(pq, (nd, cnt, v))
                    if collect:
                        return max(dist.values())
                    return best
                # 子树很小（<=15 个点），直接所有点对取最大，避免两次扫的平局隐患
                diam = max([far(v, True) for v in nodes] + [0])
                out.append(str(diam))
        return "\n".join(out) + "\n"

    expect = solve(a, list(bel), list(lw), n, k, ops)
    return inp, expect


def run(exe, inp, to=8):
    r = subprocess.run([exe], input=inp.encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=to)
    return r.returncode, r.stdout.decode("utf-8", errors="replace"), r.stderr.decode("utf-8", errors="replace")


if __name__ == "__main__":
    mode = sys.argv[1] if len(sys.argv) > 1 else "all"
    tags = ["B1%s" % c for c in "ABCD"] + ["B2%s" % c for c in "ABCD"] + \
           ["B3%s" % c for c in "ABCD"] + ["B4%s" % c for c in "ABCD"] + ["B5%s" % c for c in "ABCD"]
    exes = {}
    for t in tags:
        e, err = build(t)
        if not e:
            print("%s: 编译失败" % t)
            print("   ", err.strip().splitlines()[-1][:160] if err.strip() else "")
        exes[t] = e
    print()
    if mode == "big":
        # 大数据冒烟测试（性能/越界）：n=k=m=1e5
        bigp = os.path.join(D, "fill2_big.txt")
        if os.path.exists(bigp):
            inp = open(bigp, encoding="utf-8").read()
            print("复用已生成的大样例 %s（%d 字节）" % (bigp, len(inp)))
        else:
            inp = None
        if inp is None:
          rnd = random.Random(2026)
          n, k, m = 100000, 100000, 100000
          # 每个主链顶点的叶边条数（保证总和 = k，且最后顶点也可能有）
          cnt = [0] * (n + 1)
          left = k
          for i in range(1, n):
              c = min(rnd.randint(0, 3), left)
              cnt[i] = c
              left -= c
          cnt[n] = left
          lines = ["%d %d %d" % (n, k, m), " ".join(str(rnd.randint(0, 10**9)) for _ in range(n - 1))]
          owner = [0] * (k + 2)
          z = 0
          for i in range(1, n + 1):
              ws = []
              for _ in range(cnt[i]):
                  z += 1
                  owner[z] = i
                  ws.append(str(rnd.randint(0, 10**9)))
              lines.append(" ".join([str(cnt[i])] + ws))
          assert z == k
          for _ in range(m):
              o = rnd.choice([1, 1, 2, 3, 3, 3])
              if o == 1:
                  lines.append("1 %d %d" % (rnd.randint(1, n - 1), rnd.randint(0, 10**9)))
              elif o == 2:
                  lines.append("2 %d %d" % (rnd.randint(1, k), rnd.randint(0, 10**9)))
              else:
                  a1 = rnd.randint(1, n); b1 = rnd.randint(1, n)
                  lines.append("3 %d %d" % (min(a1, b1), max(a1, b1)))
          inp = "\n".join(lines) + "\n"
          open(os.path.join(D, "fill2_big.txt"), "w").write(inp)
        # 官方答案版
        s_official = tpl
        for kk in ("B1", "B2", "B3", "B4", "B5"):
            s_official = s_official.replace("@@%s@@" % kk, OPTS[kk][ANS[kk]])
        p = os.path.join(D, "fill2.cpp")
        open(p, "w", encoding="utf-8").write(s_official)
        e = os.path.join(D, "fill2.exe")
        rr = subprocess.run(["g++", "-static", "-O2", "-std=c++14", p, "-o", e], capture_output=True, text=True)
        e = e if rr.returncode == 0 else None
        import time
        for t, exe in [("官方答案", e)]:
            if not exe:
                print("编译失败", rr.stderr[:200]); continue
            st = time.time()
            rc, out, errtxt = run(exe, inp)
            print("大样例 n=k=m=1e5 %s: exit=%d 输出行数=%d 用时 %.2fs" % (t, rc, len(out.splitlines()), time.time() - st))
        # 逐空错误选项在大样例上的表现（B5D 会死循环，单独用小样例验证，见 log_f2_min.txt）
        for t in ["B1A", "B1B", "B1C", "B2B", "B2C", "B2D", "B3A", "B3B", "B3C",
                  "B4B", "B4C", "B4D", "B5A", "B5B"]:
            if t not in exes or not exes[t]:
                continue
            st = time.time()
            try:
                rc, out, errtxt = run(exes[t], inp, to=30)
                print("大样例 %s: exit=%d 行数=%d %.2fs" % (t, rc, len(out.splitlines()), time.time() - st))
            except subprocess.TimeoutExpired:
                print("大样例 %s: 超时" % t)
        sys.exit(0)

    print("生成随机数据 ...")
    NCASE = 60 if mode == "quick" else 300
    cases = []
    nq_total = 0
    for seed in range(1, NCASE + 1):
        inp, expect = gen(seed)
        cases.append((seed, inp, expect.split()))
        nq_total += len(expect.split())
    print("共 %d 组数据，合计 %d 个询问\n" % (len(cases), nq_total))

    print("%-6s %-8s %-12s %s" % ("变体", "口径", "结论", "细节"))
    for t in tags:
        exe = exes[t]
        official = t[:-1] in ANS and t[-1] == ANS[t[:-1]]
        label = "官方答案" if official else "错误选项"
        if not exe:
            print("%-6s %-8s %-12s %s" % (t, label, "编译失败", ""))
            continue
        badcase = 0
        badq = 0
        big = small = 0
        crash = None
        first = ""
        timeout = 0
        for seed, inp, exp in cases:
            try:
                rc, out, errtxt = run(exe, inp)
            except subprocess.TimeoutExpired:
                timeout += 1
                badcase += 1
                if not first:
                    first = "seed=%d 死循环/爆栈（%ds 未结束）" % (seed, 8)
                break
            got = out.split()
            if rc != 0:
                crash = (seed, rc)
                badcase += 1
                if not first:
                    first = "seed=%d 运行崩溃 exit=%u(0x%08X)" % (seed, rc & 0xFFFFFFFF, rc & 0xFFFFFFFF)
                continue
            if got != exp:
                badcase += 1
                for i in range(len(exp)):
                    gv = got[i] if i < len(got) else None
                    if gv != exp[i]:
                        badq += 1
                        try:
                            if gv is None:
                                small += 1
                            elif int(gv) > int(exp[i]):
                                big += 1
                            else:
                                small += 1
                        except ValueError:
                            pass
                        if not first:
                            first = "seed=%d 第%d个询问 期望%s 得到%s" % (seed, i + 1, exp[i], gv)
        if crash:
            concl = "运行崩溃"
        elif timeout:
            concl = "死循环/爆栈"
        elif badcase == 0:
            concl = "答案正确"
        else:
            concl = "答案错误"
        print("%-6s %-8s %-12s %d/%d 组错，%d/%d 个询问错（偏大%d 偏小%d）%s | %s" % (
            t, label, concl, badcase, len(cases), badq, nq_total, big, small,
            ("  <-- %s" % ANS[t[:-1]]) if official else "", first[:70]))
