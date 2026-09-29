#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
int v[N], a[N];

int main() {
    int n;
    long long d;
    scanf("%d %lld", &n, &d);
    for (int i = 1; i < n; i++) scanf("%d", &v[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

    long long ans = 0;
    int cheapest = a[1];  // 走到这里为止见过的最低单价
    for (int i = 1; i < n; i++) {
        cheapest = min(cheapest, a[i]);
        ans += 1LL * (v[i] / d) * cheapest;  // 这一段的车票，回溯到最便宜的点买
    }
    printf("%lld\n", ans);
    return 0;
}
