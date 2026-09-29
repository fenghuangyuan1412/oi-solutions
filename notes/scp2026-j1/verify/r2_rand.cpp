// r2_rand.cpp —— 第 21/22/23 题：随机 + 穷举找反例
//   21: 交换 a, b 输出是否一定不变
//   22: 输出是否一定 <= n + m
//   23: 输出是否一定 >  min(n, m)
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
using namespace std;

int solve(string a, string b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= m; ++j) {
            if (i == 0) dp[i][j] = j;
            else if (j == 0) dp[i][j] = i;
            else if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1;
        }
    return dp[n][m];
}

int main() {
    mt19937 rng(20260809);
    uniform_int_distribution<int> len(1, 6), ch(0, 3);
    const string ABCD = "abcd";
    int T = 20000;
    int bad21 = 0, bad22 = 0, bad23 = 0, lessMin = 0, eqMin = 0;
    vector<string> ex21, ex22, ex23, exLess;
    for (int t = 0; t < T; ++t) {
        int n = len(rng), m = len(rng);
        string a, b;
        for (int i = 0; i < n; ++i) a += ABCD[ch(rng)];
        for (int j = 0; j < m; ++j) b += ABCD[ch(rng)];
        int v = solve(a, b), vs = solve(b, a);
        if (v != vs) { if (++bad21 <= 5) ex21.push_back(a + " / " + b + " -> " + to_string(v) + " vs " + to_string(vs)); }
        if (v > n + m) { if (++bad22 <= 5) ex22.push_back(a + " / " + b); }
        if (!(v > min(n, m))) {
            ++bad23;
            if (v == min(n, m)) ++eqMin; else ++lessMin;
            if (ex23.size() < 8) ex23.push_back("a=" + a + " b=" + b + " n=" + to_string(n) +
                                                " m=" + to_string(m) + " out=" + to_string(v) +
                                                " min=" + to_string(min(n, m)));
        }
    }
    cout << "随机 " << T << " 组 (长度1..6, 字母a~d)\n";
    cout << "  21 交换后不同 : " << bad21 << (bad21 ? "  => F" : "  => 无反例") << "\n";
    for (auto& s : ex21) cout << "      " << s << "\n";
    cout << "  22 输出 > n+m : " << bad22 << (bad22 ? "  => F" : "  => 无反例(命题成立 T)") << "\n";
    for (auto& s : ex22) cout << "      " << s << "\n";
    cout << "  23 输出 <= min(n,m) 的反例个数 : " << bad23
         << "  (其中 输出==min: " << eqMin << ", 输出<min: " << lessMin << ")\n";
    for (auto& s : ex23) cout << "      " << s << "\n";

    // 穷举：所有长度 1..4、字母取自 {a,b} 的有序对
    cout << "\n穷举 长度1..4、字母取自{a,b} 的全部有序对：\n";
    vector<string> all;
    for (int mask = 0; mask < (1 << 4); ++mask)
        for (int L = 1; L <= 4; ++L) {
            string s;
            for (int i = 0; i < L; ++i) s += char('a' + ((mask >> i) & 1));
            all.push_back(s);
        }
    // 去重
    set<string> u(all.begin(), all.end());
    all.assign(u.begin(), u.end());
    cout << "  字符串个数 = " << all.size() << ", 有序对 = " << all.size() * all.size() << "\n";
    int g21 = 0, g22 = 0, g23 = 0, gless = 0;
    vector<string> gex23;
    long long maxOut = 0;
    for (auto& x : all) for (auto& y : all) {
        int v = solve(x, y);
        maxOut = max(maxOut, (long long)v);
        if (v != solve(y, x)) ++g21;
        if (v > (int)(x.size() + y.size())) ++g22;
        if (!(v > (int)min(x.size(), y.size()))) { ++g23;
            if (v < (int)min(x.size(), y.size())) ++gless;
            if (gex23.size() < 10) gex23.push_back("a=" + x + " b=" + y + " out=" + to_string(v) + " min=" + to_string(min(x.size(), y.size()))); }
    }
    cout << "  交换不同 = " << g21 << " ;  输出>n+m = " << g22 << " ;  输出<=min = " << g23
         << " (其中严格小于 min 的 = " << gless << ") ; 最大输出 = " << maxOut << "\n";
    for (auto& s : gex23) cout << "      " << s << "\n";
    return 0;
}
