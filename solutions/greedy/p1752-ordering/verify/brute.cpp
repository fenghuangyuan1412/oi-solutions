// verify/brute.cpp —— 网络流暴力（仅用于验证，非讲解代码）
// 与 solution.cpp 完全独立的第二判定器：
//   二分周数改为从 1 开始线性扫；单周可行性用最大流判定——
//   S -> 人（容量 = week），人 -> 菜（容量 1，仅当该人能点这道菜），菜 -> T（容量 1），
//   最大流 == m 当且仅当 week 周内能把 m 道菜全点一遍。
// 编译：g++ -static -O2 -std=c++14 brute.cpp -o brute.exe
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Dinic {
    struct E { int to; ll cap; int rev; };
    vector<vector<E>> g;
    vector<int> level, iter;
    Dinic(int n) : g(n), level(n), iter(n) {}
    void add(int a, int b, ll c) {
        g[a].push_back({b, c, (int)g[b].size()});
        g[b].push_back({a, 0, (int)g[a].size() - 1});
    }
    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> que;
        level[s] = 0; que.push(s);
        while (!que.empty()) {
            int v = que.front(); que.pop();
            for (auto &e : g[v]) if (e.cap > 0 && level[e.to] < 0) {
                level[e.to] = level[v] + 1;
                que.push(e.to);
            }
        }
        return level[t] >= 0;
    }
    ll dfs(int v, int t, ll f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < (int)g[v].size(); i++) {
            E &e = g[v][i];
            if (e.cap > 0 && level[v] < level[e.to]) {
                ll d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) { e.cap -= d; g[e.to][e.rev].cap += d; return d; }
            }
        }
        return 0;
    }
    ll maxflow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(iter.begin(), iter.end(), 0);
            ll f;
            while ((f = dfs(s, t, LLONG_MAX)) > 0) flow += f;
        }
        return flow;
    }
};

int n, m, p, q;
ll delv[205], priv[205], lowv[205], capv[205];

// 第 person 个人（0..n-1，前 p 个挑剔、接着 q 个贫穷、其余普通人）能否点第 dish 道菜
bool acceptable(int person, int dish) {
    if (person < p) return delv[dish] >= lowv[person];
    if (person < p + q) return priv[dish] <= capv[person - p];
    return true;
}

bool feasible(ll week) {
    int S = 0, T = n + m + 1;
    Dinic d(n + m + 2);
    for (int i = 0; i < n; i++) d.add(S, 1 + i, week);
    for (int j = 0; j < m; j++) d.add(1 + n + j, T, 1);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (acceptable(i, j)) d.add(1 + i, 1 + n + j, 1);
    return d.maxflow(S, T) == m;
}

int main() {
    scanf("%d%d%d%d", &n, &m, &p, &q);
    for (int i = 0; i < m; i++) scanf("%lld%lld", &delv[i], &priv[i]);
    for (int i = 0; i < p; i++) scanf("%lld", &lowv[i]);
    for (int i = 0; i < q; i++) scanf("%lld", &capv[i]);

    // 线性扫（不共享正解的二分逻辑，让两份代码连外层都独立）
    for (int week = 1; week <= m; week++)
        if (feasible(week)) { printf("%d\n", week); return 0; }
    printf("-1\n");
    return 0;
}
