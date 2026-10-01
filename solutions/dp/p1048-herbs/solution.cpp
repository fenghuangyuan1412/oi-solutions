#include <bits/stdc++.h>
using namespace std;

const int N = 1005;
int T, m;
int f[N];                    // f[j] = 用掉 j 时间能采到的最大价值（一维滚动）

// 命门：内层必须"从大到小"枚举容量。
//   倒序 ⇒ f[j - t] 还是"没考虑第 i 株草药"时的旧值，第 i 株只会被用一次（01 背包）。
//   正序 ⇒ f[j - t] 已经在本轮被更新过，等于允许同一株草药反复采（完全背包）。
int main() {
    scanf("%d%d", &T, &m);
    for (int i = 1; i <= m; i++) {
        int t, v;
        scanf("%d%d", &t, &v);
        for (int j = T; j >= t; j--)                 // j < t 时装不下，保持原值，不用写 else
            f[j] = max(f[j], f[j - t] + v);
    }
    printf("%d\n", f[T]);
    return 0;
}
