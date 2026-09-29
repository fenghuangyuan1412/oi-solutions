#include <bits/stdc++.h>
using namespace std;

int pos[35][35];

// 暴力：真的拿一只"笔"在格子上走，撞墙或踩到已走过的格子就右转
int main() {
    int n, m;
    long long x;
    scanf("%d %d %lld", &n, &m, &x);
    long long rank = 1;
    for (int i = 1, tot = n * m; i <= tot; i++) {
        long long a;
        scanf("%lld", &a);
        if (a > x) rank++;
    }
    memset(pos, 0, sizeof pos);
    int dr[4] = {0, 1, 0, -1}, dc[4] = {1, 0, -1, 0};  // 右 下 左 上
    int r = 1, c = 1, d = 0;
    for (int step = 1; step <= n * m; step++) {
        pos[r][c] = step;
        if (step == rank) { printf("%d %d\n", r, c); return 0; }
        int nr = r + dr[d], nc = c + dc[d];
        if (nr < 1 || nr > n || nc < 1 || nc > m || pos[nr][nc]) {
            d = (d + 1) % 4;
            nr = r + dr[d];
            nc = c + dc[d];
        }
        r = nr;
        c = nc;
    }
    return 0;
}
