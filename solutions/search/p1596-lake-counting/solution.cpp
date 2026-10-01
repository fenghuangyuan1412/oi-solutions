#include <bits/stdc++.h>
using namespace std;

const int MAXN = 105;            // 题面上限 100x100，开一圈哨兵省得每次判边界

int n, m;
char field[MAXN][MAXN];          // 'W' = 水，'.' = 干地
bool vis[MAXN][MAXN];            // 全局"已经归到某个水塘里了"，注意：本题绝不撤销
int ans;

// ★ 本题与 P1451（求细胞数量）唯一的区别就是这两行：这里 8 个方向，那里 4 个方向。
// 顺序：左上、上、右上、右、下右、下、下左、左 —— 覆盖 {-1,0,1}^2 去掉 (0,0) 的全部 8 格。
const int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
const int dy[8] = { 1, 1, 1, 0,-1,-1, -1,  0};

// 洪水填充：把与 (x,y) 八方向连通的所有水格统统标记。
// 这里 vis 一旦置位就再也不清除 —— 我们数的是"块"，同一块水塘只该被数一次。
void flood(int x, int y) {
    vis[x][y] = true;
    for (int d = 0; d < 8; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;   // 先判范围再访问数组
        if (field[nx][ny] == 'W' && !vis[nx][ny]) flood(nx, ny);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> field[i];   // 一行字符串，字符之间没有空格

    // 扫描整个田地：遇到一块"还没被归进任何水塘"的水，就水塘数 +1，
    // 然后把它整块淹掉（标记），这样这块水剩下的格子不会再被数第二遍。
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (field[i][j] == 'W' && !vis[i][j]) {
                ans++;
                flood(i, j);
            }

    cout << ans << '\n';
    return 0;
}
