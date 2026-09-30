#include <bits/stdc++.h>
using namespace std;

// 中间档（约 70 分）：承认"每列只走一个方向"这个结构，但枚举进列点 k 时不做优化。
//
//   f[i][j] = 走到 (i,j)、马上要向右离开本列 的最大分数
//   转移：上一列停在第 k 行 ⇒ 从 (k,j) 进本列 ⇒ 在本列沿单一方向直走到第 i 行
//         这一段拿到的格子是 min(k,i)..max(k,i)，用前缀和 O(1) 求段和。
//   f[i][j] = max over k of ( f[k][j-1] + sum(a[min..max][j]) )
//
// 复杂度：枚举 (i,j) 再枚举 k ⇒ O(n^2 * m)。
//   n=m=300：300*300*300 = 2.7e7，稳稳能过；
//   n=m=1000：1e9，必挂。把内层"枚举 k"用前/后缀最大值一次扫出来就是 O(nm)，
//             见同目录 solution.cpp。
// 本文件同时是 O(nm) 版的第二份对拍参照：两份写法独立推导，答案必须逐字相同。

const int N = 1005;
const long long NEG = -(1LL << 50);

int n, m;
int a[N][N];
long long f[N][N];
long long pre[N];   // pre[i] = a[1][j]+...+a[i][j]，每换一列重算，pre[0]=0

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[i][j];

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= m; j++) f[i][j] = NEG;

    // 第 1 列：从 (1,1) 出发只能一路向下，没有选择
    long long acc = 0;
    for (int i = 1; i <= n; i++) { acc += a[i][1]; f[i][1] = acc; }

    for (int j = 2; j <= m; j++) {
        for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i][j];
        for (int i = 1; i <= n; i++) {
            long long best = NEG;
            for (int k = 1; k <= n; k++) {                    // 枚举"从上一列的第 k 行跨进来"
                if (f[k][j - 1] <= NEG / 2) continue;
                int lo = min(i, k), hi = max(i, k);           // 段和只跟区间端点有关，跟方向无关
                best = max(best, f[k][j - 1] + (pre[hi] - pre[lo - 1]));
            }
            f[i][j] = best;
        }
    }

    cout << f[n][m] << '\n';
    return 0;
}
