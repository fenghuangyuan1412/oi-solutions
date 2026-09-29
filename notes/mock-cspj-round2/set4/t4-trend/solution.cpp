#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MOD = 998244353;
const int N = 5005;
int p[N], f[N], g[N];

int main() {
    int n, d;
    scanf("%d %d", &n, &d);
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);

    int best = 0;
    ll cnt = 0;
    for (int i = 1; i <= n; i++) {
        f[i] = 1;
        g[i] = 1;  // 只选自己这一天
        for (int j = max(1, i - d); j < i; j++) {
            if (p[j] >= p[i]) continue;  // 必须严格上升
            if (f[j] + 1 > f[i]) {
                f[i] = f[j] + 1;
                g[i] = g[j];
            } else if (f[j] + 1 == f[i]) {
                g[i] += g[j];
                if (g[i] >= MOD) g[i] -= MOD;
            }
        }
        if (f[i] > best) {
            best = f[i];
            cnt = g[i];
        } else if (f[i] == best) {
            cnt += g[i];
            if (cnt >= MOD) cnt -= MOD;
        }
    }
    printf("%d %lld\n", best, cnt % MOD);
    return 0;
}
