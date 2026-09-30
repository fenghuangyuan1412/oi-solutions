#include <bits/stdc++.h>
using namespace std;
/* DP 版 check（仅验证用，非讲解代码）：
 *   f[i] = 前 i 行能切出的"最多合格带数"，f[i] = max{ f[k] + 1 | 带 [k+1..i] 合格 }
 *   带 [i..j] 是否合格 = 用列前缀和 O(C) 取出列和后跑一遍贪心
 * 单次 check 复杂度 O(R^2 * C)，用来：
 *   1) 与 README 里"贪心切最早带"的 solution.cpp 对拍，验证条带选择的等价性；
 *   2) 实测 O(R^2*C*log) 在 R=C=500 上的真实耗时（见 RESULTS.md）。
 * 编译：g++ -static -O2 -std=c++14 dp_check.cpp -o dp.exe
 */
const int MAXN = 505;
int R, C, A, B;
long long pre[MAXN][MAXN];   // pre[i][j] = 第 j 列前 i 行的和
int f[MAXN];

bool goodStrip(int i, int j, long long X) {   // 带 [i..j] 能否切出 >= B 块 >= X
    int cnt = 0;
    long long cur = 0;
    for (int c = 1; c <= C; ++c) {
        cur += pre[j][c] - pre[i - 1][c];
        if (cur >= X) { ++cnt; cur = 0; }
    }
    return cnt >= B;
}

bool check(long long X) {
    f[0] = 0;
    for (int i = 1; i <= R; ++i) {
        f[i] = -1;
        for (int k = 0; k < i; ++k)
            if (f[k] >= 0 && goodStrip(k + 1, i, X))
                f[i] = max(f[i], f[k] + 1);
    }
    return f[R] >= A;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> R >> C >> A >> B)) return 0;
    long long total = 0;
    for (int i = 1; i <= R; ++i)
        for (int j = 1; j <= C; ++j) {
            long long v; cin >> v;
            pre[i][j] = pre[i - 1][j] + v;
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
