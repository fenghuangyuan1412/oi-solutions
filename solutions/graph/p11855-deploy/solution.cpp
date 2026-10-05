/*
 * P11855 [CSP-J 2022 山东] 部署
 * ---------------------------------------------------------------------------
 * 题意压缩：
 *   给一棵以 1 号城市为根的树，每个点有初始兵力 a[i]。进行 m 次操作，两种：
 *     1 x y : 把【x 及其整棵子树】的所有城市兵力 +y；
 *     2 x y : 把【x、x 的父亲、x 的所有儿子】兵力 +y（即 x 的"邻域"）。
 *   所有操作做完以后，回答 q 次单点询问：某城市最终兵力是多少。
 *
 * 关键观察——把"在线做操作"换成"离线数贡献"：
 *   操作之间互不影响，而且是"全部做完才问"，所以可以反过来看：
 *   一个点 u 的最终兵力 = a[u] + Σ_{操作 o} (o 是否覆盖 u) · y_o。
 *   于是问题拆成两个独立的部分：
 *
 *   (A) 操作 1(x, y) 覆盖 u  ⇔  x 是 u 的祖先（含 u 自己，因为子树含根 x）
 *        做法：对每个操作 1，在它的中心 x 上打一个标记 raw1[x] += y。
 *              点 u 被打到的总量 = 根到 u 这条路径上所有 raw1 之和
 *              —— 一次自顶向下的前缀和（BFS 序天然父亲在前）即可。
 *
 *   (B) 操作 2(x, y) 覆盖 u  ⇔  u == x  或  u == parent(x)  或  u 是 x 的儿子
 *        做法：记 A[x] = 所有"以 x 为中心"的操作 2 的 y 之和，则点 u 的增益为
 *              A[u]  ← 自己就是被操作的中心
 *            + A[parent(u)]  ← 自己是"被操作点的父亲"（父亲被 x 跳到）
 *            + Σ_{c 是 u 的儿子} A[c]  ← 自己是"被操作点的父亲"里的另一半
 *        第三项与具体的 x 无关，可以一次性"子 → 父"累加成 childSum[u]。
 *
 *   最终 ans[u] = a[u] + delta1[u] + A[u] + A[parent(u)] + childSum[u]
 *
 * 复杂度：时间 O(n + m + q)，空间 O(n)。
 *
 * 数值范围：n, m, q ≤ 10^6；a_i ≤ 10^9；1 ≤ y ≤ 10
 *   raw1[x]、A[x] ≤ 10^6 × 10 = 10^7，int 够；
 *   但 delta1 / childSum / a 都可能到 10^13 量级 ⇒ 一律 long long。
 *
 * 实现注意：
 *   · n 到 10^6，链可以深到 10^6 ⇒ 必须用【迭代 BFS】而不是递归 DFS，否则爆栈。
 *   · 用【链式前向星】而不是 vector<vector<int>>，省内存也更快。
 *   · 1e6 级别的读入输出，手写 fread / fwrite 缓冲。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 *   （本机必须带 -static，否则运行时段错误）
 */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static const int MAXN = 1000000 + 5;
static const int MAXE = 2000000 + 5;

/* ---------------- 手写快读 / 快写：1e6 级别输入，cin 与 scanf 都不够稳 ------- */
namespace FastIO {
const int IN_BUFSZ = 1 << 20;
char ibuf[IN_BUFSZ];
int ip = 0, ilen = 0;

inline char gc() {
    if (ip == ilen) {
        ilen = (int)fread(ibuf, 1, IN_BUFSZ, stdin);
        ip = 0;
        if (ilen == 0) return 0;
    }
    return ibuf[ip++];
}
inline int readInt() {
    char c = gc();
    while (c && (c < '0' || c > '9')) c = gc();
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x;
}

char obuf[1 << 22];
int op = 0;
inline void flushOut() { if (op) { fwrite(obuf, 1, op, stdout); op = 0; } }
inline void putChar(char c) { if (op == (1 << 22)) flushOut(); obuf[op++] = c; }
inline void putLL(ll x) {
    if (x == 0) { putChar('0'); return; }
    char s[24]; int t = 0;
    while (x > 0) { s[t++] = char('0' + x % 10); x /= 10; }
    while (t) putChar(s[--t]);
}
} // namespace FastIO

/* ---------------- 链式前向星 ---------------- */
int head[MAXN], nxt_[MAXE], to_[MAXE], ecnt = 0;
inline void addEdge(int u, int v) {
    to_[++ecnt] = v; nxt_[ecnt] = head[u]; head[u] = ecnt;
}

int parent_[MAXN];   // 父亲；1 号点为根，parent_[1] = 0
int order_[MAXN];    // BFS 序（父亲一定排在儿子之前）

ll  a[MAXN];         // 初始兵力
ll  delta1[MAXN];    // 根 -> u 路径上的 raw1 之和 = 操作 1 给 u 的增益
ll  childSum[MAXN];  // childSum[u] = Σ_{c 是 u 的儿子} A[c]
int raw1[MAXN];      // raw1[x]  = 所有作用在 x 上的操作 1 的 y 之和
int A[MAXN];         // A[x]     = 所有作用在 x 上的操作 2 的 y 之和

int main() {
    int n = FastIO::readInt();
    for (int i = 1; i <= n; ++i) a[i] = FastIO::readInt();

    for (int i = 1; i < n; ++i) {          // n - 1 条无向边
        int u = FastIO::readInt(), v = FastIO::readInt();
        addEdge(u, v);
        addEdge(v, u);
    }

    /* ---- BFS：求父亲 + BFS 序。用迭代而非递归，链深 1e6 也不爆栈 ---- */
    {
        int qh = 0, qt = 0;
        order_[qt++] = 1;
        parent_[1] = 0;
        while (qh < qt) {
            int u = order_[qh++];
            for (int e = head[u]; e; e = nxt_[e]) {
                int v = to_[e];
                if (v == parent_[u]) continue;   // 树上只需跳过父亲
                parent_[v] = u;
                order_[qt++] = v;
            }
        }
    }

    /* ---- 读命令：不真正做加法，只把贡献记进两个桶 ---- */
    int m = FastIO::readInt();
    for (int i = 0; i < m; ++i) {
        int op = FastIO::readInt();
        int x  = FastIO::readInt();
        int y  = FastIO::readInt();
        if (op == 1) raw1[x] += y;   // 子树加 —— 记在子树的根上
        else         A[x]    += y;   // 邻域加 —— 记在中心点上
    }

    /* ---- 操作 1 的贡献：自顶向下前缀和 ---- */
    for (int i = 0; i < n; ++i) {
        int u = order_[i];
        delta1[u] = raw1[u] + (parent_[u] ? delta1[parent_[u]] : 0LL);
    }

    /* ---- 操作 2 的第三项：把每个点的 A 累加给它父亲 ---- */
    for (int v = 1; v <= n; ++v)
        if (parent_[v]) childSum[parent_[v]] += A[v];

    /* ---- 回答询问 ---- */
    int q = FastIO::readInt();
    for (int i = 0; i < q; ++i) {
        int u = FastIO::readInt();
        ll ans = a[u]
               + delta1[u]                              // 操作 1：u 在 x 的子树里
               + (ll)A[u]                               // 操作 2：u 就是中心 x
               + (parent_[u] ? (ll)A[parent_[u]] : 0LL) // 操作 2：u 是中心的父亲
               + childSum[u];                           // 操作 2：u 是中心的儿子
        FastIO::putLL(ans);
        FastIO::putChar('\n');
    }
    FastIO::flushOut();
    return 0;
}
