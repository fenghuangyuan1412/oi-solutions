#include <bits/stdc++.h>
using namespace std;

const int N = 1005;         // R <= 1000
int r, a[N][N];
int f[N];                   // 滚动的一行：f[j] = 从第 i 行第 j 格出发到塔底的最大路径和

// 结构观察（本题命门）：算第 i 行只用得到第 i+1 行的 f[j] 和 f[j+1]，
// 所以自底向上填表、一行滚动作废，空间从 O(R^2) 降到 O(R)。
// 如果写"自顶向下"，最后一行还要遍历整行取 max，两个方向的表含义不同，初值也不同。
int main() {
    scanf("%d", &r);
    for (int i = 1; i <= r; i++)
        for (int j = 1; j <= i; j++) scanf("%d", &a[i][j]);

    for (int j = 1; j <= r; j++) f[j] = a[r][j];        // 塔底：自己就是最大值，不用转移
    for (int i = r - 1; i >= 1; i--)
        for (int j = 1; j <= i; j++)
            f[j] = a[i][j] + max(f[j], f[j + 1]);       // 左下 = f[j]，右下 = f[j+1]

    printf("%d\n", f[1]);
    return 0;
}
