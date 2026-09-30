// r2_27.cpp -- 题 27：ans += dp[k][p] 恰好执行 8 次的最小 n
// 扫描 n=1..N，统计执行次数 cnt(n)（=Zeckendorf 表示的非零项数），找 cnt==8 的最小 n
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
ll exec_cnt = 0;
int cnt_exec(ll n) {          // solve3 第 42-43 行的 if 命中次数
    int k = 0; ll c = 0; int p = 0;
    while (len[k + 1] <= n) k++;
    for (; k >= 0; k--) if (len[k] <= n)
        c++, n -= len[k], p ^= len[k] & 1;
    return (int)c;
}
int main(int argc, char** argv) {
    ll N = argc > 1 ? atoll(argv[1]) : 2000000;
    init();
    vector<ll> good;
    map<int, ll> firstn;
    for (ll n = 1; n <= N; n++) {
        int c = cnt_exec(n);
        if (!firstn.count(c)) firstn[c] = n;
        if (c == 8 && good.size() < 25) good.push_back(n);
    }
    printf("N=%lld\n", N);
    printf("执行次数恰好为 k 的最小 n：\n");
    for (auto &kv : firstn) printf("  k=%2d -> min n=%lld\n", kv.first, kv.second);
    printf("cnt==8 的前 %d 个 n: ", (int)good.size());
    for (size_t i = 0; i < good.size(); i++) printf("%lld ", good[i]);
    printf("\n");
    printf("最小 n = %lld\n", good.empty() ? -1LL : good[0]);
    // 理论值：len[0]+len[2]+...+len[14]
    ll s = 0;
    for (int i = 0; i <= 14; i += 2) s += len[i];
    printf("理论值 sum len[0,2,4,...,14] = %lld\n", s);
    printf("len[0..16]: "); for (int i = 0; i <= 16; i++) printf("%lld ", len[i]); printf("\n");
    printf("四个选项的检查: n=986 cnt=%d, n=1596 cnt=%d, n=2583 cnt=%d, n=4180 cnt=%d\n",
           cnt_exec(986), cnt_exec(1596), cnt_exec(2583), cnt_exec(4180));
    printf("1596 的分解: ");
    {
        ll n = 1596; int k = 0;
        while (len[k + 1] <= n) k++;
        for (; k >= 0; k--) if (len[k] <= n) { printf("len[%d]=%lld ", k, len[k]); n -= len[k]; }
        printf("余 %lld\n", n);
    }
    return 0;
}
