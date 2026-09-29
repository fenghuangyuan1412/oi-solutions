// fill1_tpl.cpp -- 完善程序(1) 线性筛求 i^i mod 998244353 的模板
// a = 1ull * a * a % mod; bsgs2[i][j] = 1ull * bsgs1[i][j - 1] * qpow(f[i], B) % mod; for(int ex = gap + 1; ex <= now; ex++) f[pr[j] * i] = 1ull * bsgs1[pr[j]][i % B] * bsgs2[pr[j]][i / B] % mod * cur % mod; !(i % pr[j]) 是五个空，由 fill1_run.py 用选项文本替换
#include <bits/stdc++.h>
using namespace std;
const int N = 1e7+5, SN = (int)sqrt(N) + 5, mod = 998244353;
int qpow(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = 1ull * res * a % mod;
        a = 1ull * a * a % mod;
        b >>= 1;
    }
    return res;
}
int bsgs1[SN][SN], bsgs2[SN][SN];
bool vis[N];
int f[N], pr[N / 10], len;
int powers[N], S;
int main() {
    int n;
    scanf("%d", &n);
    f[1] = 1;
    const int B = sqrt(n);
    for (int i = 2; i <= n; i++) {
        if (!vis[i]) {
            pr[++len] = i, f[i] = qpow(i, i);
            if (i <= B) {
                bsgs1[i][0] = 1;
                for (int j = 1; j <= B; j++) bsgs1[i][j] = 1ull * bsgs1[i][j - 1] * f[i] % mod;
                bsgs2[i][0] = 1;
                for (int j = 1; j <= B; j++)
                    bsgs2[i][j] = 1ull * bsgs1[i][j - 1] * qpow(f[i], B) % mod;
            }
        }
        powers[0] = 1;
        int cur = 1, gap = 0;
        for (int j = 1; j <= len && i * pr[j] <= n; j++) {
            vis[pr[j] * i] = 1;
            int now = pr[j] - pr[j - 1];
            for(int ex = gap + 1; ex <= now; ex++)
                powers[ex] = 1ull * powers[ex - 1] * f[i] % mod;
            gap = max(gap, now);
            cur = 1ull * cur * powers[now] % mod;
            f[pr[j] * i] = 1ull * bsgs1[pr[j]][i % B] * bsgs2[pr[j]][i / B] % mod * cur % mod;
            if (!(i % pr[j]))
                break;
        }
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", f[i]);
    return 0;
}
