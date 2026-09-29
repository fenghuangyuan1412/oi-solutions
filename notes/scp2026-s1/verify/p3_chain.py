# p3_chain.py -- 题 30：m=n-1 链、u_i=i, v_i=i+1, w_i=1 时，输出总和 =? n(n-1)(n-2)/3
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from p3_common import *

P = 998244353
def chain(n):
    es = [(i, i + 1) for i in range(n - 1)]        # 0-based: (0,1),(1,2)...
    return es, [1] * (n - 1)

print("%4s %12s %12s %10s %10s %8s" % ("n", "sum(prog)", "formula", "equal", "run", "ans[n]"))
bad = 0
for n in range(3, 51):
    es, w = chain(n)
    data = build_input(n, es, w)
    o1, e1, rc1 = run(EXE_R3, data)          # 原版（随机种子）
    o2, e2, rc2 = run(EXE_DET, data)         # 固定种子版
    vals = [int(x) for x in o1.split()]
    vals2 = [int(x) for x in o2.split()]
    assert vals == vals2, (n, o1, o2)
    s = sum(vals) % P
    f = n * (n - 1) * (n - 2) // 3
    eq = (s == f % P)
    if not eq: bad += 1
    # 闭式：ans[u] = (u-1)(n-1) - (u-1)u/2
    closed = [((u) * (n - 1) - u * (u + 1) // 2) for u in range(n)]
    assert vals == closed, (n, vals, closed)
    print("%4d %12d %12d %10s %10s %8d" % (n, s, f, "YES" if eq else "NO", "same", vals[-1]))
print("不相等的 n 个数 =", bad)
print()
print("n=8 的逐点输出（对照闭式 (u-1)(n-1) - (u-1)u/2）：")
es, w = chain(8)
o, _, _ = run(EXE_DET, build_input(8, es, w))
print("prog:", o.strip())
print("form:", [u * 7 - u * (u + 1) // 2 for u in range(8)])
