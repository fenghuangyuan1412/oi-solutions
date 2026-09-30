#include <bits/stdc++.h>
using namespace std;

const int M = 85;
int m;                       // 当前处理这一行有多少个数
long long a[M];              // 本行的数（1 基）
long long f[M][M];           // f[l][r] = 区间 [l,r] 还没取，剩下的数按规则取完能拿的最大分
long long pw[M];             // pw[i] = 2^i

// 单行求解：区间 DP
//   设本行共 m 个数，区间 [l,r] 是"还没取"的部分，那么已经取走了 m-(r-l+1) 个，
//   下一次取数就是第 k = m-(r-l+1)+1 次，乘数是 2^k。
//   f[l][r] = max( a[l]*2^k + f[l+1][r],  a[r]*2^k + f[l][r-1] )
//   边界：l > r 时 f = 0。答案 f[1][m]。
long long solveRow() {
    for (int len = 1; len <= m; len++) {           // 按区间长度从小到大填表
        for (int l = 1; l + len - 1 <= m; l++) {
            int r = l + len - 1;
            int k = m - len + 1;                   // 这是第 k 次取数
            f[l][r] = max(a[l] * pw[k] + f[l + 1][r],
                          a[r] * pw[k] + f[l][r - 1]);
        }
    }
    return f[1][m];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n >> m;
    pw[0] = 1;
    for (int i = 1; i <= m; i++) pw[i] = pw[i - 1] * 2;   // 2^80 早就爆 long long 了 ← 本题的 40 分

    long long total = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) cin >> a[j];
        memset(f, 0, sizeof f);          // 每行独立，行与行之间只是把得分相加
        total += solveRow();
    }

    cout << total << '\n';
    return 0;
}
