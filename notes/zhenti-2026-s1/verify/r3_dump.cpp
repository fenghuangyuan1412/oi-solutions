// r3_dump.cpp -- 第 3 篇插桩版（结构分析用）
// 与原文唯一的改动：把 he[i] = rnd() 换成 he[i] = 1ull << (bitcnt++)，
// 即每条"返边"分配一个独立的二进制位，于是 he[i] 的 1 位集合 = 覆盖该边的返边集合，
// 完全消除异或哈希冲突的可能性（原题"不考虑异或哈希值的冲突"的强化版）。
// 额外在 stderr 打印每条边的分类（割边 / ct==1 / ct>=2）与 ans[]。
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
const int N = 100005, M = 200005, S = 400037;
const int P = 998244353, iv2 = (P + 1) / 2;
int n, m, w[M], sum, dep[N], ct[M], sw[M], cur[M], ans[N];
ull h[N], he[M];
bool vis[N];
vector<pair<int, int> > G[N], T[N];
int bitcnt = 0;                       // <== 替代 rnd()
int EU[M], EV[M];                     // 记录边端点，只为打印
int par[N], pedge[N];
int tot, hd[S];
struct node { int nxt; ull key; } mp[M];
int getid(ull key) {
    int u = key % S;
    for (int i = hd[u]; i; i = mp[i].nxt)
        if (mp[i].key == key) return i;
    return mp[++tot] = {hd[u], key}, hd[u] = tot;
}
void dfs1(int u, int p) {
    vis[u] = 1;
    for (auto [v, i] : G[u]) if (i ^ p) {
        if (!vis[v]) {
            dep[v] = dep[u] + 1, par[v] = u, pedge[v] = i;
            T[u].push_back({v, i});
            dfs1(v, i); h[u] ^= h[v]; he[i] = h[v];
        } else if (dep[v] < dep[u]) {
            if (bitcnt >= 60) { fprintf(stderr, "BIT MODE overflow\n"); exit(2); }
            he[i] = 1ull << (bitcnt++);
            h[u] ^= he[i]; h[v] ^= he[i];
        }
    }
}
vector<int> br_status;                // per edge: 0=bridge 1=ct==1 2=ct>=2 (仅树边)
void dfs2(int u, ll c, ll b, ll b2) {
    ans[u] = ((c + b * sum - (b * b + b2) % P * iv2) % P + P) % P;
    for (auto [v, i] : T[u]) {
        ll w = ::w[i];
        if (!he[i]) {
            br_status[i] = 0;
            dfs2(v, c, (b + w) % P, (b2 + w * w) % P);
            continue;
        }
        int j = getid(he[i]);
        if (ct[j] == 1) { br_status[i] = 1; dfs2(v, c, b, b2); }
        else {
            br_status[i] = 2;
            int d = (w * (sw[j] - cur[j] * 2 - w) % P + P) % P;
            cur[j] = (cur[j] + w) % P;
            dfs2(v, (c + d) % P, b, b2);
            cur[j] = (cur[j] - w + P) % P;
        }
    }
}
int main() {
    ios::sync_with_stdio(0), cin.tie(0);
    cin >> n >> m;
    for (int i = 1, u, v; i <= m; i++)
        cin >> u >> v >> w[i], sum = (sum + w[i]) % P,
        EU[i] = u, EV[i] = v,
        G[u].push_back({v, i}), G[v].push_back({u, i});
    br_status.assign(m + 1, -1);       // -1 = 不是树边（返边）
    dfs1(1, 0);
    for (int i = 1, j; i <= m; i++) if (he[i])
        j = getid(he[i]), ct[j]++, sw[j] = (sw[j] + w[i]) % P;
    dfs2(1, 0, 0, 0);
    // ---- dump ----
    fprintf(stderr, "N=%d M=%d sum=%d\n", n, m, sum);
    for (int i = 1; i <= m; i++) {
        int j = he[i] ? getid(he[i]) : 0;
        fprintf(stderr, "EDGE %d (%d,%d) w=%d he=%llx backedges=%llx bucket=%d ct=%d sw=%d status=%d\n",
                i, EU[i], EV[i], w[i], (unsigned long long)he[i], (unsigned long long)he[i],
                j, j ? ct[j] : 0, j ? sw[j] : 0, br_status[i]);
    }
    for (int u = 1; u <= n; u++)
        fprintf(stderr, "ANS %d %d\n", u, ans[u]);
    for (int u = 1; u <= n; u++)
        fprintf(stderr, "DEP %d %d par=%d pedge=%d h=%llx\n", u, dep[u], par[u], pedge[u], (unsigned long long)h[u]);
    for (int i = 1; i <= n; i++) cout << ans[i] << (i == n ? '\n' : ' ');
    return 0;
}
