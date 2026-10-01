#include <bits/stdc++.h>
using namespace std;

const int N = 10005;
int n, m;
long long f[N];            // f[j] = 恰好花掉 j 元的点菜方案数

// 命门一：f[0] = 1。"什么都不点"是花 0 元的唯一方案，它是所有转移的起点；写 0 会导致答案恒为 0。
// 命门二：内层倒序。每种菜只有一份，倒序保证第 i 道菜只从"还没考虑它"的旧值转移过来。
// 命门三：答案保证不超过 int，但 f[j]（j < m）不一定 —— 中间值可以远超 f[m]，所以这里用 long long。
int main() {
    scanf("%d%d", &n, &m);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        int a;
        scanf("%d", &a);
        for (int j = m; j >= a; j--) f[j] += f[j - a];
    }
    printf("%lld\n", f[m]);
    return 0;
}
