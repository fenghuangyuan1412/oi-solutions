/*
 * P11228 [CSP-J 2024] 地图探险 —— 骗分 / 暴力版（模拟 + 线性查重）
 * ---------------------------------------------------------------------------
 * 模拟规则和正解【完全一样】，唯一区别是「统计经过了多少个不同格子」：
 *   正解用 bool vis[n][m] 二维数组，查询 O(1)；
 *   这里把经过的格子依次塞进 vector<pair>，每次要判断「走过没有」就线性扫一遍，
 *   去重代价 O(已经过的格子数)，整体 O(k^2)。
 *
 * 能拿分：测试点 1~6 里 k 最大 2000，O(k^2)=4*10^6，完全跑得动 ⇒ 约 60 分。
 * 会挂：测试点 7~10 的 k 到 10^6，O(k^2)=10^12，必然超时。
 *
 * 考场用法：先交这份把小数据兜住；把去重换成二维 bool 数组就是正解。
 *
 * 编译：g++ -static -O2 -std=c++14 solution_partial.cpp -o p11228_partial.exe
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;

    const int dx[4] = {0, 1, 0, -1};
    const int dy[4] = {1, 0, -1, 0};

    while (T--) {
        int n, m;
        long long k;
        cin >> n >> m >> k;

        int x, y, d;
        cin >> x >> y >> d;

        vector<string> g(n + 1);
        for (int i = 1; i <= n; ++i) { string s; cin >> s; g[i] = " " + s; }

        vector<pair<int,int>> visited;            // 经过的格子按到达顺序存
        visited.push_back({x, y});

        auto inside = [&](int xx, int yy) {
            return xx >= 1 && xx <= n && yy >= 1 && yy <= m;
        };
        auto seen = [&](int xx, int yy) {         // 线性查重：O(visited.size())
            for (auto &p : visited)
                if (p.first == xx && p.second == yy) return true;
            return false;
        };

        for (long long step = 0; step < k; ++step) {
            int nx = x + dx[d], ny = y + dy[d];
            if (inside(nx, ny) && g[nx][ny] == '.') {
                x = nx; y = ny;
                if (!seen(x, y)) visited.push_back({x, y});
            } else {
                d = (d + 1) % 4;
            }
        }

        cout << (int)visited.size() << "\n";
    }
    return 0;
}
