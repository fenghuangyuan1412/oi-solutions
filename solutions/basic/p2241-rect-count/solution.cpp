#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    cin >> n >> m;       // n, m ≤ 5000

    // 枚举"子矩形的宽 w × 高 h"，而不是枚举四个端点坐标。
    //   尺寸 w×h 的子矩形个数 = (n - h + 1) * (m - w + 1)
    //     （左上角行号可取 1..n-h+1，列号可取 1..m-w+1）
    //   w == h 是正方形，否则是长方形（题面明确：长方形不含正方形）
    // 复杂度 O(nm) = 2.5e7，一次乘法一次加法，稳过。
    //
    // 反面教材：枚举左上+右下四个坐标是 O(n^2 m^2) ≈ 6.25e14，必 TLE。
    long long sq = 0, rect = 0;
    for (long long h = 1; h <= n; h++) {
        for (long long w = 1; w <= m; w++) {
            long long cnt = (n - h + 1) * (m - w + 1);   // 最大 5000*5000 = 2.5e7，仍要 long long
            if (w == h) sq += cnt;
            else       rect += cnt;
        }
    }

    cout << sq << ' ' << rect << '\n';   // 输出顺序：正方形 长方形，别写反
    return 0;
}
