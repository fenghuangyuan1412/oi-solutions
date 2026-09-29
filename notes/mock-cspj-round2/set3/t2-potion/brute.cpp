#include <bits/stdc++.h>
using namespace std;

const int N = 100005;
int v[N], a[N];

// 另一套写法：每一段都从头扫一遍找最便宜的购买点，O(n^2)，不维护任何"历史最低"状态
int main() {
    int n;
    long long d;
    scanf("%d %lld", &n, &d);
    for (int i = 1; i < n; i++) scanf("%d", &v[i]);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

    long long ans = 0;
    for (int i = 1; i < n; i++) {
        int best = i, bestp = a[i];
        for (int j = 1; j <= i; j++) {
            if (a[j] < bestp) {
                bestp = a[j];
                best = j;
            }
        }
        ans += 1LL * (v[i] / d) * bestp;
    }
    printf("%lld\n", ans);
    return 0;
}
