/*
 * P11233 [CSP-S 2024] 染色 —— 骗分 / 暴力版
 *
 * 档 1（n <= 20）：直接枚举 2^n 种染色方案，逐个算分取最大。
 *                  对应测试点 1~4（n <= 15），本机 T=10 实测 21 ms。
 * 档 2（更大的 n）：动态规划。状态 = 「另一种颜色最后一个数的值」，
 *                  每一步老老实实遍历全部状态，O(n * V)，V = 数组里出现过的不同值的个数。
 *                  对应测试点 5~10、13~15（n <= 2000 或 A_i <= 10 时 V 很小，跑得动）；
 *                  测试点 11~12（n <= 2e4、值域大，每组 V 实测约 1.98e4）本机 6818 ms，超时限；
 *                  测试点 16~20 单组（n=2e5、V=181494）本机 70 s，T=10 时 60 s 还没输出一行。
 *
 * 编译（本仓库统一按 -static 编译，本机有两套 MinGW 路径冲突）：
 *   g++ -static -O2 -std=c++14 solution_partial.cpp -o E:/ai/suanfa_study/.raw/bin/p11233_partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 200000 + 5;
const int MAXV = 1000000;          // A_i <= 1e6
const ll NEG = -(1LL << 60);

/*  fread 快读：极限数据 2e6 个数，scanf 在本机要 0.87 s，快读只要 0.02 s（见 README 易错点） */
static char ibuf[1 << 22];
static size_t ipos = 0, ilen = 0;
inline int gc() {
    if (ipos == ilen) { ilen = fread(ibuf, 1, sizeof ibuf, stdin); ipos = 0; if (!ilen) return -1; }
    return ibuf[ipos++];
}
inline int rd() {
    int c = gc(), v = 0;
    while (c <= ' ' && c != -1) c = gc();
    for (; c > ' '; c = gc()) v = v * 10 + (c - '0');
    return v;
}

int n;
int A[MAXN];                       // A_i <= 1e6，用 int 存；只有"得分"才需要 long long

/* ---------- 档 1：n <= 20，枚举 2^n 种染色 ---------- */
ll bruteForce() {
    ll best = 0;
    for (int mask = 0; mask < (1 << n); ++mask) {
        ll lastR = -1, lastB = -1, s = 0;      // 上一个红 / 上一个蓝的"值"
        for (int i = 1; i <= n; ++i) {          // A 是从 1 开始编号的
            if ((mask >> (i - 1)) & 1) {        // 染红
                if (lastR == A[i]) s += A[i];  // 只看"最靠近的同色"
                lastR = A[i];
            } else {                            // 染蓝
                if (lastB == A[i]) s += A[i];
                lastB = A[i];
            }
        }
        if (s > best) best = s;
    }
    return best;
}

/* ---------- 档 2：通用 DP（老实版，O(n*V)） ----------
 * 染完前 i 个数之后，第 i 个数所在颜色的"最后一个值"就是 A[i]，
 * 于是整个状态只剩一个自由度：另一种颜色的最后一个值是多少（0 = 还没染过这种颜色）。
 *   f[v] = 染完前 i 个数、另一种颜色最后一个值为 v 时的最大得分
 * 转移到第 i 个数（x = A[i]，y = A[i-1]）：
 *   (1) 与第 i-1 个同色：所有状态的 v 都不变，得分统一 +add，add = (x==y ? x : 0)
 *   (2) 换一种颜色：落进状态 y，得分 = max over v of ( f[v] + (v==x ? x : 0) )
 * 正解的做法是把 (1) 的"全体加同一个数"记成一个偏移量，这里就老老实实遍历，慢在 O(V)。
 */
static ll f[MAXV + 1];             // f[v]：状态"另一种颜色最后值 = v"的最大得分
static int vis[MAXV + 1];          // 时间戳，避免每组清空 1e6 大小的数组
static int ver = 0;
static vector<int> keys;           // 当前活跃状态（0 + 已经出现过的值）

inline void addKey(int v) {
    if (vis[v] != ver) {
        vis[v] = ver;
        f[v] = (v == 0) ? 0 : NEG;  // i = 1 时只有"另一种颜色还没出现"这个状态，得分 0
        keys.push_back(v);
    }
}

ll dpSlow() {
    ++ver;
    keys.clear();
    addKey(0);
    for (int i = 2; i <= n; ++i) {
        int x = A[i], y = A[i - 1];
        addKey(x);
        addKey(y);
        ll add = (x == y) ? (ll)x : 0;
        // (2) 换色：必须用"加 add 之前"的 f 来算
        ll best2 = NEG;
        for (size_t k = 0; k < keys.size(); ++k) {
            int v = keys[k];
            if (f[v] <= NEG / 2) continue;
            ll cand = f[v] + ((v == x) ? (ll)x : 0);
            if (cand > best2) best2 = cand;
        }
        // (1) 同色：把每个状态都真的加上 add
        for (size_t k = 0; k < keys.size(); ++k) {
            int v = keys[k];
            if (f[v] > NEG / 2) f[v] += add;
        }
        if (f[y] < best2) f[y] = best2;
    }
    ll ans = 0;
    for (size_t k = 0; k < keys.size(); ++k)
        if (f[keys[k]] > ans) ans = f[keys[k]];
    return ans;
}

int main() {
    int T = rd();
    while (T--) {
        n = rd();
        for (int i = 1; i <= n; ++i) A[i] = rd();
        ll ans;
        if (n <= 20) ans = bruteForce();   // 小档：枚举就够
        else         ans = dpSlow();        // 大档：状态 DP，精确，但 O(n*V) 会被时限卡住
        printf("%lld\n", ans);
    }
    return 0;
}
