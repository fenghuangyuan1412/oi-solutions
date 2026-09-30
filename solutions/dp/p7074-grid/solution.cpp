#include <bits/stdc++.h>
using namespace std;

const int N = 1005;
const long long NEG = -(1LL << 50);

int n, m;
int a[N][N];
long long f[N][N][2];   // f[i][j][0]：到达 (i,j)，本列这一段是"从上往下"走过来的
                       // f[i][j][1]：到达 (i,j)，本列这一段是"从下往上"走过来的

// 结构观察（本题命门）：只能向右/向上/向下、且不能重复经过 ⇒
//   每一列里路径必是"从左边第 k 行跨进来，沿单一方向走到第 i 行，再向右离开"。
//   列内不可能先上后下（必踩回头路），所以每列只要两个方向状态。
// 记本列前缀和 pre[i] = a[1][j]+...+a[i][j]，best[k] = max(f[k][j-1][0], f[k][j-1][1])：
//   向下 f[i][j][0] = max( f[i-1][j][0] + a[i][j],           // 本列继续往下
//                          pre[i] + max_{k<=i}( best[k] - pre[k-1] ) )
//   向上 f[i][j][1] = max( f[i+1][j][1] + a[i][j],
//                         -pre[i-1] + max_{k>=i}( best[k] + pre[k] ) )
//   （向上那格的区间是 [i, k]，减去的是 pre[i-1]；写成 pre[i] 会把 a[i][j] 漏掉，
//     样例 1 就会从 9 变成 11 —— 漏掉的恰好是负数格，答案反而偏大，最难查）
// 两个 max 一边扫一边维护 ⇒ 总复杂度 O(nm)，n=m=1000 时 1e6 次运算。
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[i][j];

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++)
            f[i][j][0] = f[i][j][1] = NEG;      // 数字可为负：初值必须是 -INF，写 0 会把负数路径全丢掉

    // 第 1 列：只能从 (1,1) 出发一路向下
    f[1][1][0] = f[1][1][1] = a[1][1];
    for (int i = 2; i <= n; i++) f[i][1][0] = f[i - 1][1][0] + a[i][1];

    static long long pre[N];
    for (int j = 2; j <= m; j++) {
        for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i][j];   // pre[0] = 0（static 零初始化）

        long long best = NEG;                                          // max_{k<=i}( best[k] - pre[k-1] )
        for (int i = 1; i <= n; i++) {
            long long up = max(f[i][j - 1][0], f[i][j - 1][1]);
            if (up > NEG / 2) best = max(best, up - pre[i - 1]);
            long long v = best + pre[i];
            if (i > 1 && f[i - 1][j][0] > NEG / 2) v = max(v, f[i - 1][j][0] + a[i][j]);
            f[i][j][0] = v;
        }

        best = NEG;                                                    // max_{k>=i}( best[k] + pre[k] )
        for (int i = n; i >= 1; i--) {
            long long up = max(f[i][j - 1][0], f[i][j - 1][1]);
            if (up > NEG / 2) best = max(best, up + pre[i]);
            long long v = best - pre[i - 1];
            if (i < n && f[i + 1][j][1] > NEG / 2) v = max(v, f[i + 1][j][1] + a[i][j]);
            f[i][j][1] = v;
        }
    }

    cout << max(f[n][m][0], f[n][m][1]) << '\n';
    return 0;
}
