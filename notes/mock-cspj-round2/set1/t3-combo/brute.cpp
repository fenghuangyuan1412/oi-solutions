#include <bits/stdc++.h>
using namespace std;

int a[305], f[305];

// 暴力：f[i] = 前 i 只怪物里最多能打几段。枚举最后一段 [j,i] 是否恰好等于 k。
int main() {
    int n;
    long long k;
    scanf("%d %lld", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= n; i++) {
        f[i] = f[i - 1];
        long long s = 0;
        for (int j = i; j >= 1; j--) {
            s += a[j];
            if (s > k) break;
            if (s == k) f[i] = max(f[i], f[j - 1] + 1);
        }
    }
    printf("%d\n", f[n]);
    return 0;
}
