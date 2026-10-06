/*
 * P14361 [CSP-S 2025] 社团招新 —— 骗分 / 部分分版（二维计数 DP）
 * ---------------------------------------------------------------------------
 * 学生最容易想到的"正规"做法：把"每个部门各分了几个人"当状态。
 *   dp[c1][c2] = 已经分配了若干人、部门 1 有 c1 人、部门 2 有 c2 人时的最大满意度；
 *   部门 3 的人数不用存，它等于「已分配人数 - c1 - c2」（这是本题状态压缩的关键一步）。
 * 转移：第 i 个人去部门 1 / 2 / 3，各自要求对应部门人数不超过 n/2。
 *
 * 复杂度：O(n * (n/2+1)^2)。
 *   n = 30  ->  30 * 16 * 16        ~ 7.7 * 10^3      秒内轻松
 *   n = 200 ->  200 * 101 * 101     ~ 2.0 * 10^6      轻松（覆盖测试点 1~11）
 *   n = 10^5->  10^5 * 5*10^4 * 5*10^4 ~ 2.5 * 10^14  状态表还要 10 GB，直接没救
 * 所以这份代码是一份"有明确天花板"的骗分：小数据档全对，大数据档必须换正解的贪心。
 *
 * 更小学的数据档（测试点 1~4，n <= 10）连 DP 都不用写，直接 DFS 枚举 3^n 就行，
 * README 的"骗分"一节里有那份代码。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        const int half = n / 2;
        const int NEG = -1;                       // -1 表示该状态不可达（满意度非负，可以当标记）

        vector<vector<int>> dp(half + 1, vector<int>(half + 1, NEG));
        dp[0][0] = 0;

        for (int i = 1; i <= n; ++i) {
            int a[3];
            cin >> a[0] >> a[1] >> a[2];
            vector<vector<int>> ndp(half + 1, vector<int>(half + 1, NEG));

            for (int c1 = 0; c1 <= half; ++c1) {
                for (int c2 = 0; c2 <= half; ++c2) {
                    if (dp[c1][c2] == NEG) continue;
                    int c3 = (i - 1) - c1 - c2;   // 已分配 i-1 人时部门 3 的人数
                    if (c3 < 0 || c3 > half) continue;   // 旧状态本身就不合法

                    if (c1 < half)                            // 第 i 个人去部门 1
                        ndp[c1 + 1][c2] = max(ndp[c1 + 1][c2], dp[c1][c2] + a[0]);
                    if (c2 < half)                            // 去部门 2
                        ndp[c1][c2 + 1] = max(ndp[c1][c2 + 1], dp[c1][c2] + a[1]);
                    if (c3 < half)                            // 去部门 3（c1,c2 不变，c3 隐含增加）
                        ndp[c1][c2] = max(ndp[c1][c2], dp[c1][c2] + a[2]);
                }
            }
            dp.swap(ndp);
        }

        int ans = 0;
        for (int c1 = 0; c1 <= half; ++c1)
            for (int c2 = 0; c2 <= half; ++c2)
                if (dp[c1][c2] != NEG) ans = max(ans, dp[c1][c2]);   // 终态 c3 = n-c1-c2 已被转移保证 <= half
        cout << ans << "\n";
    }
    return 0;
}
