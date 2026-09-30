// perm_brute.cpp -- 独立暴力：枚举所有排列 p，求 sum prod a[i][p[i]] (permanent)，对拍 r1
// 用法: 输入 n 然后 n*n 个数（与 r1 相同格式）。输出暴力结果（mod 998244353）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll mod = 998244353;
int main() {
    int n;
    if (!(cin >> n)) return 0;
    vector<vector<ll>> a(n, vector<ll>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) cin >> a[i][j];
    vector<int> p(n);
    for (int i = 0; i < n; i++) p[i] = i;
    ll ans = 0;
    do {
        ll t = 1;
        for (int i = 0; i < n; i++) t = t * (a[i][p[i]] % mod) % mod;
        ans = (ans + t) % mod;
    } while (next_permutation(p.begin(), p.end()));
    cout << ans;
    return 0;
}
