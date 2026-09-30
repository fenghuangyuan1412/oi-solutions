# p3_semantics.py -- 题 31：r3 对每个点求的到底是 A/B/C/D 中哪一个？
# 枚举 n<=5 的全部连通简单图 + n=6 随机 300 个连通图，独立暴力算四个选项，与程序输出对照
import sys, os, random, itertools
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from p3_common import *

def main():
    random.seed(20260816)
    graphs = []
    for n in (3, 4, 5):
        for es in graphs_of_n(n):
            graphs.append((n, es))
    cand6 = list(itertools.combinations(range(6), 2))
    got = 0
    while got < 300:
        es = random.sample(cand6, random.randint(5, 12))
        if connected(6, es):
            graphs.append((6, es)); got += 1
    print("图数 = %d（n=3/4/5 全部连通简单图 + n=6 随机 300）" % len(graphs))
    tot = {"A": 0, "B": 0, "C": 0, "D": 0}
    nodes = 0
    counter_examples = {"A": [], "B": [], "C": [], "D": []}
    mismatch_dump_vs_det = 0
    gi = 0
    for n, es in graphs:
        w = [random.randint(1, 9) for _ in es]
        m = len(es)
        out, err, rc = run(EXE_DUMP, build_input(n, es, w))
        ans, edge = parse_dump(err, n, m)
        if gi < 50:                              # 位模式版 vs 固定种子随机哈希版 交叉验证
            o2, _, _ = run(EXE_DET, build_input(n, es, w))
            if [int(x) for x in o2.split()] != ans:
                mismatch_dump_vs_det += 1
                print("  !! r3_dump 与 r3_det 输出不同:", n, es, w, ans, o2.strip())
        paths = all_simple_paths(n, es, 0)
        vals = [brute_options(paths[u], m, w) for u in range(n)]
        nodes += n
        for k, name in enumerate("ABCD"):
            tot[name] += sum(1 for u in range(n) if vals[u][k] == ans[u])
        for u in range(n):
            for k, name in enumerate("ABCD"):
                if vals[u][k] != ans[u] and len(counter_examples[name]) < 3:
                    counter_examples[name].append((n, es, w, u + 1, ans[u], vals[u]))
        gi += 1
    print("统计的点数 = %d" % nodes)
    print("与程序输出吻合的点数：A=%d  B=%d  C=%d  D=%d" % (tot["A"], tot["B"], tot["C"], tot["D"]))
    print("r3_dump(位模式) 与 r3_det(固定种子) 不一致次数 =", mismatch_dump_vs_det)
    for name in "ABCD":
        print("选项 %s 的反例（前 2 个）:" % name)
        for ex in counter_examples[name][:2]:
            n, es, w, u, a, vals = ex
            print("   n=%d edges=%s w=%s  点%d: 程序=%d  A=%d B=%d C=%d D=%d"
                  % (n, es, w, u, a, vals[0], vals[1], vals[2], vals[3]))
    return tot

if __name__ == "__main__":
    main()
