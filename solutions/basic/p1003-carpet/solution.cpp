#include <bits/stdc++.h>
using namespace std;

const int N = 10005;
int a[N], b[N], g[N], k[N];   // 第 i 张地毯：左下角 (a,b)，x 方向长 g，y 方向长 k

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;                          // n 可以为 0（题面 0 ≤ n ≤ 1e4），后面一行都不能多读
    for (int i = 1; i <= n; i++) cin >> a[i] >> b[i] >> g[i] >> k[i];

    int x, y;
    cin >> x >> y;

    // 关键：后铺的压在先铺的上面 ⇒ "最上面的"就是编号最大的那张。
    // 所以从 n 倒着往 1 找，第一张罩住 (x,y) 的地毯就是答案，找到立刻停。
    // 不用真的把网格涂出来：坐标到 1e5，涂网格要开 1e5×1e5 的数组（40GB），必炸。
    int ans = -1;                      // 一张都没罩住 ⇒ -1
    for (int i = n; i >= 1; i--) {
        // 题面原话："在矩形地毯边界和四个顶点上的点也算被地毯覆盖" ⇒ 全是闭区间，等号必须取
        if (a[i] <= x && x <= a[i] + g[i] && b[i] <= y && y <= b[i] + k[i]) {
            ans = i;
            break;                     // 少了 break 会继续往前找，拿到的是更下面的地毯
        }
    }

    cout << ans << '\n';
    return 0;
}
