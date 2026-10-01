#include <bits/stdc++.h>
using namespace std;

const int N = 25;              // 坐标 0..20
int bx, by, hx, hy;
bool blocked[N][N];            // 马的控制点（含马本身）共 9 个
long long f[N][N];             // f[i][j] = 从 (0,0) 走到 (i,j) 的路径条数

// 命门一：答案可以是 C(40,20) = 137846528820，超过 int 上限 21 亿，必须 long long。
// 命门二：只能向下或向右（每次 x+1 或 y+1） ⇒ f[i][j] = f[i-1][j] + f[i][j-1]，控制点直接置 0（不可经过）。
int main() {
    scanf("%d%d%d%d", &bx, &by, &hx, &hy);

    // 马 + 8 个"日"字落点
    static const int dx[9] = {0, -2, -2, -1, -1, 1, 1, 2, 2};
    static const int dy[9] = {0, -1, 1, -2, 2, -2, 2, -1, 1};
    for (int k = 0; k < 9; k++) {
        int x = hx + dx[k], y = hy + dy[k];
        if (x >= 0 && x <= bx && y >= 0 && y <= by) blocked[x][y] = true;   // 越界的控制点要丢掉
    }

    f[0][0] = blocked[0][0] ? 0 : 1;               // 题目保证起点不是控制点，这行只是把逻辑写全
    for (int i = 0; i <= bx; i++)
        for (int j = 0; j <= by; j++) {
            if (i == 0 && j == 0) continue;
            if (blocked[i][j]) { f[i][j] = 0; continue; }
            f[i][j] = (i > 0 ? f[i - 1][j] : 0) + (j > 0 ? f[i][j - 1] : 0);
        }

    printf("%lld\n", f[bx][by]);
    return 0;
}
