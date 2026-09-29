#include <bits/stdc++.h>
using namespace std;

int a[25];

// 暴力：n <= 20，直接枚举 2^n 个子集
int main() {
    int n;
    long long S;
    scanf("%d %lld", &n, &S);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    long long ans = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        long long s = 0;
        for (int i = 0; i < n; i++) if (mask >> i & 1) s += a[i];
        if (s > S) ans++;
    }
    printf("%lld\n", ans % 998244353);
    return 0;
}
