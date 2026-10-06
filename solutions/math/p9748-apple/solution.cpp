/*
 * P9748 [CSP-J 2023] 小苹果
 * ---------------------------------------------------------------------------
 * 题意：n 个苹果排成一列，编号 1..n。每天从最左边第 1 个开始、每隔 2 个拿走 1 个
 *       （即拿走当前第 1、4、7、… 个，位置按 1 起算），拿完把剩下的按原顺序重排。
 *       问：(1) 多少天拿完；(2) 编号为 n 的苹果是第几天被拿走的。
 *
 * 正解 O(log n)：不要真的模拟整列苹果，只盯住「数量」和「目标的位置」两个量。
 *
 *   ① 总天数：设当天还剩 cnt 个苹果，被拿走的是第 1、4、7… 个，共 ceil(cnt/3) 个，
 *      于是 cnt ← cnt - ceil(cnt/3)。每轮至少乘 2/3，O(log n) 轮后归零。
 *   ② 编号 n 的苹果：第 day 天它的位置是 pos。若 pos ≡ 1 (mod 3)，这一天它就被拿走；
 *      否则它前面被拿走了 ceil((pos-1)/3) = (pos+1)/3 个（整数除法），新位置 = pos - (pos+1)/3。
 *
 * 数据范围：1 <= n <= 10^9。天数、位置都不超过 n，int 够用，这里用 long long 更省心。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n;
    if (!(cin >> n)) return 0;

    /* ① 需要多少天拿完 */
    long long cnt = n, days = 0;
    while (cnt > 0) {
        cnt -= (cnt + 2) / 3;      // 拿走 ceil(cnt/3) 个
        ++days;
    }

    /* ② 编号为 n 的苹果第几天被拿走 */
    long long pos = n, day = 0;
    while (true) {
        ++day;
        if ((pos - 1) % 3 == 0) break;   // 它正好落在「第 1、4、7…」这些位置上
        pos -= (pos + 1) / 3;            // 否则前面被拿走 (pos+1)/3 个，位置前移
    }

    cout << days << " " << day << "\n";
    return 0;
}
