# p3_more.py -- 补充实验：
#   (a) he[i]==0 <=> 树边 i 是割边；he[i]!=0 <=> 边 i 在某个简单环里
#   (b) status==1(ct==1 分支) 的结构刻画：popcount(he)>=2 且桶里只有它自己
#   (c) 题 32 换多种边输入顺序（改变 DFS 顺序）再验一次 D 恒成立 / A,B,C 恒不成立
import sys, os, random, itertools
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from p3_common import *

def pop(x): return bin(x).count("1")

def main():
    random.seed(2026)
    graphs = []
    for n in (3, 4, 5):
        for es in graphs_of_n(n):
            graphs.append((n, es))
    cand6 = list(itertools.combinations(range(6), 2))
    g = 0
    while g < 200:
        es = random.sample(cand6, random.randint(5, 12))
        if connected(6, es):
            graphs.append((6, es)); g += 1
    print("图数 =", len(graphs))
    tot_edges = 0
    bridge_he0_bad = 0          # 割边但 he!=0 或 非割边但 he==0
    cycle_he_bad = 0            # he!=0 但不在任何环 / 在环里但 he==0
    ct1_pop_bad = 0             # ct==1 但 popcount(he)<2
    ct1_edges = 0
    q32 = {"A": 0, "B": 0, "C": 0, "D": 0}
    q32_total = 0
    order_rounds = 3
    for n, es0 in graphs:
        cycles = all_simple_cycles(n, es0)
        brG = bridges(n, es0)
        in_cyc = set()
        for c in cycles: in_cyc |= c
        for r in range(order_rounds):
            perm = list(range(len(es0)))
            if r: random.shuffle(perm)
            es = [es0[i] for i in perm]
            w = [random.randint(1, 9) for _ in es]
            # 原边号映射：新序号 new_i 对应原边 perm[new_i]
            out, err, rc = run(EXE_DUMP, build_input(n, es, w))
            ans, edge = parse_dump(err, n, len(es))
            m = len(es)
            for i in range(1, m + 1):
                orig = perm[i - 1]
                st = edge[i]["status"]
                he = edge[i]["he"]
                tot_edges += 1
                # (a) 割边 <=> he==0（只对树边判，返边的 he 恒非 0）
                if st in (0, 1, 2):
                    if ((orig in brG) != (he == 0)):
                        bridge_he0_bad += 1
                        if bridge_he0_bad <= 3:
                            print("  !! 割边/he 不一致", n, es, w, i, orig, st, he, brG)
                # 在任意简单环 <=> he != 0（树边与返边一起看）
                if ((orig in in_cyc) != (he != 0)):
                    cycle_he_bad += 1
                    if cycle_he_bad <= 3:
                        print("  !! 环/he 不一致", n, es, w, i, orig, st, he, sorted(in_cyc))
                # (b) ct==1 的结构
                if st == 1:
                    ct1_edges += 1
                    if pop(he) < 2:
                        ct1_pop_bad += 1
                        if ct1_pop_bad <= 3:
                            print("  !! ct==1 但 popcount(he)<2:", n, es, w, i, hex(he))
                    # 新割边检查（题 32）
                    q32_total += 1
                    a = orig in brG
                    brG_e = bridges(n, es0, forbid=orig)
                    newbr = brG_e - brG
                    b = len(newbr) > 0
                    c = orig not in in_cyc
                    d = len(newbr) == 0
                    for name, val in zip("ABCD", [a, b, c, d]):
                        if not val: q32[name] += 1
    print("统计的边数 =", tot_edges, " ct==1 分支的树边数 =", ct1_edges, " 题32 判定次数 =", q32_total)
    print("(a) 割边 <=> he==0 不成立的次数 =", bridge_he0_bad)
    print("(a) 在某个简单环 <=> he!=0 不成立的次数 =", cycle_he_bad)
    print("(b) ct==1 但 popcount(he)<2 的次数 =", ct1_pop_bad)
    print("(c) 题 32：ct==1 分支的边中，选项 A 成立 %d 次 / B 成立 %d 次 / C 成立 %d 次 / D 成立 %d 次（共 %d 次）"
          % (q32_total - q32["A"], q32_total - q32["B"], q32_total - q32["C"], q32_total - q32["D"], q32_total))

if __name__ == "__main__":
    main()
