#include <bits/stdc++.h>
using namespace std;

// 20 分档写法：n,m ≤ 5 时直接 DFS 枚举所有不自交路径。
// 这同时是 dp 版（同目录 solution.cpp）的**对拍暴力**：小数据上两者必须逐一相同。
// 复杂度：状态数指数级（每步最多 3 个方向、格子最多 25 个），n,m=6 就已经很勉强，
//         1000×1000 完全不可能 —— 所以它只值 20 分。
int n, m;
int a[10][10];
bool vis[10][10];
int ans = INT_MIN;

// 只能向右 / 向上 / 向下，不能回头，不能越界
const int dr[3] = {0, -1, 1};
const int dc[3] = {1, 0, 0};

void dfs(int r, int c, int sum) {
    if (r == n && c == m) {                 // 到达右下角：这条路径结算
        if (sum > ans) ans = sum;
        return;                             // 终点还要往外走没有意义，直接返回
    }
    for (int d = 0; d < 3; d++) {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr < 1 || nr > n || nc < 1 || nc > m) continue;
        if (vis[nr][nc]) continue;          // "不能重复经过已经走过的方格"
        vis[nr][nc] = true;
        dfs(nr, nc, sum + a[nr][nc]);
        vis[nr][nc] = false;                // 回溯
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) cin >> a[i][j];

    vis[1][1] = true;
    dfs(1, 1, a[1][1]);
    cout << ans << '\n';
    return 0;
}
