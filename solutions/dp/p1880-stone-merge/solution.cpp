#include <bits/stdc++.h>
using namespace std;

/*
 * P1880 [NOI1995] 石子合并 —— 环形区间 DP
 *
 * 一、先把"直线版"想清楚
 *   把 n 堆石子排成一条线，每次只能合并相邻两堆，合并的得分是这两堆的石子数之和。
 *   设 f[i][j] = 把区间 [i..j] 合并成一堆的最小得分。
 *   看【最后一次合并】：它必然把 [i..k] 与 [k+1..j] 这两堆并成一堆，
 *   而这一次的得分恰好是这两堆的总石子数，也就是 sum(i..j) ——
 *   与 k 取在哪里【完全无关】（无论怎么切，最后并起来的总数都一样）。
 *   于是
 *       f[i][j] = min_{i <= k < j} ( f[i][k] + f[k+1][j] ) + sum(i..j)
 *   求最大得分同理，把 min 换成 max 即可。
 *
 * 二、环形怎么办：断环成链
 *   圆形上"最后合并的两堆"可能跨越第 n 堆与第 1 堆的边界，
 *   直线 DP 覆盖不到这种切法。
 *   标准技巧：把数组【复制一倍】，a[1..2n] = 原序列 接 原序列，
 *   在长度 2n 的直线上做完全相同的 DP，然后枚举"环从哪里剪开"：
 *       答案 = min / max over i = 1..n of  f[i][i+n-1]
 *   这样每一种环形方案都对应某条长度 n 的直线区间，不重不漏。
 *
 * 复杂度：状态 O(n^2)、转移 O(n) ⇒ O(n^3) = 100^3 = 1e6，双 DP 也只有 2e6。
 */

const int MAXN = 205;          // 2n <= 200
const int INF = 0x3f3f3f3f;

int n;
int a[MAXN];                   // a[1..2n]，后 n 个是前 n 个的复制
int pre[MAXN];                 // 前缀和：sum(i..j) = pre[j] - pre[i-1]
int fmin_[MAXN][MAXN], fmax_[MAXN][MAXN];

int main() {
  if (scanf("%d", &n) != 1) return 0;
  for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
  for (int i = 1; i <= n; ++i) a[i + n] = a[i];            // 断环成链
  for (int i = 1; i <= 2 * n; ++i) pre[i] = pre[i - 1] + a[i];

  // 长度 1 的区间已经是一堆，不需要合并，得分 0
  for (int i = 1; i <= 2 * n; ++i) fmin_[i][i] = fmax_[i][i] = 0;

  for (int len = 2; len <= n; ++len) {                     // 区间长度：必须【先短后长】
    for (int i = 1; i + len - 1 <= 2 * n; ++i) {           // 区间左端
      int j = i + len - 1;                                 // 区间右端
      fmin_[i][j] = INF;
      fmax_[i][j] = -INF;
      int s = pre[j] - pre[i - 1];                         // 最后一次合并的得分（与 k 无关）
      for (int k = i; k < j; ++k) {                        // 枚举分割点
        fmin_[i][j] = min(fmin_[i][j], fmin_[i][k] + fmin_[k + 1][j] + s);
        fmax_[i][j] = max(fmax_[i][j], fmax_[i][k] + fmax_[k + 1][j] + s);
      }
    }
  }

  int ansmin = INF, ansmax = -INF;
  for (int i = 1; i <= n; ++i) {                           // 枚举环的 N 种剪法
    ansmin = min(ansmin, fmin_[i][i + n - 1]);
    ansmax = max(ansmax, fmax_[i][i + n - 1]);
  }
  printf("%d\n%d\n", ansmin, ansmax);
  return 0;
}
