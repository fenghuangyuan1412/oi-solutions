#include <bits/stdc++.h>
using namespace std;

// P1044 栈：1..n 依次进栈，问有多少种出栈序列（卡特兰数）
// 分治推导：考虑"第一个进栈的数 1"什么时候出栈。
//   若 1 是第 k+1 个出栈的，那么它出栈前，2..k+1 这 k 个数必须已经进过栈并且都排在 1 上面，
//   这些数的出栈顺序彼此独立，方案数 f[k]；1 出栈之后还剩 n-1-k 个数没进栈，方案数 f[n-1-k]。
//   两类相乘再对 k 求和：f[n] = sum_{k=0}^{n-1} f[k] * f[n-1-k]，f[0] = 1（空序列算一种）。
long long f[25];

int main() {
    int n;
    scanf("%d", &n);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        f[i] = 0;
        for (int k = 0; k < i; k++) f[i] += f[k] * f[i - 1 - k];
    }
    printf("%lld\n", f[n]);
    return 0;
}
