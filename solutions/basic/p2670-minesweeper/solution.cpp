#include <bits/stdc++.h>
using namespace std;

int n, m;
char grid[105][105];

// 八个方向的偏移：上、上右、右、下右、下、下左、左、上左
const int dx[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 0; i < n; i++) cin >> grid[i];   // 一行字符串，字符之间没有空格

    // 直接模拟：逐格看自己是什么。'*' 原样输出；'?' 就数八邻域的雷。
    // 复杂度 O(8nm) = 8e4，本题本身就是"暴力即正解"的签到题。
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '*') {              // 雷格本身输出 '*'，不是数字
                cout << '*';
                continue;
            }
            int cnt = 0;
            for (int d = 0; d < 8; d++) {
                int x = i + dx[d], y = j + dy[d];
                if (x >= 0 && x < n && y >= 0 && y < m && grid[x][y] == '*') cnt++;
                // 越界判断必须有，否则角上的格子会读到数组外
            }
            cout << cnt;                          // 0~8，直接输出数字字符，无分隔符
        }
        cout << '\n';
    }
    return 0;
}
