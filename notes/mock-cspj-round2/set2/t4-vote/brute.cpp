#include <bits/stdc++.h>
using namespace std;

int a[15][105], k[15];
int m, S;
long long ans;

// 暴力：每天枚举投哪个项目，直接 DFS 到底
void dfs(int day, int sum, int last) {
    if (sum > S) return;
    if (day > m) {
        if (sum == S) ans++;
        return;
    }
    for (int j = 1; j <= k[day]; j++) {
        if (a[day][j] == last) continue;  // 相邻两天热度值不能相同
        dfs(day + 1, sum + a[day][j], a[day][j]);
    }
}

int main() {
    scanf("%d %d", &m, &S);
    for (int i = 1; i <= m; i++) {
        scanf("%d", &k[i]);
        for (int j = 1; j <= k[i]; j++) scanf("%d", &a[i][j]);
    }
    ans = 0;
    dfs(1, 0, -1);
    printf("%lld\n", ans % 998244353LL);
    return 0;
}
