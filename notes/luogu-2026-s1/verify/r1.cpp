// r1.cpp -- 洛谷 SCP-S1 2026 阅读程序(1)，严格按原文还原（P4, 行号 1..20）
#include <bits/stdc++.h>
using namespace std;
// 1 
const int N = 20, mod = 998244353;                          // 4
int n, a[N][N], f[1 << N];                                  // 5
// 6
int main() {                                                // 7
    cin >> n;                                               // 8
    for (int i = 0; i < n; i++)                             // 9
        for (int j = 0; j < n; j++)                         // 10
            cin >> a[i][j];                                 // 11
    f[0] = 1;                                               // 12
    for (int i = 0; i < (1 << n); i++)                      // 13
        for (int j = 0; j < n; j++)                         // 14
            if (i >> j & 1)                                 // 15
                (f[i] += 1ll * f[i ^ (1 << j)] *            // 16
                          a[__builtin_popcount(i) - 1][j] % mod) %= mod; // 17
    cout << f[(1 << n) - 1];                                // 18
    return 0;                                               // 19
}
