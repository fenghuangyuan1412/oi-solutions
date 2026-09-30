// r2_dp.cpp —— 与 r2.cpp 完全相同的递推，额外把整张 dp 表打出来（讲课用）
// 用法: echo "aba bab" | ./r2_dp.exe
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    string a, b;
    cin >> a >> b;
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    vector<string> why(n + 1, string(m + 1, '?'));
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0) { dp[i][j] = j; why[i][j] = 'T'; }          // T: 顶行初始化 = j
            else if (j == 0) { dp[i][j] = i; why[i][j] = 'L'; }     // L: 左列初始化 = i
            else if (a[i - 1] == b[j - 1]) { dp[i][j] = dp[i - 1][j - 1] + 1; why[i][j] = '='; }
            else { dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + 1; why[i][j] = 'm'; }
        }
    }
    // 打印表头
    cout << "a = \"" << a << "\"  b = \"" << b << "\"  n=" << n << " m=" << m << "\n";
    cout << "        j:   ";
    for (int j = 0; j <= m; ++j) cout << (j == 0 ? ' ' : b[j - 1]) << "    ";
    cout << "\n";
    cout << "  i |      ";
    for (int j = 0; j <= m; ++j) cout << j % 10 << "    ";
    cout << "\n";
    for (int i = 0; i <= n; ++i) {
        cout << "    " << (i == 0 ? '-' : a[i - 1]) << " |  ";
        for (int j = 0; j <= m; ++j) cout << dp[i][j] << "    ";
        cout << "   (";
        for (int j = 0; j <= m; ++j) cout << why[i][j];
        cout << ")\n";
    }
    cout << "dp[" << n << "][" << m << "] = " << dp[n][m] << "   <== 程序输出\n";

    // 对照用：真正的 LCS 长度 与 真正的编辑距离，帮助学生分辨这不是那两个算法
    vector<vector<int>> lcs(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            lcs[i][j] = (a[i-1]==b[j-1]) ? lcs[i-1][j-1]+1 : max(lcs[i-1][j], lcs[i][j-1]);
    vector<vector<int>> ed(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; ++i) ed[i][0] = i;
    for (int j = 0; j <= m; ++j) ed[0][j] = j;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            ed[i][j] = (a[i-1]==b[j-1]) ? ed[i-1][j-1]
                     : min(ed[i-1][j-1], min(ed[i-1][j], ed[i][j-1])) + 1;
    cout << "（对照：LCS 长度 = " << lcs[n][m] << "，标准编辑距离 = " << ed[n][m] << "）\n";
    return 0;
}
