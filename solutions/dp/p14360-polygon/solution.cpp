#include <bits/stdc++.h>
using namespace std;
#define int long long

const int N = 5005;
const int p = 998244353;

int a[N], f[N], sum[N];   // f[s]=当前前缀的物品凑出和恰为 s 的子集数; sum[i]=前 i 个物品中和 <= a[i+1] 的子集数

// 快速幂：求 2^k，用于"前 k 个物品的子集总数"
int qp(int b, int e) {
    int r = 1;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    int maxn = 0;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        maxn = max(maxn, a[i]);
    }
    // 排序后，"下标最大的那根"唯一确定，每个子集恰好被统计一次（值相等时靠下标区分）
    sort(a + 1, a + 1 + n);

    // 以 i (>=3) 为最大下标：{1..i-1} 的任意子集与 a[i] 搭配。
    // 能拼多边形 <=> 其余长度之和 > a[i]。所以
    //   合法方案数 = 2^(i-1) - (和 <= a[i] 的子集数)
    // 后一项正是 sum[i-1]。
    f[0] = 1;                 // 空集，和为 0
    f[a[1]] = 1;              // 只选第 1 根
    for (int i = 2; i <= n; i++) {
        // 01 背包计数：逆序枚举，保证每根木棍至多用一次
        for (int s = maxn; s >= a[i]; s--) {
            f[s] += f[s - a[i]];
            if (f[s] >= p) f[s] -= p;      // 比 %= 快
        }
        // 前缀和截断到 a[i+1]：即以 i+1 为最大下标时，"凑不过去"的子集数
        for (int s = 0; s <= a[i + 1]; s++) {
            sum[i] += f[s];
            if (sum[i] >= p) sum[i] -= p;
        }
    }

    int ans = 0;
    for (int i = 3; i <= n; i++) {   // i<3 时子集元素数不足 3，必然不合法
        ans = (ans + qp(2, i - 1) - sum[i - 1] + p) % p;
    }
    cout << ans << '\n';
    return 0;
}
