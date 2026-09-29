// r3_more.cpp —— 第 27 / 29B 题的穷举证据
//   A) 把所有长度 1..12 的 01 串都跑一遍：
//        - 原版(1e9) 与 改版(1234567) 的输出是否逐个相同      -> 第 27 题
//        - 有没有"不可达"(输出 = INF) 的串
//        - 出现过的最大输出值 (证明 int 不会自然溢出)          -> 第 29 题 B
//   B) 输出恰好等于 min(步数) 与"翻转序列"是否唯一（九连环结构）
// 编译： g++ -static -O2 -std=c++14 -Wl,--stack,268435456 r3_more.cpp -o r3_more.exe
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;
int n;
string s;
long long calls, maxans, unreachable_cnt, calls_total, maxcalls, maxdeep_now, maxdeep_all;
const int INFBIG = 1000000000, INFSMALL = 1234567;
int INF;

int dfs(string t, int lst, int deep) {
    calls++;
    if (deep > maxdeep_now) maxdeep_now = deep;
    int ret = 0;
    for (int i = 0; i <= n; i++) if (t[i] != '1') ret = INF;
    if (ret == 0) return 0;
    for (int i = 0; i <= n; i++)
        if (i != lst && s.substr(n - i, i) == t.substr(0, i)) {
            string cur = t;
            cur[i] ^= 1;
            ret = min(ret, dfs(cur, i, deep + 1) + 1);
        }
    return ret;
}

int main() {
    int diff = 0, unreach = 0, unreach2 = 0, tot = 0;
    long long mx1 = 0, mx2 = 0, maxcalls_all = 0, maxdeep_overall = 0;
    for (int len = 1; len <= 12; len++) {
        for (int mask = 0; mask < (1 << len); mask++) {
            s.clear();
            for (int i = 0; i < len; i++) s += char('0' + ((mask >> (len - 1 - i)) & 1));
            n = len;
            int a1, a2;
            INF = INFBIG;   calls = 0; maxdeep_now = 0;
            a1 = dfs(string(n + 1, '0'), -1, 0);
            maxcalls_all = max(maxcalls_all, calls);
            maxdeep_overall = max(maxdeep_overall, maxdeep_now);
            INF = INFSMALL; calls = 0; maxdeep_now = 0;
            a2 = dfs(string(n + 1, '0'), -1, 0);
            tot++;
            if (a1 != a2) { if (diff < 10) printf("  差异: s=%s  1e9版=%d  1234567版=%d\n", s.c_str(), a1, a2); diff++; }
            if (a1 == INFBIG) unreach++;
            if (a2 == INFSMALL) unreach2++;
            mx1 = max(mx1, (long long)(a1 == INFBIG ? 0 : a1));
            mx2 = max(mx2, (long long)(a2 == INFSMALL ? 0 : a2));
        }
        printf("len=%2d 完成, 已测 %d 个串, 1e9/1234567 输出不同的串 = %d, 不可达 = %d\n", len, tot, diff, unreach);
        fflush(stdout);
    }
    printf("\n总计 %d 个串\n", tot);
    printf("  两版输出不同的串个数 = %d   (0 => 第27题 T)\n", diff);
    printf("  不可达(输出=INF)的串个数: 1e9版 = %d, 1234567版 = %d\n", unreach, unreach2);
    printf("  可达答案的最大值 = %lld  (远小于 2^31-1 => 不会 int 自然溢出)\n", mx1);
    printf("  dfs 单次程序运行的最大调用次数 = %lld, 最大递归深度 = %lld\n", maxcalls_all, maxdeep_overall);
    printf("  2^13 = %d, 2^19 = %d (n<=18 时状态数上限)\n", (1 << 13), (1 << 19));
    return 0;
}
