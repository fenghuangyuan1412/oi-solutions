// r1_noll.cpp -- 第 16 行删掉 1ll* 的版本（题 18 反例用）
#include <bits/stdc++.h>
using namespace std;
const int N = 20, mod = 998244353;
int n, a[N][N], f[1 << N];
int main() {
    cin >> n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> a[i][j];
    f[0] = 1;
    for (int i = 0; i < (1 << n); i++)
        for (int j = 0; j < n; j++)
            if (i >> j & 1)
                (f[i] += f[i ^ (1 << j)] *                 // 16: 没有 1ll*
                          a[__builtin_popcount(i) - 1][j] % mod) %= mod;
    cout << f[(1 << n) - 1];
    return 0;
}
