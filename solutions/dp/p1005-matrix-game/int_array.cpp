#include <bits/stdc++.h>
using namespace std;

// 【反面教材，不是题解】和 solution.cpp 的逻辑一模一样，唯一的区别是把存数、存乘数、存 DP 值
// 的三个数组都写成 `int`：答案变量虽然是 `long long`，但乘法在 `int` 里就算完了，溢出发生在
// 相加之前 —— 正好对应 README「易错点与坑」第 1 条。
// 复现：g++ -static -O2 -std=c++14 -Wall int_array.cpp -o sol_int.exe
//       ./sol_int.exe < "1 30" + 30 个 1000 的那份数据   → 输出 -2000（真值 2147483646000）
const int M = 85;
int m;
int a[M];        // 本行的数（1 基）—— 应该是 long long
int f[M][M];     // f[l][r] = 区间 [l,r] 还没取时，取完能拿的最大分 —— 应该是 i128/高精度
int pw[M];       // pw[i] = 2^i —— 到 2^31 就爆 int

long long solveRow() {
    for (int len = 1; len <= m; len++) {
        for (int l = 1; l + len - 1 <= m; l++) {
            int r = l + len - 1;
            int k = m - len + 1;
            f[l][r] = max(a[l] * pw[k] + f[l + 1][r],
                          a[r] * pw[k] + f[l][r - 1]);   // 这一行全程按 int 算
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
    for (int i = 1; i <= m; i++) pw[i] = pw[i - 1] * 2;

    long long total = 0;                                 // 只有这里是 64 位，晚了
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) cin >> a[j];
        memset(f, 0, sizeof f);
        total += solveRow();
    }

    cout << total << '\n';
    return 0;
}
