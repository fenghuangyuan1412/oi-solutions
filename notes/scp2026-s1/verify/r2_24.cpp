// r2_24.cpp -- 题 24：|solve3(n,0)-solve3(n,1)| 能否超过 1
// 扫描 n=1..N，输出差值分布、最大差值、首次达到各差值的 n
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
ll solve3(ll n, int p) {
    int k = 0; ll ans = 0;
    while (len[k + 1] <= n) k++;
    for (; k >= 0; k--) if (len[k] <= n)
        ans += dp[k][p], n -= len[k], p ^= len[k] & 1;
    return ans;
}
// 独立暴力：直接构造 Fibonacci 词数前缀（验证 solve3 的语义）
ll brute(ll n, int p) {
    string s = "0", nxt = "01", t;
    while ((ll)nxt.length() < n) t = nxt + s, s = nxt, nxt = t;
    ll a = 0;
    for (ll i = p; i < n; i += 2) a += nxt[i] - '0';
    return a;
}
int main(int argc, char** argv) {
    ll N = argc > 1 ? atoll(argv[1]) : 100000;
    init();
    map<ll, ll> hist;
    ll best = -1, bestn = 0;
    map<ll, ll> first_at;      // 差值 -> 最小的 n
    vector<pair<ll, ll>> top;  // (diff, n)
    int chk = 0;
    for (ll n = 1; n <= N; n++) {
        ll a = solve3(n, 0), b = solve3(n, 1);
        ll d = llabs(a - b);
        hist[d]++;
        if (!first_at.count(d)) first_at[d] = n;
        if (d > best) { best = d; bestn = n; }
        top.push_back({d, n});
        if (n <= 2000 && n % 1 == 0) {           // 小规模交叉验证语义
            chk++;
            if (a != brute(n, 0) || b != brute(n, 1)) { printf("SEMANTIC MISMATCH at n=%lld\n", n); break; }
        }
    }
    printf("N = %lld, 交叉验证(brute Fibonacci word) 个数 = %d 全部一致\n", N, chk);
    printf("差值分布 diff -> 个数:\n");
    for (auto &kv : hist) printf("  |diff|=%3lld : %8lld 个   最小 n=%lld\n", kv.first, kv.second, first_at[kv.first]);
    printf("最大差值 = %lld (首次 n=%lld)\n", best, bestn);
    partial_sort(top.begin(), top.begin() + min((size_t)12, top.size()), top.end(),
                 [](const pair<ll,ll>& x, const pair<ll,ll>& y) { return x.first > y.first; });
    printf("差值最大的若干个 n: ");
    for (size_t i = 0; i < min((size_t)12, top.size()); i++) printf("(%lld, n=%lld) ", top[i].first, top[i].second);
    printf("\n");
    // 前 40 个差值
    printf("n=1..40 的 solve3(n,0), solve3(n,1), diff:\n");
    for (ll n = 1; n <= 40; n++) printf("  n=%2lld s3_0=%lld s3_1=%lld diff=%lld\n", n, solve3(n,0), solve3(n,1), llabs(solve3(n,0)-solve3(n,1)));
    return 0;
}
