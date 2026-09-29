// r1_count.cpp -- 带计数器的第 1 篇：统计
//   if_checks : 第 15 行 if 被执行（求值）的次数
//   if_true   : 第 15 行条件为真的次数（= 第 16/17 行循环体执行次数）
//   并记录实际访问到的最大下标：f 的最大下标、a 的行/列最大下标、1<<j 的最大值
// 用法: 输入 n 后跟 n*n 个整数（同 r1）
#include <bits/stdc++.h>
using namespace std;
const int N = 20, mod = 998244353;
int n, a[N][N], f[1 << N];
int main() {
    cin >> n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> a[i][j];
    long long if_checks = 0, if_true = 0, body = 0;
    long long max_f_idx = 0, max_row = -1, max_col = -1, max_mask = 0, max_prev_f = 0;
    f[0] = 1;
    max_f_idx = max(max_f_idx, 0LL);
    for (int i = 0; i < (1 << n); i++)
        for (int j = 0; j < n; j++) {
            if_checks++;
            if (i >> j & 1) {
                if_true++;
                max_mask = max(max_mask, (long long)(1 << j));
                long long prev = i ^ (1 << j);
                max_prev_f = max(max_prev_f, prev);
                max_f_idx = max(max_f_idx, (long long)i);
                max_row = max(max_row, (long long)__builtin_popcount(i) - 1);
                max_col = max(max_col, (long long)j);
                body++;
                (f[i] += 1ll * f[i ^ (1 << j)] *
                          a[__builtin_popcount(i) - 1][j] % mod) %= mod;
            }
        }
    fprintf(stderr, "f array size = %d (1<<N)\n", 1 << N);
    fprintf(stderr, "a array size = %d x %d\n", N, N);
    fprintf(stderr, "n = %d\n", n);
    fprintf(stderr, "max f index accessed   = %lld\n", max_f_idx);
    fprintf(stderr, "max (1<<j)             = %lld\n", max_mask);
    fprintf(stderr, "max f index (i^mask)   = %lld\n", max_prev_f);
    fprintf(stderr, "max a row index        = %lld\n", max_row);
    fprintf(stderr, "max a col index        = %lld\n", max_col);
    fprintf(stderr, "if_checks (line15 eval)= %lld\n", if_checks);
    fprintf(stderr, "if_true  (line15 true) = %lld\n", if_true);
    fprintf(stderr, "n*2^n                  = %lld\n", (long long)n * (1LL << n));
    fprintf(stderr, "n*2^(n-1)              = %lld\n", (long long)n * (1LL << (n - 1)));
    fprintf(stderr, "if_checks/(n*2^n)      = %.6f\n", (double)if_checks / ((double)n * (1LL << n)));
    fprintf(stderr, "if_true  /(n*2^n)      = %.6f\n", (double)if_true / ((double)n * (1LL << n)));
    fprintf(stderr, "if_true  /(n*2^(n-1))  = %.6f\n", (double)if_true / ((double)n * (1LL << (n - 1))));
    cout << f[(1 << n) - 1];
    return 0;
}
