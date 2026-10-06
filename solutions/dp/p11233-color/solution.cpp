/*
 * P11233 [CSP-S 2024] 染色 —— 正解 O(n)
 *
 * 三步观察（完整推导见同目录 README.md 第二节）：
 *  1) 第 i 个数能不能得分，只取决于"同色位置上离它最近的那个数"，
 *     也就是**这种颜色最后一次出现的数是多少**。所以只需记两种颜色各自的"最后值"。
 *  2) 染完第 i 个数之后，其中一个颜色的最后值必然是 A[i]，
 *     状态只剩一个自由度：另一种颜色的最后值 v（v = 0 表示这种颜色还没用过）。
 *  3) "和第 i 个数同色"这条转移会把**所有状态同时加上同一个数** ⇒ 不真加，记一个整体偏移量；
 *     "换色"这条转移只往 v = A[i-1] 这一个格子里写，而且它要的是全部状态的最大值 ⇒ 维护 baseG。
 *     于是每一步 O(1)，总复杂度 O(n)。
 *
 * 编译（本仓库统一按 -static 编译，本机有两套 MinGW 路径冲突）：
 *   g++ -static -O2 -std=c++14 solution.cpp -o E:/ai/suanfa_study/.raw/bin/p11233_color.exe
 */
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 200000 + 5;
const int MAXV = 1000000;          // A_i <= 1e6
const ll NEG = -(1LL << 60);

/* fread 快读：极限数据一共 200 万个数，scanf 版本机 907 ms，快读版 64 ms（README 易错点 7） */
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
int A[MAXN];                       // 数组本身只用 int 存；爆的是"得分"

static ll base_[MAXV + 1];         // base_[v] = dp[v] - shift，v = 另一种颜色的最后值
static int seen[MAXV + 1];         // 时间戳：省掉每组清空 1e6 个元素的开销
static int clk = 0;

inline ll getb(int v) { return seen[v] == clk ? base_[v] : NEG; }
inline void setb(int v, ll x) {
    if (seen[v] != clk) { seen[v] = clk; base_[v] = x; }
    else if (x > base_[v]) base_[v] = x;      // base 只会变大，全局最大值才能增量维护
}

int main() {
    int T = rd();
    while (T--) {
        n = rd();
        for (int i = 1; i <= n; ++i) A[i] = rd();

        ++clk;
        setb(0, 0);                             // i = 1：另一种颜色还没用过，得分 0
        ll baseG = 0;                           // max over v of base_[v]
        ll shift = 0;                           // 所有状态共同背着的那段加分

        for (int i = 2; i <= n; ++i) {
            int x = A[i], y = A[i - 1];
            ll add = (x == y) ? (ll)x : 0;      // 转移 1：与第 i-1 个同色 ⇒ 全部状态 +add

            // 转移 2：第 i 个换颜色。它的"最靠近的同色数"就是原来另一种颜色的最后一个数，
            //         所以只有 v == x 的状态能再拿 x 分，其余状态直接取最大值
            ll cand2 = baseG;
            ll bx = getb(x);
            if (bx > NEG / 2) cand2 = max(cand2, bx + x);

            // 换算成 base 坐标：新 base_[y] = max(旧 base_[y], cand2 - add)
            ll nb = max(getb(y), cand2 - add);
            setb(y, nb);
            if (nb > baseG) baseG = nb;
            shift += add;
        }
        printf("%lld\n", baseG + shift);
    }
    return 0;
}
