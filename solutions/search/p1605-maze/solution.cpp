#include <bits/stdc++.h>
using namespace std;

const int MAXN = 8;              // 题面上限只有 5x5，数组开大一点当"哨兵"，心里更踏实

int n, m, t;
bool blocked[MAXN][MAXN];        // 障碍格：永久不可走
bool vis[MAXN][MAXN];            // 只在"当前正在试的这条路"上有效，不是全局访问标记
int fx, fy;
int ans;

// 上下左右四个方向。和 P1596（八方向数水塘）唯一的区别就是这张表只有 4 项。
const int dx[4] = {1, -1, 0, 0};
const int dy[4] = {0, 0, 1, -1};

void dfs(int x, int y) {
    // 到达终点 = 找到一条完整方案，计 1 后立刻返回。
    // "立刻返回"是题意：方案是"从起点到终点"，站到终点就已经走完了，不能借道终点继续逛。
    if (x == fx && y == fy) {
        ans++;
        return;
    }

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        // 顺序不能反：先判断下标合法，再访问数组。
        // 写成 if (blocked[nx][ny] ... ) 而 nx 已经是 0 或 n+1，就是越界读，属于未定义行为。
        if (nx < 1 || nx > n || ny < 1 || ny > m) continue;
        if (blocked[nx][ny] || vis[nx][ny]) continue;   // 障碍不能走；这条路已经走过的格子也不能再走

        vis[nx][ny] = true;      // 走进这个格子
        dfs(nx, ny);             // 把剩下的路全交出去
        vis[nx][ny] = false;     // 回溯：这条路试完了，把格子还给后面的路用
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> t;
    int sx, sy;
    cin >> sx >> sy >> fx >> fy;
    for (int i = 0; i < t; i++) {
        int x, y;
        cin >> x >> y;
        blocked[x][y] = true;
    }

    // 起点也"经过了一次"，所以进 DFS 之前就要标上；
    // 它不需要撤销，因为整次搜索只有这一条起始路径。
    vis[sx][sy] = true;
    dfs(sx, sy);

    cout << ans << '\n';
    return 0;
}
