/*
 * P9748 [CSP-J 2023] 小苹果 —— 骗分 / 暴力版
 * ---------------------------------------------------------------------------
 * 直接按题意模拟整列苹果：每天扫一遍剩下的苹果，把第 1、4、7… 个拿走，其余按序保留。
 *
 * 复杂度：每天处理的元素数依次为 n, 2n/3, 4n/9, …，总和 < 3n，即 O(n)。
 *         n <= 10^7 左右可以过；n = 10^9 时数组开不下、时间也爆。
 *
 * 考场用法：先交这一份，保证「小数据那几档」拿到手，并且它一定能跑、不会 RE；
 *           有时间再换 solution.cpp 的 O(log n) 正解。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n;
    if (!(cin >> n)) return 0;

    vector<long long> cur(n);
    for (long long i = 0; i < n; ++i) cur[i] = i + 1;   // 当前从左到右的苹果编号

    long long days = 0, dayOfN = -1;
    while (!cur.empty()) {
        ++days;
        vector<long long> nxt;
        nxt.reserve(cur.size());
        for (size_t i = 0; i < cur.size(); ++i) {
            if (i % 3 == 0) {                            // 第 1、4、7… 个被拿走
                if (cur[i] == n) dayOfN = days;
            } else {
                nxt.push_back(cur[i]);
            }
        }
        cur.swap(nxt);
    }

    cout << days << " " << dayOfN << "\n";
    return 0;
}
