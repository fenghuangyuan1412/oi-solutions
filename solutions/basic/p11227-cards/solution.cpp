/*
 * P11227 [CSP-J 2024] 扑克牌 —— 正解
 * ---------------------------------------------------------------------------
 * 题意：给了 n 张牌（可能重复），每张牌是「花色(D/C/H/S) + 点数(A2..9TJQK)」
 *       共 4*13 = 52 种。问最少再借几张，能从手里的牌里选出完整的一副 52 张。
 *
 * 关键观察：借来的牌想借什么就有什么，所以【手里已经有的每种牌都只用留 1 张】。
 *       重复的牌帮不上忙（完整牌每种只要 1 张）。
 *   ⇒ 答案 = 52 − (手里出现的「不同牌种」的个数)。
 *
 * 做法：开一张 bool seen[4][13]，读到一张牌就把对应的格子标记为 true，
 *       最后数有多少个 true。花色 4 种、点数 13 种，直接映射即可。
 *
 * 数据范围：n <= 52，规模极小，一趟读入 O(n) 就够。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o p11227_full.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    string ranks = "A23456789TJQK";           // 点数表：下标 0..12
    int suitId[256];                           // 花色字符 -> 0..3
    suitId[(int)'D'] = 0; suitId[(int)'C'] = 1;
    suitId[(int)'H'] = 2; suitId[(int)'S'] = 3;

    bool seen[4][13] = {};                    // seen[花色][点数] 是否出现过

    for (int i = 0; i < n; ++i) {
        string s;                             // 长度为 2 的牌，如 "CA"
        cin >> s;
        int a = suitId[(int)s[0]];            // 花色
        int b = (int)ranks.find(s[1]);        // 点数下标
        seen[a][b] = true;                     // 只关心「有没有」，重复不影响
    }

    int cnt = 0;
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 13; ++b)
            if (seen[a][b]) ++cnt;            // 数不同牌种

    cout << 52 - cnt << "\n";
    return 0;
}
