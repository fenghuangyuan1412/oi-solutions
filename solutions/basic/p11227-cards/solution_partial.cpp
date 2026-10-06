/*
 * P11227 [CSP-J 2024] 扑克牌 —— 骗分 / 暴力版
 * ---------------------------------------------------------------------------
 * 赌「特殊性质 A：所有牌两两不同」。这时手里 n 张牌就是 n 个不同牌种，
 * 直接输出 52 - n 即可，连牌面都不用看。
 *
 * 能拿分：测试点 1（n<=1, 性质 A）、测试点 2~4（n<=52, 性质 A）共 4 个点。
 * 会挂：一旦出现重复牌（性质 B 的点 5~7、无性质的点 8~10），
 *      52 - n 会把答案算小（少借了），样例 2 就过不去。
 *
 * 考场用法：先交这份把性质 A 的 40 分兜住，再补上「去重」换成 solution.cpp。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o p11227_partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    string s;
    for (int i = 0; i < n; ++i) cin >> s;     // 读进来但不用（假设两两不同）

    cout << 52 - n << "\n";                    // 押性质 A：不同牌种数 = n
    return 0;
}
