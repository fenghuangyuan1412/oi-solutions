#include <bits/stdc++.h>
using namespace std;

const int N = 1000005;
int a[N];

int main() {
    int n;
    long long k;
    scanf("%d %lld", &n, &k);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);

    long long ans = 0, sum = 0;
    int l = 1;
    for (int r = 1; r <= n; r++) {
        sum += a[r];
        while (l <= r && sum > k) {  // 血量和太大，左端点右移（a 全为正，和单调）
            sum -= a[l];
            l++;
        }
        if (sum == k) {  // 以 r 结尾能成段——立刻收割，给后面留最多怪物
            ans++;
            sum = 0;
            l = r + 1;
        }
    }
    printf("%lld\n", ans);
    return 0;
}
