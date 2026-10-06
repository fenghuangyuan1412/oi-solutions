/*
 * P9749 [CSP-J 2023] 公路
 * ---------------------------------------------------------------------------
 * 题意：公路上 n 个站点，站点 i 到 i+1 相距 d[i] 公里；站点 i 每升油 a[i] 元，
 *       只能买整数升。车从站点 1 出发（油箱空），每升油能走 v 公里，油箱无限大。
 *       问从站点 1 开到站点 n 最少花多少钱。
 *
 * 正解 O(n)：只在"需要油"的时候买，且永远按「到目前为止见过的最低价」买。
 *
 *   关键观察：油箱无限大 ⇒ 可以在最便宜的那个站一次性预买未来的油。
 *   于是维护两个量：
 *     rest  —— 油箱里还能走多少公里
 *     minp  —— 从起点到当前站出现过的最低油价
 *   走到第 i 段时：
 *     · 若 rest >= d[i]，直接开过去，一分钱不花；
 *     · 否则缺 need = d[i] - rest 公里，至少要买 ceil(need / v) 升（整数升），
 *       按 minp 结算 —— 等价于"这些油当初是在最便宜的那个站买的"。
 *
 *   为什么这样就最优：任何方案里，第 i 段烧掉的油都可以"平移"到它之前的最便宜站点去买，
 *   价钱只降不升；把每段都做这种平移，就得到本算法，故它不劣于任何方案。
 *
 * 数据范围：n <= 10^5，d[i] <= 10^5，a[i] <= 10^5，v <= 10^5。
 *   总花费可达 10^5 * 10^5 * 10^5 / 1 = 10^15 量级 ⇒ 必须 long long。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, v;
    if (!(cin >> n >> v)) return 0;

    vector<long long> d(n + 1, 0);      // d[i]：站点 i → i+1 的距离（i = 1..n-1）
    for (int i = 1; i < n; ++i) cin >> d[i];
    vector<long long> a(n + 1, 0);      // a[i]：站点 i 的油价（i = 1..n）
    for (int i = 1; i <= n; ++i) cin >> a[i];

    long long ans = 0;                  // 总花费
    long long rest = 0;                 // 油箱里还能走多少公里
    long long minp = LLONG_MAX;         // 到目前为止的最低价

    for (int i = 1; i < n; ++i) {
        minp = min(minp, a[i]);         // 记住历史最低价

        if (rest >= d[i]) {             // 油够，直接开过去
            rest -= d[i];
            continue;
        }
        long long need = d[i] - rest;             // 还缺多少公里
        long long buy = (need + v - 1) / v;       // 至少要买几升（向上取整）
        ans += buy * minp;                        // 按历史最低价结算
        rest += buy * v - d[i];                   // 买来的油扣掉这一段消耗
    }

    cout << ans << "\n";
    return 0;
}
