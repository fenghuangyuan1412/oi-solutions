#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, int> PLI;

const int N = 10005;
const ll INF = (1LL << 60);
vector<pair<int, int> > e[N];  // (终点, 开通时刻)
ll dist[N];
int n, m, k;

inline ll round_up(ll x) {  // 不小于 x 的第一个 k 的倍数
    return (x + k - 1) / k * k;
}

int main() {
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 1; i <= m; i++) {
        int u, v, a;
        scanf("%d %d %d", &u, &v, &a);
        e[u].push_back(make_pair(v, a));
    }
    for (int i = 1; i <= n; i++) dist[i] = INF;
    priority_queue<PLI, vector<PLI>, greater<PLI> > pq;
    dist[1] = 0;
    pq.push(make_pair(0LL, 1));
    while (!pq.empty()) {
        PLI cur = pq.top();
        pq.pop();
        int u = cur.second;
        if (cur.first != dist[u]) continue;  // 堆里的旧记录
        for (int i = 0; i < (int)e[u].size(); i++) {
            int v = e[u][i].first, a = e[u][i].second;
            ll depart = round_up(max(cur.first, (ll)a));  // 等到"既是 k 的倍数、又已开通"
            ll nd = depart + 1;
            if (nd < dist[v]) {
                dist[v] = nd;
                pq.push(make_pair(nd, v));
            }
        }
    }
    printf("%lld\n", dist[n] == INF ? -1 : dist[n]);
    return 0;
}
