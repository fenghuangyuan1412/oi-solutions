/*
 * P14361 [CSP-S 2025] 社团招新 —— 正解（满分档，O(n log n)）
 * ---------------------------------------------------------------------------
 * 题意：n（偶数）个人、3 个部门，第 i 个人对部门 j 的满意度 a[i][j]。
 *       要求「没有任何部门多于一半人」（即每个部门人数 <= n/2），求满意度总和最大值。
 *       t <= 5 组数据，n <= 10^5，a[i][j] <= 2*10^4。
 *
 * 三步走的贪心（板书就三行）：
 *   ① 先不管限制，让每个人都选自己最满意的部门，得到基准方案 B，总分 tot = Σ max_j a[i][j]。
 *   ② 超过 n/2 人的部门「至多一个」：若有两个部门都 > n/2，人数和就 > n 了，矛盾。
 *      设它是部门 b，超出 r = cnt[b] - n/2 个人，必须改派 r 个人去别的部门。
 *   ③ 改派第 i 个人的代价 loss[i] = a[i][b] - max(a[i][其他两个部门]) >= 0，
 *      取损失最小的 r 个人改派即可 —— 代价最小的 r 个 loss 之和就是必须付出的损失。
 *
 * 为什么第 ③ 步不用管"接收部门会不会又挤爆"：
 *   另外两个部门在 B 里一共只有 n - cnt[b] = n/2 - r 人，
 *   所以任意一个接收部门的空闲名额 n/2 - cnt[j] >= n/2 - (n/2 - r) = r，
 *   就算把 r 个人全塞进同一个部门也放得下。限制在这一步自动满足。
 *
 * 为什么这样就是最优：见 README「正确性」。核心是下界——
 *   任何合法方案里，部门 b 最多 n/2 人 ⇒ 至少有 r 个 B 中的人必须离开 b，
 *   每人至少付出自己的 loss；其余人的损失 >= 0。所以任何方案损失 >= 最小的 r 个 loss 之和，
 *   而本算法恰好达到这个下界。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 *      （必须 -static，本机两套 MinGW 冲突，否则 exit 139）
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

        vector<int> cnt(3, 0);            // 基准方案 B 里三个部门的人数
        vector<vector<int>> loss(3);      // loss[j]：B 中被分到部门 j 的人，改派的代价

        long long tot = 0;                // 基准总分 Σ max_j a[i][j]
        for (int i = 0; i < n; ++i) {
            int a[3];
            cin >> a[0] >> a[1] >> a[2];

            int b = 0;                    // 最满意的部门（平局取编号最小的，取哪个都不影响答案）
            if (a[1] > a[b]) b = 1;
            if (a[2] > a[b]) b = 2;

            int second = -1;              // 去掉 b 之后，剩下两个部门里的较大满意度
            for (int j = 0; j < 3; ++j) if (j != b) second = max(second, a[j]);

            ++cnt[b];
            tot += a[b];
            loss[b].push_back(a[b] - second);   // >= 0
        }

        int heavy = -1;                   // 唯一可能超载的部门
        for (int j = 0; j < 3; ++j) if (cnt[j] > half) heavy = j;

        if (heavy != -1) {
            int r = cnt[heavy] - half;    // 至少要改派 r 个人
            vector<int>& L = loss[heavy];
            sort(L.begin(), L.end());     // 取最小的 r 个代价
            for (int k = 0; k < r; ++k) tot -= L[k];
        }

        cout << tot << "\n";
    }
    return 0;
}
