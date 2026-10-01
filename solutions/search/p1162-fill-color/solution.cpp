#include <bits/stdc++.h>
using namespace std;

const int MAXN = 35;             // 题面上限 n <= 30

int n;
int a[MAXN][MAXN];               // 原图：0 空格，1 墙壁（闭合圈）
bool outside[MAXN][MAXN];        // "能到达边界"的 0 格 —— 也就是圈外

// 只在 0 格里走，四方向（题面："只向上下左右 4 个方向移动且仅经过其他 0"）
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

// 从某个边界 0 格出发做洪泛，把所有"能走到边界"的 0 全标记成 outside。
// 和 P1596/P1451 一样：标记了绝不撤销 —— 我们只关心"这块属不属于圈外"，不数路径。
void flood(int x, int y) {
    outside[x][y] = true;
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;   // 先判范围再访问数组
        if (a[nx][ny] == 0 && !outside[nx][ny]) flood(nx, ny);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> a[i][j];                                   // 数字之间有空格，逐个 int 读最稳

    // ★ 本题的教学重点：正着判"这个 0 能不能到边界"，每个 0 都要重搜一遍，O(n^4)；
    //   逆过来想 —— "能从边界出发的 0 全是圈外"，从四条边一起洪泛一次就够了，O(n^2)。
    //   注意遍历的是"整条边界"，不是某一个角：圈可以贴边，角上那个格子完全可能是 1。
    for (int i = 0; i < n; i++) {
        if (a[0][i] == 0 && !outside[0][i]) flood(0, i);           // 上边
        if (a[n - 1][i] == 0 && !outside[n - 1][i]) flood(n - 1, i); // 下边
        if (a[i][0] == 0 && !outside[i][0]) flood(i, 0);           // 左边
        if (a[i][n - 1] == 0 && !outside[i][n - 1]) flood(i, n - 1); // 右边
    }

    // 剩下的 0 到不了边界 => 按题面定义就在闭合圈内，涂成 2。
    // 是 1 的格子原样输出，圈外的 0 也原样输出 0。
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int v = a[i][j];
            if (v == 0 && !outside[i][j]) v = 2;
            if (j) cout << ' ';                                 // 数字之间一个空格，行末不留空格
            cout << v;
        }
        cout << '\n';
    }
    return 0;
}
