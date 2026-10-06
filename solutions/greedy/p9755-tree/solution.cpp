/*
 * P9755 [CSP-S 2023] 种树 —— 骗分 / 暴力版
 * ---------------------------------------------------------------------------
 * 题意（已用样例 1 校准）：
 *   n 片地块构成一棵树，1 号是入口。每天可以选一片「未种树、且与某片已种树的地块
 *   相邻」的地块种一棵高 0 的树；第 1 天只能在 1 号种。
 *   第 t 天，i 号地块上的树长高 b[i] + t * c[i] 米（不足 1 按 1 算），t 是【全局天数】，
 *   从 1 起算，且【种下的当天就开始长】。
 *   目标：所有地块的树都不低于 h[i] 米，求最少天数。
 *
 * 正解（不在本文件里）：二分答案 + 按「每块地最晚能拖到第几天种」排序后贪心，
 *   复杂度 O(n log T log n)。
 *
 * 本文件是【骗分版】：暴力枚举所有合法的种树顺序（只对 n 很小的时候可行），
 * 对每个顺序二分总天数 T，取所有顺序里的最小值。
 *   · n <= 8 时结果【正确】（合法顺序有限，全部试过）；
 *   · n 大时退化成 BFS 顺序的近似解，答案可能偏大 —— 这是"部分分"的含义。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 */
#include <bits/stdc++.h>
using namespace std;
typedef __int128 i128;

int n;
vector<long long> H, B, C;
vector<vector<int>> g;

/* 地块 i 在第 [l, r] 天（含）内的总生长量，O(1) 闭式 */
i128 sumGrow(int i, long long l, long long r) {
    if (l > r) return 0;
    long long b = B[i], c = C[i], cnt = r - l + 1;
    if (c >= 0)
        return (i128)cnt * b + (i128)c * (i128)(l + r) * cnt / 2;
    long long mxc = (b - 1) / (-c);              // t <= mxc 时 b + t*c >= 1
    if (mxc < l) return cnt;                     // 整段都是每天 1
    if (mxc >= r)
        return (i128)cnt * b + (i128)c * (i128)(l + r) * cnt / 2;
    i128 p1 = (i128)(mxc - l + 1) * b
            + (i128)c * (i128)(l + mxc) * (mxc - l + 1) / 2;
    return p1 + (r - mxc);
}

/* 给定"每块地第几天种"，判断总天数 T 够不够 */
bool check(long long T, const vector<long long>& day) {
    for (int i = 1; i <= n; ++i) {
        if (day[i] > T) return false;
        if (sumGrow(i, day[i], T) < (i128)H[i]) return false;
    }
    return true;
}

/* 在固定顺序下二分最少天数 */
long long solveOrder(const vector<int>& order) {
    vector<long long> day(n + 1, 0);
    for (int i = 0; i < n; ++i) day[order[i]] = i + 1;
    long long lo = n, hi = 1000000000LL;
    if (!check(hi, day)) return LLONG_MAX;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (check(mid, day)) hi = mid; else lo = mid + 1;
    }
    return lo;
}

long long bestAns = LLONG_MAX;

void dfs(int day, vector<int>& order, vector<char>& planted) {
    if (day > n) { bestAns = min(bestAns, solveOrder(order)); return; }
    for (int v = 1; v <= n; ++v) {
        if (planted[v]) continue;
        if (day == 1) { if (v != 1) continue; }        // 第 1 天只能种 1 号
        else {
            bool ok = false;
            for (int u : g[v]) if (planted[u]) { ok = true; break; }
            if (!ok) continue;                          // 必须与已种地块相邻
        }
        planted[v] = 1; order.push_back(v);
        dfs(day + 1, order, planted);
        order.pop_back(); planted[v] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    H.assign(n + 1, 0); B.assign(n + 1, 0); C.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) cin >> H[i] >> B[i] >> C[i];
    g.assign(n + 1, {});
    for (int i = 1; i < n; ++i) {
        int u, v; cin >> u >> v;
        g[u].push_back(v); g[v].push_back(u);
    }

    if (n <= 8) {                                       // 小数据：枚举所有合法顺序
        vector<int> order; vector<char> planted(n + 1, 0);
        dfs(1, order, planted);
        cout << bestAns << "\n";
    } else {                                            // 大数据：BFS 顺序近似
        vector<long long> day(n + 1, -1);
        queue<int> q; q.push(1); day[1] = 1;
        long long ord = 1;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (day[v] == -1) { day[v] = ++ord; q.push(v); }
        }
        long long lo = n, hi = 1000000000LL;
        while (lo < hi) {
            long long mid = lo + (hi - lo) / 2;
            if (check(mid, day)) hi = mid; else lo = mid + 1;
        }
        cout << lo << "\n";
    }
    return 0;
}
