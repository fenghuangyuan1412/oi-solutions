// r2_bf.cpp —— 第 26 题：穷举所有长度 2、字母取自 {a,b,c,d} 的有序对 (a,b)，共 16*16=256 组
// 统计 r2 程序输出为 3 的组合数，并顺便打印输出值分布 + 交换后是否对称
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
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
    const string L = "abcd";
    vector<string> all;
    for (char x : L) for (char y : L) all.push_back(string() + x + y);
    cout << "长度为2、字母取自abcd 的字符串共 " << all.size() << " 个，有序对 "
         << all.size() * all.size() << " 组\n\n";

    map<int, int> dist;
    int eq3 = 0, asym = 0;
    cout << "输出值分布：\n";
    // 打一张 16x16 的表
    cout << "a\\b     ";
    for (auto& s : all) cout << s << " ";
    cout << "\n";
    for (auto& x : all) {
        cout << x << "  ";
        for (auto& y : all) {
            int v = solve(x, y);
            dist[v]++;
            if (v == 3) eq3++;
            if (solve(x, y) != solve(y, x)) asym++;
            cout << v << "  ";
        }
        cout << "\n";
    }
    cout << "\n输出值分布: ";
    for (auto& kv : dist) cout << kv.first << "->" << kv.second << "  ";
    cout << "\n输出恰好为 3 的有序对个数 = " << eq3 << "\n";
    cout << "（行优先计数）不满足 f(a,b)==f(b,a) 的有序对个数 = " << asym << "\n";

    cout << "\n输出为 3 的具体列表：\n";
    int c = 0;
    for (auto& x : all) for (auto& y : all) if (solve(x, y) == 3) {
        cout << "(" << x << "," << y << ") ";
        if (++c % 8 == 0) cout << "\n";
    }
    cout << "\n合计 " << c << "\n";
    return 0;
}
