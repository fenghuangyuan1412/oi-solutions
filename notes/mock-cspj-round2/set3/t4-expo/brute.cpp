#include <bits/stdc++.h>
using namespace std;

const int N = 12, B = 400;  // 小规模：把"时刻"直接当成一层来 BFS
int n, m, k;
int eu[30], ev[30], ea[30];
int vis[N][B + 5];

int main() {
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < m; i++) scanf("%d %d %d", &eu[i], &ev[i], &ea[i]);
    queue<pair<int, int> > q;  // (路口, 时刻)
    vis[1][0] = 1;
    q.push(make_pair(1, 0));
    int ans = -1;
    while (!q.empty()) {
        int u = q.front().first, t = q.front().second;
        q.pop();
        if (u == n) {  // BFS 按时间递增弹出，第一个到终点的时刻就是答案
            ans = t;
            break;
        }
        if (t + 1 > B) continue;
        if (!vis[u][t + 1]) {  // 等 1 分钟
            vis[u][t + 1] = 1;
            q.push(make_pair(u, t + 1));
        }
        if (t % k == 0) {  // 只有 k 的倍数时刻才允许上路
            for (int i = 0; i < m; i++) {
                if (eu[i] == u && t >= ea[i]) {
                    int v = ev[i];
                    if (!vis[v][t + 1]) {
                        vis[v][t + 1] = 1;
                        q.push(make_pair(v, t + 1));
                    }
                }
            }
        }
    }
    printf("%d\n", ans);
    return 0;
}
