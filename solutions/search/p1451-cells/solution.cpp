#include <bits/stdc++.h>
using namespace std;

const int MAXN = 105;            // 题面上限 100x100

int n, m;
char cell[MAXN][MAXN];           // 直接存字符：'0' 是空白，'1'~'9' 都是细胞
bool vis[MAXN][MAXN];            // 全局"已经归进某个细胞了"，本题绝不撤销
int ans;

// ★ 与 P1596（数水塘）唯一的代码差异：这里是 4 个方向。
// 题面原话"沿细胞数字上下左右若还是细胞数字则为同一细胞"，只提上下左右，斜角不算连通。
const int dx[4] = {-1, 1, 0, 0};     // 上、下、左、右
const int dy[4] = {0, 0, -1, 1};

// 洪水填充：把与 (x,y) 四方向连通的所有细胞格统统标记。
void flood(int x, int y) {
    vis[x][y] = true;                // 进函数第一件事：先标记自己，否则会被反复展开
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;   // 先判范围，再访问数组
        if (cell[nx][ny] != '0' && !vis[nx][ny]) flood(nx, ny);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> cell[i];   // 每行是长度 m 的数字串，字符间无空格

    // 扫描 + 洪泛：碰到没归类的细胞格就说明发现一个新细胞，整块淹掉，
    // 块里剩下的格子之后再被扫到时 !vis 不成立，不会重复计数。
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (cell[i][j] != '0' && !vis[i][j]) {
                ans++;
                flood(i, j);
            }

    cout << ans << '\n';
    return 0;
}
