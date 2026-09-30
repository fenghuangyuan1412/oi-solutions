#include <bits/stdc++.h>
using namespace std;
/* 二分答案 + 真·二维前缀和 + 贪心选带（O(RC logV)，不超时）。
 * 与 sol_2dpre.cpp 的唯一区别：选带不再用 O(R^2) 的 DP，
 * 而是"每条带取最短可行长度"的贪心 —— 靠的是"合格性对扩行单调"（README 引理 3）。
 * 二维前缀和在这里当 O(1) 矩形求和的查询工具。
 *
 * pre[i][j] = 左上 (1,1) 到 (i,j) 的豆数和
 * rect(r1,c1,r2,c2) = pre[r2][c2]-pre[r1-1][c2]-pre[r2][c1-1]+pre[r1-1][c1-1]
 */
const int MAXN = 505;
int R, C, A, B;
long long pre[MAXN][MAXN];

static inline long long rect(int r1, int c1, int r2, int c2) {
    return pre[r2][c2] - pre[r1 - 1][c2] - pre[r2][c1 - 1] + pre[r1 - 1][c1 - 1];
}

// 带 [r1..r2] 用二维前缀和逐列取列和，贪心数块，能否 >= B 块且每块 >= X
bool stripOK(int r1, int r2, long long X) {
    int cnt = 0;
    long long cur = 0;
    for (int c = 1; c <= C; ++c) {
        cur += rect(r1, c, r2, c);
        if (cur >= X) { ++cnt; cur = 0; }
    }
    return cnt >= B;
}

// check(X)：贪心选带 —— 从当前行起取"最短合格带"，收口后继续
bool check(long long X) {
    int strips = 0, i = 1;
    while (i <= R) {
        int end = -1;
        for (int j = i; j <= R; ++j) {
            if (stripOK(i, j, X)) { end = j; break; }   // 第一次合格就停，行留给后面的带
        }
        if (end == -1) return false;
        if (++strips >= A) return true;                 // 剩余行并进最后一条带，仍合格
        i = end + 1;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> R >> C >> A >> B)) return 0;
    long long total = 0;
    for (int i = 1; i <= R; ++i)
        for (int j = 1; j <= C; ++j) {
            long long v; cin >> v;
            pre[i][j] = pre[i - 1][j] + pre[i][j - 1] - pre[i - 1][j - 1] + v;
            total += v;
        }
    long long lo = 0, hi = total / (1LL * A * B), ans = 0;
    while (lo <= hi) {
        long long mid = (lo + hi) >> 1;
        if (check(mid)) { ans = mid; lo = mid + 1; } else hi = mid - 1;
    }
    cout << ans << '\n';
    return 0;
}
