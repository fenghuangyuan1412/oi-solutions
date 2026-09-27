// P14359 [CSP-J 2025] 异或和
// 原作者解法（逻辑经三方对拍验证正确），保留原样并补充逐段说明。
//
// 两步转化：
//   1) 前缀异或：[l,r] 权值为 k  <=>  sum[l-1] == sum[r] ^ k
//   2) 最多不相交区间 => 按右端点扫描，能成段就立刻收割（earliest finish 贪心）

#include <bits/stdc++.h>
using namespace std;

const int N = 1e7;            // d[] 长度。值域其实只有 2^20，开 1e7 约 40MB，浪费但安全
int a[500005], sum[500005], d[N];
// d[v] = 前缀异或值 v 最近一次出现的下标；-1 表示从未出现

int main() {
    int n, k;
    cin >> n >> k;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        sum[i] = sum[i - 1] ^ a[i];   // 前缀异或，sum[0] 全局默认 0
    }

    int lst = 0, ans = 0;             // lst = 上一个已选区间的右端点（前缀下标）
    memset(d, -1, sizeof(d));         // 必须显式置 -1：0 是合法下标，不能拿默认值当"未出现"
    d[0] = 0;                         // 空前缀 sum_0 = 0 在下标 0，必须放在 memset 之后

    for (int i = 1; i <= n; i++) {
        int x = sum[i] ^ k;           // 要让 [l,i] 合法，sum[l-1] 必须等于 x
        if (d[x] >= lst) {            // 最近的匹配位置还没被上一个区间覆盖 => 可以成段
            ans++;
            lst = i;                  // 最早结束贪心：立刻收割
        }
        d[sum[i]] = i;                // 登记当前前缀。顺序关键：必须写在 if 之后
    }

    cout << ans;
    return 0;
}

/* 等价写法一：把桶压到值域大小，内存 40MB -> 4MB
 *   const int V = 1 << 20;
 *   int d[V];
 *
 * 等价写法二：省掉 a[]，边读边算
 *   for (int i = 1; i <= n; i++) { int x; cin >> x; sum[i] = sum[i-1] ^ x; }
 *
 * 等价写法三：O(n) DP（同一个贪心的显式形式，可用于自查）
 *   f[i] = max(f[i-1], f[d[sum[i]^k]] + 1)   // 若 d[..] == -1 则只取 f[i-1]
 * 成立原因：f 单调不减，所以最近的匹配下标就给到最大的 f[j]。
 */
