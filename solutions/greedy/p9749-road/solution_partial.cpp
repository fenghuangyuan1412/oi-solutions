/*
 * P9749 [CSP-J 2023] 公路 —— 骗分 / 慢速版
 * ---------------------------------------------------------------------------
 * 和正解【同一套贪心逻辑】，只是把"找下一个更便宜的站"写成 O(n) 暴力、
 * 把"从 i 走到 nxt 的总距离"也每次重算，于是整体是 O(n^2)。
 *
 *   在站点 i：
 *     · 向后找第一个油价【严格更低】的站点 nxt（找不到就取终点 n）；
 *     · 算 i 到 nxt 的总距离 dist；
 *     · 油箱不够 dist 就补买 ceil((dist - rest)/v) 升，按 a[i] 结算；
 *     · 然后正常开过第 i 段。
 *
 * 复杂度 O(n^2)：n <= 2000 的档稳过，n = 10^5 会超时。
 * 考场用法：先交这一份拿住小数据；时间够再换 solution.cpp 的 O(n) 版。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, v;
    if (!(cin >> n >> v)) return 0;

    vector<long long> d(n + 1, 0), a(n + 1, 0);
    for (int i = 1; i < n; ++i) cin >> d[i];
    for (int i = 1; i <= n; ++i) cin >> a[i];

    long long ans = 0, rest = 0;      // rest：油箱里还能走多少公里

    for (int i = 1; i < n; ++i) {
        int nxt = n;                              // 默认开到终点
        for (int k = i + 1; k <= n; ++k)          // 向后找第一个更便宜的站
            if (a[k] < a[i]) { nxt = k; break; }

        long long dist = 0;                       // i 到 nxt 的距离
        for (int k = i; k < nxt; ++k) dist += d[k];

        if (rest < dist) {                        // 油不够，在这个站补买
            long long need = dist - rest;
            long long buy = (need + v - 1) / v;
            ans += buy * a[i];
            rest += buy * v;
        }
        rest -= d[i];                             // 开过第 i 段
    }

    cout << ans << "\n";
    return 0;
}
