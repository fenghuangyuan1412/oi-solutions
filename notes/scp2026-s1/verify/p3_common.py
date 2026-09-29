# p3_common.py -- 第 3 篇实验公共工具：图枚举、简单路径/简单环/割边暴力、r3 输出解析
import subprocess, os, itertools, random
D = os.path.dirname(os.path.abspath(__file__))
EXE_DUMP = os.path.join(D, "r3_dump.exe")
EXE_DET = os.path.join(D, "r3_det.exe")
EXE_R3 = os.path.join(D, "r3.exe")
P = 998244353

def run(exe, data):
    p = subprocess.run([exe], input=data.encode(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return p.stdout.decode(), p.stderr.decode(), p.returncode

def build_input(n, edges, w):
    """edges/w 用 0-based 顶点；程序输入是 1-based"""
    s = "%d %d\n" % (n, len(edges))
    for (u, v), wi in zip(edges, w):
        s += "%d %d %d\n" % (u + 1, v + 1, wi)
    return s

def adjacency(n, edges):
    adj = [[] for _ in range(n)]
    for k, (u, v) in enumerate(edges):
        adj[u].append((v, k)); adj[v].append((u, k))
    return adj

def all_simple_paths(n, edges, src):
    """paths[u] = 从 src 到 u 的所有简单路径（边下标 frozenset）列表"""
    adj = adjacency(n, edges)
    res = [[] for _ in range(n)]
    used_v = [False] * n
    used_e = []
    def dfs(u):
        res[u].append(frozenset(used_e))
        for v, k in adj[u]:
            if used_v[v] or k in used_e:
                continue
            used_v[v] = True; used_e.append(k)
            dfs(v)
            used_e.pop(); used_v[v] = False
    used_v[src] = True
    dfs(src)
    return res

def all_simple_cycles(n, edges):
    """枚举所有简单环（边下标 frozenset 去重）"""
    adj = adjacency(n, edges)
    out = set()
    def dfs(start, u, parent_e, visited_v, path_e):
        for v, k in adj[u]:
            if k == parent_e:
                continue
            if v == start:
                if len(path_e) >= 2:
                    out.add(frozenset(path_e + [k]))
            elif not visited_v[v]:
                visited_v[v] = True
                dfs(start, v, k, visited_v, path_e + [k])
                visited_v[v] = False
    for start in range(n):
        vis = [False] * n; vis[start] = True
        dfs(start, start, -1, vis, [])
    return list(out)

def bridges(n, edges, forbid=None):
    """返回图（可删掉边 forbid）中割边的原始边下标集合"""
    keep = [i for i in range(len(edges)) if i != forbid]
    E = [edges[i] for i in keep]
    adj = [[] for _ in range(n)]
    for idx, (u, v) in enumerate(E):
        adj[u].append((v, idx)); adj[v].append((u, idx))
    tin = [-1] * n; low = [0] * n; timer = [0]; res = []
    def dfs(u, pe):
        tin[u] = low[u] = timer[0]; timer[0] += 1
        for v, idx in adj[u]:
            if idx == pe: continue
            if tin[v] != -1:
                low[u] = min(low[u], tin[v])
            else:
                dfs(v, idx)
                low[u] = min(low[u], low[v])
                if low[v] > tin[u]:
                    res.append(keep[idx])
    for s in range(n):
        if tin[s] == -1: dfs(s, -1)
    return set(res)

def brute_options(paths_u, m, w):
    """paths_u: 源点(=1)到 u 的所有简单路径(边下标集合)。返回 A/B/C/D 四个选项的值"""
    A = B = C = Dv = 0
    for i, j in itertools.combinations(range(m), 2):
        hit = lambda p: (i in p) or (j in p)
        both = lambda p: (i in p) and (j in p)
        if any(hit(p) for p in paths_u):  A += w[i] * w[j]
        if any(both(p) for p in paths_u): B += w[i] * w[j]
        if all(hit(p) for p in paths_u):  C += w[i] * w[j]
        if all(both(p) for p in paths_u): Dv += w[i] * w[j]
    return [A % P, B % P, C % P, Dv % P]

def parse_dump(err, n, m):
    ans = [None] * (n + 1)
    edge = {}
    for ln in err.splitlines():
        t = ln.split()
        if not t: continue
        if t[0] == "ANS": ans[int(t[1])] = int(t[2])
        if t[0] == "EDGE":
            i = int(t[1])
            d = dict(x.split("=", 1) for x in t[3:] if "=" in x)
            uv = t[2].strip("()").split(",")
            edge[i] = dict(u=int(uv[0]), v=int(uv[1]), he=int(d["he"], 16),
                           bucket=int(d["bucket"]), ct=int(d["ct"]),
                           sw=int(d["sw"]), status=int(d["status"]))
    return ans[1:], edge

def connected(n, edges):
    dsu = list(range(n))
    def f(x):
        while dsu[x] != x: dsu[x] = dsu[dsu[x]]; x = dsu[x]
        return x
    for u, v in edges:
        a, b = f(u), f(v)
        if a != b: dsu[a] = b
    return len({f(x) for x in range(n)}) == 1

def graphs_of_n(n, limit=None):
    cand = list(itertools.combinations(range(n), 2))
    got = 0
    for k in range(1 << len(cand)):
        es = [cand[i] for i in range(len(cand)) if k >> i & 1]
        if es and connected(n, es):
            yield es
            got += 1
            if limit and got >= limit: return
