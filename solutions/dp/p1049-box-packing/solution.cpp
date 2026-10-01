#include <bits/stdc++.h>
using namespace std;

const int N = 20005;
int V, n;
int f[N];                 // f[j] = 容量 j 的箱子最多能装多少体积

// 装箱问题 = 01 背包的"体积当价值"变形：
//   每个物品体积 v_i，价值也是 v_i，求不超过 V 的最大总体积；答案输出 V - f[V]。
//   因为"价值 = 体积"，最优解一定是"装得下就尽量装满"，剩下的是装不进去的空隙。
int main() {
    scanf("%d%d", &V, &n);
    for (int i = 1; i <= n; i++) {
        int v;
        scanf("%d", &v);
        for (int j = V; j >= v; j--)
            f[j] = max(f[j], f[j - v] + v);
    }
    printf("%d\n", V - f[V]);
    return 0;
}
