// P9751 [CSP-J 2023] 旅游巴士 —— 骗分版（两条精确路线，按数据自动二选一）
// 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o <输出>
//
// 路线①：全部 a_i = 0（测试点 1~2、6~7、11~13）⇒ 分层 BFS，O(k(n+m))。
//   时刻限制消失后只剩"走多长"：把状态写成 (地点 v, 已走步数 mod k)，
//   BFS 求 (1,0) 到 (n,0) 的最短步数；走不到就是 -1。
// 路线②：一般情况且 n、m 很小（测试点 3~5：n≤10, m≤15）⇒ 按时间逐秒模拟，精确但慢。
//   R_t = {1（若 k|t，表示这刻刚来一辆车、可以上车开走）}
//         ∪ {v : 存在边 u→v，u∈R_{t-1} 且 t-1 ≥ a}   （走 1 单位，出发时刻不早于 a）
//   答案 = 最小的 t≡0(mod k) 使 n∈R_t。
//   上界：若存在方案，最早离开时刻 ≤ max(a) + (n+1)k（见 README"界引理"），模拟到 U 仍无解 ⇒ -1。
// 实测：n=10 级最坏（U≈1e6，m=15）16 ms；n=1e4/m=2e4 的通用数据实测跑不完（见 README）。
#include <bits/stdc++.h>
using namespace std;

struct Edge { int v, a; };

int main() {
    int n, m, k;
    if (scanf("%d%d%d", &n, &m, &k) != 3) return 0;
    vector<vector<Edge>> out(n + 1);
    int maxa = 0;
    bool allzero = true;
    for (int i = 0; i < m; ++i) {
        int u, v, a;
        scanf("%d%d%d", &u, &v, &a);
        out[u].push_back({v, a});
        if (a > maxa) maxa = a;
        if (a != 0) allzero = false;
    }

    // ---------- 路线①：a_i 全为 0 ----------
    if (allzero) {
        // dist[v][r]：从 (1,0) 走到 (v, r) 的最短步数；BFS 天然允许重复经过点/边
        vector<vector<int>> dist(n + 1, vector<int>(k, -1));
        queue<pair<int, int>> q;
        dist[1][0] = 0;
        q.push({1, 0});
        while (!q.empty()) {
            int u = q.front().first, r = q.front().second;
            q.pop();
            for (const Edge &e : out[u]) {
                int nr = (r + 1) % k;
                if (dist[e.v][nr] == -1) {
                    dist[e.v][nr] = dist[u][r] + 1;
                    q.push({e.v, nr});
                }
            }
        }
        printf("%d\n", dist[n][0]);   // n≥2，长度 0 的"原地不动"不会被误当成答案
        return 0;
    }

    // ---------- 路线②：逐时刻模拟（小数据精确解） ----------
    long long U = (long long)maxa + (long long)(n + 1) * k + 5;   // 界引理给的模拟上限
    vector<char> cur(n + 1, 0), nxt(n + 1, 0);
    vector<int> act, nact;
    cur[1] = 1;
    act.push_back(1);                       // 0 时刻：第一班车刚到入口
    for (long long t = 1; t <= U; ++t) {
        fill(nxt.begin(), nxt.end(), 0);
        nact.clear();
        auto mark = [&](int v) { if (!nxt[v]) { nxt[v] = 1; nact.push_back(v); } };
        if (t % k == 0) mark(1);            // 这刻到站一辆新车 ⇒ 可以从入口重新出发
        for (int u : act)
            for (const Edge &e : out[u])
                if (t - 1 >= (long long)e.a) mark(e.v);   // 第 t-1 刻出发踩这条边
        cur.swap(nxt);
        act.swap(nact);
        if (t % k == 0 && cur[n]) { printf("%lld\n", t); return 0; }
    }
    puts("-1");
    return 0;
}
