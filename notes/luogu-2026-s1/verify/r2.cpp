// r2.cpp -- 洛谷 SCP-S1 2026 阅读程序(2)，严格按原文还原（P5-P7, 行号 1..56）
#include <bits/stdc++.h>
using namespace std;
// 3
typedef long long ll;                       // 4
ll len[90], dp[90][2];                       // 5
// 6
void init() {                                // 7
    len[0] = 1, len[1] = 2, dp[1][1] = 1;    // 8
    for (int i = 2; i <= 86; i++) {          // 9
        len[i] = len[i - 1] + len[i - 2];    // 10
        int op = len[i - 1] & 1;             // 11
        dp[i][0] = dp[i - 1][0] + dp[i - 2][0 ^ op];   // 12
        dp[i][1] = dp[i - 1][1] + dp[i - 2][1 ^ op];   // 13
    }                                         // 14
}                                             // 15
// 16
int solve1(int n, int p) {                    // 17
    string s = "0", nxt = "01", tmp;          // 18
    while (nxt.length() < n)                  // 19
        tmp = nxt + s, s = nxt, nxt = tmp;    // 20
    int ans = 0;                              // 21
    for (int i = p; i < n; i += 2) ans += nxt[i] - '0';  // 22
    return ans;                               // 23
}                                             // 24
// 25
int value(int pos) {                          // 26
    if (pos <= 1) return pos;                 // 27
    int k = 0;                                // 28
    while (len[k + 1] <= pos) k++;            // 29
    return value(pos - len[k]);               // 30
}                                             // 31
// 32
int solve2(int n, int p) {                    // 33
    int ans = 0;                              // 34
    for (int i = p; i < n; i += 2) ans += value(i);      // 35
    return ans;                               // 36
}                                             // 37
// 38
ll solve3(ll n, int p) {                      // 39
    int k = 0; ll ans = 0;                    // 40
    while (len[k + 1] <= n) k++;              // 41
    for (; k >= 0; k--) if (len[k] <= n)      // 42
        ans += dp[k][p], n -= len[k], p ^= len[k] & 1;   // 43
    return ans;                               // 44
}                                             // 45
// 46
int main() {                                  // 47
    ll n; int p;                              // 48
    cin >> n >> p, init();                    // 49
    if (n <= 1e6)                             // 50
        cout << solve1(n, p) << ' ' << solve2(n, p) << ' '    // 51
             << solve3(n, p) << endl;         // 52
    else                                      // 53
        cout << solve3(n, p) << endl;         // 54
    return 0;                                 // 55
}
