// r2_probe.cpp -- 第 2 篇插桩版：打印 Fibonacci 词、value() 序列、solve1/solve2/solve3 对照表
// 用法: echo "30" | ./r2_probe.exe      （n_max，程序会枚举 n=1..n_max, p=0/1）
// 目的: 题 8（233 0 的三个输出）、题 9（三函数等价性）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll len[90], dp[90][2];
void init() {
    len[0] = 1, len[1] = 2, dp[1][1] = 1;
    for (int i = 2; i <= 86; i++) {
        len[i] = len[i - 1] + len[i - 2];
        int op = len[i - 1] & 1;
        dp[i][0] = dp[i - 1][0] + dp[i - 2][0 ^ op];
        dp[i][1] = dp[i - 1][1] + dp[i - 2][1 ^ op];
    }
}
int solve1(int n, int p) {
    string s = "0", nxt = "01", tmp;
    while (nxt.length() < n) tmp = nxt + s, s = nxt, nxt = tmp;
    int ans = 0;
    for (int i = p; i < n; i += 2) ans += nxt[i] - '0';
    return ans;
}
string word(int n) {              // solve1 里那个 nxt，截到 n 位
    string s = "0", nxt = "01", tmp;
    while ((int)nxt.length() < n) tmp = nxt + s, s = nxt, nxt = tmp;
    return nxt.substr(0, n);
}
int value(int pos) {
    if (pos <= 1) return pos;
    int k = 0;
    while (len[k + 1] <= pos) k++;
    return value(pos - len[k]);
}
int solve2(int n, int p) {
    int ans = 0;
    for (int i = p; i < n; i += 2) ans += value(i);
    return ans;
}
ll solve3(ll n, int p) {
    int k = 0; ll ans = 0;
    while (len[k + 1] <= n) k++;
    for (; k >= 0; k--) if (len[k] <= n)
        ans += dp[k][p], n -= len[k], p ^= len[k] & 1;
    return ans;
}
int main() {
    int nmax;
    cin >> nmax;
    init();
    printf("len[0..12]:");
    for (int i = 0; i <= 12; i++) printf(" %lld", len[i]);
    printf("\ndp[i][0/1] i=0..14:\n");
    for (int i = 0; i <= 14 && i <= nmax; i++)
        printf("  i=%2d len=%-5lld dp0=%-5lld dp1=%-5lld\n", i, len[i], dp[i][0], dp[i][1]);
    string W = word(nmax);
    printf("Fibonacci word prefix (n=%d):\n%s\n", nmax, W.c_str());
    printf("value(0..min(nmax,60)-1):\n");
    for (int i = 0; i < nmax && i <= 60; i++) printf("%d", value(i));
    printf("\n");
    printf("  n  word | s1(p=0) s2(p=0) s3(p=0) | s1(p=1) s2(p=1) s3(p=1) | eq\n");
    int bad = 0;
    for (int n = 1; n <= nmax; n++) {
        int a10 = solve1(n, 0), a20 = solve2(n, 0); ll a30 = solve3(n, 0);
        int a11 = solve1(n, 1), a21 = solve2(n, 1); ll a31 = solve3(n, 1);
        bool eq = (a10 == a20 && a20 == a30 && a11 == a21 && a21 == a31);
        if (!eq) bad++;
        if (n <= 60 || n % 100 == 0 || n == nmax || !eq)
            printf("%4d      | %5d %5d %5lld | %5d %5d %5lld | %s\n",
                   n, a10, a20, a30, a11, a21, a31, eq ? "OK" : "DIFF");
    }
    printf("nmax=%d mismatched n count=%d\n", nmax, bad);
    return 0;
}
