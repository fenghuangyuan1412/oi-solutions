#include <bits/stdc++.h>
using namespace std;

// 另一套写法：用 set 记格子，方向用"下一个朝向"表显式写出来
int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    vector<string> g(n + 1);
    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;
        g[i] = " " + s;
    }
    int r, c, d;
    scanf("%d %d %d", &r, &c, &d);
    string op;
    cin >> op;

    set<pair<int, int>> st;
    st.insert({r, c});
    const int NX[4] = {0, 1, 0, -1};
    const int NY[4] = {1, 0, -1, 0};
    for (char ch : op) {
        if (ch == 'R') {
            if (d == 0) d = 1;
            else if (d == 1) d = 2;
            else if (d == 2) d = 3;
            else d = 0;
        } else {
            int nr = r + NX[d], nc = c + NY[d];
            bool ok = (nr >= 1 && nr <= n && nc >= 1 && nc <= (int)g[1].size() - 1 && g[nr][nc] == '.');
            if (ok) {
                r = nr;
                c = nc;
                st.insert({r, c});
            } else {
                d = (d + 1) % 4;
            }
        }
    }
    printf("%d %d %d %d\n", r, c, d, (int)st.size());
    return 0;
}
