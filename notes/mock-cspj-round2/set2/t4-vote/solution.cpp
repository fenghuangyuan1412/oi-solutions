#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int N = 5005, V = 105;
int dp[N][V], ndp[N][V], tot[N], cnt[V];

int main() {
    int m, S;
    scanf("%d %d", &m, &S);
    for (int i = 1; i <= m; i++) {
        int k;
        scanf("%d", &k);
        memset(cnt, 0, sizeof cnt);
        for (int j = 0; j < k; j++) {
            int v;
            scanf("%d", &v);
            if (v <= S) cnt[v]++;  // 单个热度就超过 S 的项目永远用不上
        }
        if (i == 1) {
            memset(dp, 0, sizeof dp);
            for (int v = 1; v <= S && v < V; v++) dp[v][v] = cnt[v];
        } else {
            memset(ndp, 0, sizeof ndp);
            for (int v = 1; v <= S && v < V; v++) {
                if (!cnt[v]) continue;
                for (int s = v; s <= S; s++) {
                    long long ways = tot[s - v] - dp[s - v][v];  // 排除"昨天也是这个热度"
                    if (ways < 0) ways += MOD;
                    ndp[s][v] = ways * cnt[v] % MOD;
                }
            }
            memcpy(dp, ndp, sizeof dp);
        }
        memset(tot, 0, sizeof tot);
        for (int s = 1; s <= S; s++) {
            long long t = 0;
            for (int v = 1; v < V; v++) t += dp[s][v];
            tot[s] = t % MOD;
        }
    }
    printf("%d\n", tot[S]);
    return 0;
}
