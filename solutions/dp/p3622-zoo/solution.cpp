// P3622 [APIO2007] 动物园 —— 环形 + 5 位窗口 状压 DP
// 编译：g++ -static -O2 -std=c++14 solution.cpp -o sol.exe
//
// 题意：N 个围栏围成一圈，C 个小朋友。每个小朋友站定后看到连续 5 个围栏
//       （起点 E，超过 N 从 1 绕回）。移走一些围栏里的动物后，小朋友开心当且仅当
//       「至少一个他害怕的动物被移走」或「至少一个他喜欢的动物没被移走」。
//       求移走集合选得最优时，最多多少个小朋友开心。
//
// 关键：每个小朋友只关心自己窗口里那 5 个围栏的状态。
//       从左往右扫一圈，"当前还没定论的围栏"永远只有 4 个（窗口滑动的重叠部分），
//       所以轮廓线状态就是 5 位二进制 mask：接下来 5 个围栏里哪些被移走。
//       环的处理：枚举开头 5 个围栏的 32 种模式 s，DP 一圈后要求结尾绕回同一个 s。

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 10005;

int n, c;
// cnt[s][mask]：窗口起点 s（围栏 s..s+4）、移走模式为 mask 时，
// 站在 s 的小朋友里有多少人开心。0..n-1 下标，起点按 mod n 归一。
int cnt[MAXN][32];

int main() {
    scanf("%d%d", &n, &c);
    for (int i = 0; i < c; i++) {
        int E, F, L;
        scanf("%d%d%d", &E, &F, &L);
        int s0 = (E - 1) % n;                       // 窗口起点（0 起）
        int fear = 0, like = 0;                     // 窗口内的相对位：bit k = 围栏 (s0+k)%n
        for (int j = 0; j < F; j++) {
            int x; scanf("%d", &x);
            fear |= 1 << ((x - 1 - s0 + n) % n);
        }
        for (int j = 0; j < L; j++) {
            int y; scanf("%d", &y);
            like |= 1 << ((y - 1 - s0 + n) % n);
        }
        // 一个小朋友的开心条件与"移哪些"有关，与"移几只"无关：
        // 对 32 种模式各判一次，把人数累进 cnt[s0][mask]
        for (int mask = 0; mask < 32; mask++) {
            bool fearGone = fear & mask;            // 有怕的 → 至少一个被移走（位与非零）
            bool likeLeft = (~mask) & like;         // 有喜欢的留在 bit 上
            if (fearGone || likeLeft) cnt[s0][mask]++;
        }
    }

    // dp[t][mask]：窗口 0..t-1 的小朋已全部结算，mask 是窗口 t（围栏 t..t+4）的移走模式。
    // 转移：窗口 t+1 的低 4 位继承 mask 的高 4 位，最高位是新围栏 t+5 的状态 b。
    // 滚动数组两行轮换。dp 值 = 前 t 个窗口开心的小朋友总数（负无穷 = 该模式不可达）。
    static int dp[2][32];
    const int NEG = INT_MIN / 2;
    int ans = 0;
    for (int s = 0; s < 32; s++) {                  // 枚举开头窗口（围栏 0..4）的模式
        int cur = 0;
        for (int m = 0; m < 32; m++) dp[cur][m] = NEG;
        dp[cur][s] = 0;                             // 窗口 0 还没结算：n 步转移会把窗口 1..n（n ≡ 0）各恰好算一次
        for (int t = 0; t < n; t++) {
            int nxt = cur ^ 1, tn = (t + 1) % n;
            for (int m = 0; m < 32; m++) dp[nxt][m] = NEG;
            for (int m = 0; m < 32; m++) {
                if (dp[cur][m] == NEG) continue;
                for (int b = 0; b < 2; b++) {       // 围栏 t+5：移(1) / 不移(0)
                    int nm = (m >> 1) | (b << 4);
                    int v = dp[cur][m] + cnt[tn][nm];
                    if (v > dp[nxt][nm]) dp[nxt][nm] = v;
                }
            }
            cur = nxt;
        }
        // 转完 n 步：cur 里的 mask 恰是"窗口 n"，它绕回就是"窗口 0"，
        // 而 dp 值已把窗口 1..n（n ≡ 0，即全部 n 个窗口）各结算一次。
        // 只有绕回 s（与开头一致）的模式才是真实存在的一圈方案。
        if (dp[cur][s] > ans) ans = dp[cur][s];
    }
    printf("%d\n", ans);
    return 0;
}
