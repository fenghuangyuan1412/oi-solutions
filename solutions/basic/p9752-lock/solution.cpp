/*
 * P9752 [CSP-S 2023] 密码锁 —— 正解（满分档）
 * ---------------------------------------------------------------------------
 * 题意：五个拨圈、每圈 0~9 循环的密码锁。小 Y 从正确密码出发「只转一次」：
 *       要么转某一个拨圈（幅度 1~9），要么同时转两个「相邻」拨圈且「幅度相同」。
 *       给出锁车后的 n 个状态（n <= 8，且都不是正确密码）。
 *       问：有多少个密码，能按上述方式转出给出的全部 n 个状态。
 *
 * 做法：密码只有 10^5 种，直接全枚举；对每个候选密码检查 n 个状态是否都「一步可达」。
 *   操作量 10^5 x 8 x 5 = 4 x 10^6，1 秒时限下绰绰有余。
 *
 *   一步可达的判定（把差值按模 10 归到 0~9）：
 *     · 恰好 1 个位置非零                → 转了一个拨圈   ✅
 *     · 恰好 2 个位置非零，位置相邻且差值相等 → 转了相邻两圈 ✅
 *     · 0 个位置非零                     → 状态就是密码本身，题面保证不会发生 → ❌
 *     · 其余（2 个不相邻 / 相邻但幅度不同 / 3 个以上）→ 一次转不出来 → ❌
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o solution.exe
 *      （必须加 -static，本机两套 MinGW 路径冲突，不加会 exit 139）
 */
#include <bits/stdc++.h>
using namespace std;

const int W = 5;          // 拨圈个数
int st[9][W + 1];         // n 个观测状态，st[k][1..5]，下标 1 起更顺手
int n;

// 候选密码 p 一步能否转到状态 s
bool reachable(const int p[W + 1], const int s[W + 1]) {
    int dif[W + 1], pos[W + 1], nz = 0;
    for (int i = 1; i <= W; ++i) {
        dif[i] = (s[i] - p[i] + 10) % 10;      // 循环拨圈上的"向前幅度"，0 表示没动
        if (dif[i] != 0) pos[nz++] = i;        // 记下动了哪些位置
    }
    if (nz == 0) return false;                 // 没转 = 状态就是密码，非法
    if (nz == 1) return true;                  // 只动一个拨圈，任意幅度 1~9 都行
    if (nz == 2 && pos[1] == pos[0] + 1 &&     // 位置相邻
        dif[pos[0]] == dif[pos[1]]) return true;  // 且幅度相同
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    for (int k = 0; k < n; ++k)
        for (int i = 1; i <= W; ++i) cin >> st[k][i];

    int ans = 0;
    int p[W + 1];
    for (int x = 0; x < 100000; ++x) {         // 00000~99999 全枚举
        int t = x;
        for (int i = W; i >= 1; --i) { p[i] = t % 10; t /= 10; }   // 高位补 0，不会漏

        bool ok = true;
        for (int k = 0; k < n && ok; ++k)      // 只要有一个状态解释不了就淘汰
            if (!reachable(p, st[k])) ok = false;
        if (ok) ++ans;
    }

    cout << ans << "\n";
    return 0;
}
