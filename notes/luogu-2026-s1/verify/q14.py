from math import comb
import bisect
n, K = 2026, 10000          # 题14：长度 n 的排列，恰好 K = 10^4 次相邻交换

def min_inv(m):
    """LIS 长度 <= m 的排列最少逆序对数：切成 m 段，每段内部递减，段长尽量平均"""
    q, r = divmod(n, m)
    return r * comb(q + 1, 2) + (m - r) * comb(q, 2)

for m in range(182, 193):
    print('m=%3d  minInv=%6d  %s' % (m, min_inv(m), '<=10000 可行' if min_inv(m) <= K else '超出'))

# 构造：25 段长 12 + 106 段长 11 + 56 段长 10，每段用连续数值、段内递减、段间递增
lens = [12] * 25 + [11] * 106 + [10] * 56
assert sum(lens) == n and len(lens) == 187
p, cur = [], 1
for L in lens:
    p.extend(range(cur + L - 1, cur - 1, -1))
    cur += L
assert sorted(p) == list(range(1, n + 1))

bit = [0] * (n + 1)
def lowbit(x): return x & -x
def qry(x):
    s = 0
    while x > 0:
        s += bit[x]; x -= lowbit(x)
    return s
def add(x):
    while x <= n:
        bit[x] += 1; x += lowbit(x)
total = 0
for i, x in enumerate(p):
    total += i - qry(x)      # 前面比 x 小的个数 = qry(x)，故前面比 x 大的 = i - qry(x)
    add(x)
tail = []
for x in p:
    k = bisect.bisect_left(tail, x)
    if k == len(tail):
        tail.append(x)
    else:
        tail[k] = x
print('构造排列：段数=%d 逆序对=%d(公式 %d) LIS=%d' % (len(lens), total,
      sum(comb(L, 2) for L in lens), len(tail)))
print('结论：LIS 最小 = 187；186 需要至少 %d 次交换 > 10^4' % min_inv(186))
