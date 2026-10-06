/*
 * P9752 [CSP-S 2023] 密码锁 —— 骗分 / 部分分版
 * ---------------------------------------------------------------------------
 * 考场上如果一时想不清"两个相邻拨圈"怎么判，可以先只认「仅转一个拨圈」这一种动作。
 * 题面的特殊性质 A 说的正是这种情形（保证所有正确密码都能靠"仅转一个拨圈"解释全部状态），
 * 所以这一版能稳稳拿下 6~8 号测试点；再补一个 n = 1 的特判，把 1~3 号测试点也搬回来。
 *
 * 为什么 n = 1 时答案恒为 81：
 *   对任意一个观测状态 S，能一步转到 S 的密码个数是固定的 ——
 *     转一个拨圈：5 个位置 x 9 种幅度 = 45
 *     转相邻两个拨圈：4 对 x 9 种幅度 = 36
 *   两类前像互不重叠（改 1 位 vs 改 2 位），合计 81。与官方样例输出一致。
 *
 * 这一版丢掉的就是"双拨圈"那 36 个候选，所以：
 *   · n = 1 靠特判输出 81（否则只会输出 45，1~3 号点全崩）；
 *   · n >= 2 且数据不满足特殊性质 A 时，答案会偏小（4~5、9~10 号点拿不到）。
 *
 * 复杂度同样是 O(10^5 x n x 5)。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

const int W = 5;
int st[9][W + 1];
int n;

// 只认「仅转一个拨圈」：恰好一个位置的差值非零
bool reachable_one_dial(const int p[W + 1], const int s[W + 1]) {
    int nz = 0;
    for (int i = 1; i <= W; ++i)
        if ((s[i] - p[i] + 10) % 10 != 0) ++nz;
    return nz == 1;                 // nz == 0 表示状态就是密码，非法
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    for (int k = 0; k < n; ++k)
        for (int i = 1; i <= W; ++i) cin >> st[k][i];

    if (n == 1) {                   // 兜底档：答案与状态无关，恒为 45 + 36 = 81
        cout << 81 << "\n";
        return 0;
    }

    int ans = 0;
    int p[W + 1];
    for (int x = 0; x < 100000; ++x) {
        int t = x;
        for (int i = W; i >= 1; --i) { p[i] = t % 10; t /= 10; }

        bool ok = true;
        for (int k = 0; k < n && ok; ++k)
            if (!reachable_one_dial(p, st[k])) ok = false;
        if (ok) ++ans;
    }

    cout << ans << "\n";
    return 0;
}
