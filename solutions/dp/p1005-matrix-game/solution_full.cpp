#include <bits/stdc++.h>
using namespace std;

// ============ 满分档：把 long long 换成 __int128 ============
// 为什么够用：最大答案 = n 行 × 每行 sum_{k=1..m} 1000 * 2^k = 80 * 1000 * (2^81 - 2) ≈ 1.93e29，
// 而 __int128 的上限约 1.7e38（2^127-1），绰绰有余；long long 只有 9.22e18，连 2^63 都撑不到。
// 注意：__int128 是 GCC/Clang 扩展，不是 C++ 标准的一部分。NOI Linux / 洛谷的 g++ 支持，
//       但 cout/printf 都不认识它，**必须自己写输出函数**（下面 printInt128）。
using i128 = __int128_t;

const int M = 85;
int m;
i128 a[M], f[M][M], pw[M];

i128 solveRow() {
    for (int len = 1; len <= m; len++) {
        for (int l = 1; l + len - 1 <= m; l++) {
            int r = l + len - 1;
            int k = m - len + 1;                       // 第 k 次取数，乘 2^k
            f[l][r] = max(a[l] * pw[k] + f[l + 1][r],
                          a[r] * pw[k] + f[l][r - 1]);
        }
    }
    return f[1][m];
}

// 手写输出：负数单独处理，其余"除 10 取余、倒着塞进字符串"
void printInt128(i128 v) {
    if (v == 0) { cout << '0'; return; }
    if (v < 0) { cout << '-'; v = -v; }
    char buf[50];
    int top = 0;
    while (v > 0) { buf[top++] = char('0' + (int)(v % 10)); v /= 10; }
    while (top--) cout << buf[top];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n >> m;
    pw[0] = 1;
    for (int i = 1; i <= m; i++) pw[i] = pw[i - 1] * 2;    // 2^80 在 __int128 里是精确整数

    i128 total = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) { long long t; cin >> t; a[j] = t; }
        memset(f, 0, sizeof f);
        total += solveRow();
    }

    printInt128(total);
    cout << '\n';
    return 0;
}
