#include <bits/stdc++.h>
using namespace std;

const int N = 1005;
char g[N][N];
bool seen[N][N];
int dr[4] = {0, 1, 0, -1}, dc[4] = {1, 0, -1, 0};  // 0 东 1 南 2 西 3 北

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    for (int i = 1; i <= n; i++) scanf("%s", g[i] + 1);
    int r, c, d;
    scanf("%d %d %d", &r, &c, &d);
    string op;
    cin >> op;

    int cnt = 0;
    seen[r][c] = true;
    cnt = 1;
    for (int t = 0; t < k; t++) {
        if (op[t] == 'R') {
            d = (d + 1) & 3;
        } else {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr >= 1 && nr <= n && nc >= 1 && nc <= m && g[nr][nc] != 'x') {
                r = nr;
                c = nc;
                if (!seen[r][c]) {  // 新格子才让"不同格子数"加一
                    seen[r][c] = true;
                    cnt++;
                }
            } else {
                d = (d + 1) & 3;
            }
        }
    }
    printf("%d %d %d %d\n", r, c, d, cnt);
    return 0;
}
