// r2_25.cpp -- 题 25：统计 solve2 内部 value() 的递归调用次数 + 第 29 行 while 循环步数
// 用法: ./r2_25.exe            -> 自动跑 n = 2^m
//       ./r2_25.exe M          -> 只跑 n=2^M
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll len[90], dp[90][2];
ll g_calls = 0, g_loopsteps = 0, g_depth = 0;
int g_cur_depth = 0;
void init() {
    len[0] = 1, len[1] = 2, dp[1][1] = 1;
    for (int i = 2; i <= 86; i++) {
        len[i] = len[i - 1] + len[i - 2];
        int op = len[i - 1] & 1;
        dp[i][0] = dp[i - 1][0] + dp[i - 2][0 ^ op];
        dp[i][1] = dp[i - 1][1] + dp[i - 2][1 ^ op];
    }
}
int value(int pos) {
    g_calls++;
    int k = 0;
    if (pos <= 1) return pos;
    while (len[k + 1] <= pos) { k++; g_loopsteps++; }
    g_cur_depth++;
    g_depth = max(g_depth, (ll)g_cur_depth);
    int r = value(pos - len[k]);
    g_cur_depth--;
    return r;
}
int solve2(int n, int p) {
    int ans = 0;
    for (int i = p; i < n; i += 2) ans += value(i);
    return ans;
}
int main(int argc, char** argv) {
    init();
    int only = argc > 1 ? atoi(argv[1]) : -1;
    printf("%8s %14s %14s %10s %12s %12s %12s\n",
           "n", "value_calls", "while_steps", "maxdepth", "n*log2(n)", "calls/(n log n)", "while/(n log^2 n)");
    for (int m = 6; m <= (only > 0 ? only : 20); m++) {
        if (only > 0 && m != only) continue;
        int n = 1 << m;
        g_calls = g_loopsteps = g_depth = g_cur_depth = 0;
        auto t0 = chrono::steady_clock::now();
        int r = solve2(n, 0);
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        double lg = log2((double)n);
        printf("%8d %14lld %14lld %10lld %12.0f %12.4f %12.4f  %.1fms ans=%d\n",
               n, g_calls, g_loopsteps, g_depth, n * lg,
               g_calls / (n * lg), g_loopsteps / (n * lg * lg), ms, r);
    }
    printf("说明: maxdepth 记录 value() 递归的最大深度（=Zeckendorf 表示中非零项个数-1）\n");
    // 打印 value(pos) 与 Fibonacci 词的关系
    string s = "0", nxt = "01", t;
    while (nxt.length() < 60) t = nxt + s, s = nxt, nxt = t;
    printf("pos      :"); for (int i = 0; i < 40; i++) printf("%3d", i); printf("\n");
    printf("word[pos]:"); for (int i = 0; i < 40; i++) printf("%3d", nxt[i] - '0'); printf("\n");
    printf("value(pos):"); for (int i = 0; i < 40; i++) printf("%3d", value(i)); printf("\n");
    return 0;
}
