// fill2.cpp —— 完善程序(2) 走迷宫（最多 k 次传送），按官方答案 38~42 = C A B D D 补全
//   ① C: {-1, 1, 0, 0}      （与 dy = {0,0,-1,1} 配对：上/下/左/右）
//   ② A: !q.empty()
//   ③ B: dis[u.x][u.y][u.k]
//   ④ D: u.k < k
//   ⑤ D: q.push({nx, ny, nk})
// 编译： g++ -static -O2 -std=c++14 fill2.cpp -o fill2.exe
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 105, MAXK = 15;
const int dx[4] = {-1, 1, 0, 0};              // ① C
const int dy[4] = {0, 0, -1, 1};
int n, m, k, dis[MAXN][MAXN][MAXK];
char grid[MAXN][MAXN];
struct Node {
    int x, y, k;
};
int bfs() {
    memset(dis, 0x3f, sizeof(dis));
    queue<Node> q;
    q.push({1, 1, 0});
    dis[1][1][0] = 0;
    while(!q.empty()) {                        // ② A
        Node u = q.front();
        q.pop();
        if(u.x == n && u.y == m) {
            return dis[u.x][u.y][u.k];         // ③ B
        }
        for(int i = 0; i < 4; i++) {
            int nx = u.x + dx[i];
            int ny = u.y + dy[i];
            if(nx >= 1 && nx <= n && ny >= 1 && ny <= m && grid[nx][ny] != '#') {
                if(dis[nx][ny][u.k] > dis[u.x][u.y][u.k] + 1) {
                    dis[nx][ny][u.k] = dis[u.x][u.y][u.k] + 1;
                    q.push({nx, ny, u.k});
                }
            }
        }
        if(u.k < k) {                          // ④ D
            for(int i = 0; i < 4; i++) {
                int nx = u.x + dx[i] * 2;
                int ny = u.y + dy[i] * 2;
                int nk = u.k + 1;
                if(nx >= 1 && nx <= n && ny >= 1 && ny <= m && grid[nx][ny] != '#') {
                    if(dis[nx][ny][nk] > dis[u.x][u.y][u.k] + 1) {
                        dis[nx][ny][nk] = dis[u.x][u.y][u.k] + 1;
                        q.push({nx, ny, nk});   // ⑤ D
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    cin >> n >> m >> k;
    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++) {
            cin >> grid[i][j];
        }
    }
    cout << bfs() << endl;
    return 0;
}
