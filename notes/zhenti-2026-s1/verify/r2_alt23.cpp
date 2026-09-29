// r2_alt23.cpp -- 第 2 篇，第 11 行改为 int op = i % 3 < 2;（题 23 实测用）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll len[90], dp[90][2];
void init() {
    len[0] = 1, len[1] = 2, dp[1][1] = 1;
    for (int i = 2; i <= 86; i++) {
        len[i] = len[i - 1] + len[i - 2];
        int op = i % 3 < 2;                 // <== 改动处
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
    ll n; int p;
    cin >> n >> p, init();
    if (n <= 1e6)
        cout << solve1(n, p) << ' ' << solve2(n, p) << ' ' << solve3(n, p) << endl;
    else
        cout << solve3(n, p) << endl;
    return 0;
}
