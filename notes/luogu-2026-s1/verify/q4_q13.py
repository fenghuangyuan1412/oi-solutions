import itertools, math
from collections import deque
C = math.comb
phi = (1 + 5 ** 0.5) / 2

# ---- 题4：1..5 各一个 + 两个 '+' 两个 '-' 组成后缀式，求最大值 ----
toks = [1, 2, 3, 4, 5, '+', '+', '-', '-']
best = None
bestex = None
cnt = 0
seen = set()
for perm in itertools.permutations(toks):
    if perm in seen:
        continue
    seen.add(perm)
    st = []
    ok = True
    for t in perm:
        if isinstance(t, int):
            st.append(t)
        else:
            if len(st) < 2:
                ok = False
                break
            b = st.pop(); a = st.pop()
            st.append(a + b if t == '+' else a - b)
    if ok and len(st) == 1:
        cnt += 1
        v = st[0]
        if best is None or v > best:
            best = v
            bestex = perm
print('题4 合法后缀式个数 =', cnt, ' 最大值 =', best, ' 例:', ''.join(map(str, bestex)))

# ---- 题13：10 盏灯，3 次区间翻转，可达状态数 ----
n = 10
iv = [(l, r) for l in range(n) for r in range(l, n)]
print('题13 区间数 =', len(iv))
S = set()
for a in iv:
    for b in iv:
        for c in iv:
            m = 0
            for (l, r) in (a, b, c):
                m ^= ((1 << (r - l + 1)) - 1) << l
            S.add(m)
print('题13 3 次操作后可达状态数 =', len(S))
print('题13 公式 C(11,2)+C(11,4)+C(11,6)+1 =', C(11, 2) + C(11, 4) + C(11, 6) + 1)

# ---- 题12：正十二面体点对距离和（用三维坐标建图） ----
V = []
for sx in (1, -1):
    for sy in (1, -1):
        for sz in (1, -1):
            V.append((sx, sy, sz))
for a, b in ((0, 1), (1, 0)):
    pass
for sx in (1, -1):
    for sy in (1, -1):
        V.append((0, sx / phi, sy * phi))
        V.append((sx / phi, sy * phi, 0))
        V.append((sx * phi, 0, sy / phi))
print('题12 顶点数 =', len(V))
edge = 2 / phi
adj = [[] for _ in range(len(V))]
for i in range(len(V)):
    for j in range(i + 1, len(V)):
        d = math.dist(V[i], V[j])
        if abs(d - edge) < 1e-9:
            adj[i].append(j)
            adj[j].append(i)
print('题12 边数 =', sum(len(a) for a in adj) // 2, ' 度数集合 =', sorted({len(a) for a in adj}))
tot = 0
dist = []
for s in range(len(V)):
    dd = [-1] * len(V)
    dd[s] = 0
    q = deque([s])
    while q:
        u = q.popleft()
        for v in adj[u]:
            if dd[v] < 0:
                dd[v] = dd[u] + 1
                q.append(v)
    dist.append(dd)
    tot += sum(dd)
from collections import Counter
print('题12 距某点的层大小 =', Counter(dist[0]))
print('题12 最短路长度总和 =', tot // 2)
