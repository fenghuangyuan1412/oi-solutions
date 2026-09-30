# p3_struct.py -- 题 29（he 相等 => 同一个环？）与题 32（ct[j]==1 分支时树边 e 的性质）
# 用 r3_dump.exe（位模式哈希：he 的置位集合 = 覆盖该边的返边集合，绝无冲突）
import sys, os, random, itertools
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from p3_common import *

def analyze(n, es, w, cycles, brG):
    out, err, rc = run(EXE_DUMP, build_input(n, es, w))
    ans, edge = parse_dump(err, n, len(es))
    res = []
    m = len(es)
    # --- 题 29 ---
    for i, j in itertools.combinations(range(1, m + 1), 2):
        hi, hj = edge[i]["he"], edge[j]["he"]
        if hi == hj and hi != 0:
            same = any((i - 1 in c and j - 1 in c) for c in cycles)
            res.append(("Q29", i, j, hi, same))
    # --- 题 32 ---
    for i in range(1, m + 1):
        st = edge[i]["status"]
        if st == 1:                     # 走了 ct[j]==1 这条分支
            a = (i - 1) in brG                                   # A: e 本身是割边
            brG_e = bridges(n, es, forbid=i - 1)
            newbr = brG_e - brG - set()
            b = len(newbr) > 0                                   # B: 删 e 后产生新割边
            c = not any((i - 1) in cyc for cyc in cycles)        # C: e 不在任何简单环
            d = len(newbr) == 0                                  # D: 删 e 后不产生新割边
            res.append(("Q32", i, n, es, w, edge[i], a, b, c, d, sorted(newbr)))
    return res, ans

def main():
    random.seed(99)
    graphs = []
    for n in (3, 4, 5):
        for es in graphs_of_n(n):
            graphs.append((n, es))
    cand6 = list(itertools.combinations(range(6), 2))
    g = 0
    while g < 250:
        es = random.sample(cand6, random.randint(5, 12))
        if connected(6, es):
            graphs.append((6, es)); g += 1
    print("图数 =", len(graphs))
    q29_pairs = q29_bad = 0
    q29_examples = []
    q32_ct1_edges = 0
    q32_fail = {"A": 0, "B": 0, "C": 0, "D": 0}
    q32_examples = {k: [] for k in "ABCD"}
    q32_ct2_edges = 0
    q32_ct2_failD = 0
    bridge_but_nonzero = 0
    nonzero_but_bridge = 0
    for n, es in graphs:
        w = [random.randint(1, 9) for _ in es]
        cycles = all_simple_cycles(n, es)
        brG = bridges(n, es)
        res, ans = analyze(n, es, w, cycles, brG)
        for r in res:
            if r[0] == "Q29":
                q29_pairs += 1
                if not r[4]:
                    q29_bad += 1
                    if len(q29_examples) < 3: q32 = q29_examples.append((n, es, w, r))
            else:
                _, i, _, _, _, ed, a, b, c, d, newbr = r
                q32_ct1_edges += 1
                for name, val in zip("ABCD", [a, b, c, d]):
                    if not val:
                        q32_fail[name] += 1
                        if len(q32_examples[name]) < 2:
                            q32_examples[name].append((n, es, w, i, ed, newbr))
    print()
    print("=== 题 29：he[i]==he[j]!=0 的边对 ===")
    print("满足条件的边对总数 = %d，其中『不在同一个简单环』的反例 = %d" % (q29_pairs, q29_bad))
    print("结论：he[i]==he[j]!=0 => 边 i,j 同在一个简单环里  ==>  %s" % ("成立(T)" if q29_bad == 0 else "不成立(F)"))
    if q29_examples:
        print("反例：", q29_examples[:2])
    print()
    print("=== 题 32：走 ct[j]==1 分支的树边 e ===")
    print("这类边总数 = %d" % q32_ct1_edges)
    for name in "ABCD":
        print("  选项 %s 不成立的次数 = %d  (%s)" % (name, q32_fail[name],
              "恒成立" if q32_fail[name] == 0 else "有反例"))
        for ex in q32_examples[name][:1]:
            n, es, w, i, ed, newbr = ex
            print("     反例: n=%d edges=%s w=%s 边%d %s 新增割边=%s" % (n, es, w, i, ed, newbr))
    return 0

if __name__ == "__main__":
    main()
