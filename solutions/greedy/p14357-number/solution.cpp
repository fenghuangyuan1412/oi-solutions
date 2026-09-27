#include <bits/stdc++.h>
using namespace std;

int cnt[10];  // cnt[d] = 数字 d 出现的次数，全局数组自动零初始化

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;  // 按字符串读入：|s| 可达 1e6，绝不能转成整数

    // 第一遍扫描：只统计数字字符，非数字直接丢弃
    // 少了这个判断，'a'-'z' 会算出 49~75 的下标，写穿 cnt[10]
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            ++cnt[c - '0'];
        }
    }

    // 输出：从大到小依次打印，每个数字打印它出现的次数
    // 位数尽可能多 + 高位尽可能大 = 数值最大
    for (int d = 9; d >= 0; --d) {
        while (cnt[d] > 0) {  // 用 >0 判定；while(cnt[d]--) 会把计数减到 -1
            cout << d;
            --cnt[d];
        }
    }
    cout << '\n';

    return 0;
}
