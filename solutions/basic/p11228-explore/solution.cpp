/*
 * P11228 [CSP-J 2024] 地图探险 —— 正解（模拟 + 二维标记）
 * ---------------------------------------------------------------------------
 * 题意：n*m 地图，'x' 障碍、'.' 空地。机器人状态 = 位置 (x,y) + 朝向 d
 *       (0 东 1 南 2 西 3 北)。每步：若「面朝方向的下一步」在地图内且是空地，
 *       就走一步（朝向不变）；否则原地向右转 d=(d+1)%4。走 k 步后，问机器人
 *       经过过多少个不同的格子（含起点）。
 *
 * 关键观察：这题就是「照规则模拟」，没有捷径；难点只在两点——
 *   1) 用 dx/dy 两个数组把「朝向 -> 位移」统一表达，避免写 4 段 if 写错；
 *   2) 去重「经过的格子」：开一张 bool vis[n+1][m+1] 边踩边标记，
 *      第一次踩到才让计数 +1。这样统计不同格子是 O(1) 查询。
 *
 * 复杂度：O(n*m + k) 每组。k <= 10^6、T <= 5 ⇒ 约 5*10^6 步，1s 绰绰有余。
 *   对比 solution_partial.cpp 用「把经过的格子塞进 vector，每次都线性查重」，
 *   去重是 O(k^2)，k 到 10^6 直接爆掉，只能拿小数据的分。
 *
 * 编译：g++ -static -O2 -std=c++14 solution.cpp -o p11228_full.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;

    // 朝向 0东 1南 2西 3北 -> (行增量, 列增量)
    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {1, 0, -1, 0};

    while (T--) {
        int n, m;
        long long k;
        cin >> n >> m >> k;

        int x, y, d;                 // 当前位置与朝向（行列 1 起）
        cin >> x >> y >> d;

        vector<string> g(n + 1);     // g[i] 前补一个空格，令 g[i][j] 直接对应第 j 列
        for (int i = 1; i <= n; ++i) {
            string s; cin >> s;
            g[i] = " " + s;
        }

        vector<vector<char>> vis(n + 1, vector<char>(m + 1, 0));  // 二维标记
        auto inside = [&](int xx, int yy) {
            return xx >= 1 && xx <= n && yy >= 1 && yy <= m;
        };

        vis[x][y] = 1;
        int cnt = 1;                 // 起点算已经过

        for (long long step = 0; step < k; ++step) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && g[nx][ny] == '.') {   // 能走就走
                x = nx; y = ny;
                if (!vis[x][y]) { vis[x][y] = 1; ++cnt; }
            } else {                                     // 走不了就右转
                d = (d + 1) % 4;
            }
        }

        cout << cnt << "\n";
    }
    return 0;
}
