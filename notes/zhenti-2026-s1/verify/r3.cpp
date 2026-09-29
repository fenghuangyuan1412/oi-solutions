// r3.cpp -- 洛谷 SCP-S1 2026 阅读程序(3)，严格按原文还原（P8-P10, 行号 1..65）
#include <bits/stdc++.h>
using namespace std;
// 3
typedef long long ll;                                            // 4
typedef unsigned long long ull;                                  // 5
const int N = 100005, M = 200005, S = 400037;                    // 6
const int P = 998244353, iv2 = (P + 1) / 2;                      // 7
// 8
int n, m, w[M], sum, dep[N], ct[M], sw[M], cur[M], ans[N];      // 9
ull h[N], he[M];                                                 // 10
bool vis[N];                                                     // 11
vector<pair<int, int> > G[N], T[N];                              // 12
mt19937_64 rnd(random_device{}());                               // 13
// 14
int tot, hd[S];                                                  // 15
struct node { int nxt; ull key; } mp[M];                         // 16
int get(ull key) {                                               // 17
    int u = key % S;                                             // 18
    for (int i = hd[u]; i; i = mp[i].nxt)                        // 19
        if (mp[i].key == key) return i;                          // 20
    return mp[++tot] = {hd[u], key}, hd[u] = tot;                // 21
}                                                                // 22
void dfs1(int u, int p) {                                        // 24
    vis[u] = 1;                                                  // 25
    for (auto [v, i] : G[u]) if (i ^ p) {                        // 26
        if (!vis[v])                                             // 27
            dep[v] = dep[u] + 1, T[u].push_back({v, i}),         // 28
            dfs1(v, i), h[u] ^= h[v], he[i] = h[v];              // 29
        else if (dep[v] < dep[u])                                // 30
            he[i] = rnd(), h[u] ^= he[i], h[v] ^= he[i];         // 31
    }                                                            // 32
}                                                                // 33
void dfs2(int u, ll c, ll b, ll b2) {                            // 35
    ans[u] = ((c + b * sum - (b * b + b2) % P * iv2) % P + P) % P;  // 36
    for (auto [v, i] : T[u]) {                                   // 37
        ll w = ::w[i];                                           // 38
        if (!he[i]) {                                            // 39
            dfs2(v, c, (b + w) % P, (b2 + w * w) % P);           // 40
            continue;                                            // 41
        }                                                        // 42
        int j = get(he[i]);                                      // 43
        if (ct[j] == 1) dfs2(v, c, b, b2);                       // 44
        else {                                                   // 45
            int d = (w * (sw[j] - cur[j] * 2 - w) % P + P) % P;  // 46
            cur[j] = (cur[j] + w) % P;                           // 47
            dfs2(v, (c + d) % P, b, b2);                         // 48
            cur[j] = (cur[j] - w + P) % P;                       // 49
        }                                                        // 50
    }                                                            // 51
}                                                                // 52
int main() {                                                     // 54
    ios::sync_with_stdio(0), cin.tie(0);                         // 55
    cin >> n >> m;                                               // 56
    for (int i = 1, u, v; i <= m; i++)                           // 57
        cin >> u >> v >> w[i], sum = (sum + w[i]) % P,           // 58
        G[u].push_back({v, i}), G[v].push_back({u, i});          // 59
    dfs1(1, 0);                                                  // 60
    for (int i = 1, j; i <= m; i++) if (he[i])                   // 61
        j = get(he[i]), ct[j]++, sw[j] = (sw[j] + w[i]) % P;     // 62
    dfs2(1, 0, 0, 0);                                            // 63
    for (int i = 1; i <= n; i++) cout << ans[i] << ' ';          // 64
}                                                                // 65
