/*
 * P11231 [CSP-S 2024] 决斗 —— 骗分 / 慢速版
 * ---------------------------------------------------------------------------
 * 和正解【同一个最大匹配贪心】，只是把"找一只比它大且没被用过的怪兽"写成 O(n) 暴力，
 * 于是整体 O(n^2)。
 *
 *   排序后，对每个目标 j（从小到大），暴力扫一遍找第一个未使用、且 a[i] > a[j] 的攻击者 i。
 *
 * 复杂度 O(n^2)：n <= 2000 的档稳过，n = 10^5 会超时。
 * 考场用法：先交这一份拿住小数据；时间够再换 solution.cpp 的 O(n) 双指针版。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());

    vector<char> used(n, 0);
    int killed = 0;
    for (int j = 0; j < n; ++j) {              // j：被攻击的目标，从小到大
        for (int i = 0; i < n; ++i) {          // 暴力找攻击者
            if (i == j || used[i]) continue;
            if (a[i] > a[j]) {                 // 找到就用掉
                used[i] = 1;
                ++killed;
                break;
            }
        }
    }

    cout << n - killed << "\n";
    return 0;
}
