#include <bits/stdc++.h>
using namespace std;

/*
 * P1182 数列分段 Section II —— 二分答案的"最小化最大值"模板
 *
 * 题目：把长度 N 的数列切成 M 段（连续），使【每段和的最大值】最小。
 *
 * 为什么是二分答案：
 *   直接枚举切法是 C(N-1, M-1) 种，N = 1e5 时天文数字。
 *   但把问法从"怎么切最优"换成判定"每段和都不超过 X，能不能做到"，
 *   判定就只剩一趟 O(N) 的贪心：
 *       从左往右，能塞就塞，塞不下（cur + a[i] > X）才开新段。
 *   这样得到的段数 cnt 是【所有切法里最少的】（引理见 README），
 *   于是 check(X) = (cnt <= M)。
 *   而 X 越大越容易可行（可行性对 X 单调）⇒ 二分最小的可行 X。
 *
 * 上界 hi = 所有数之和（只切一段时最大段和）。
 * 下界 lo = max(a[i])：每段至少含一个数，所以答案不可能小于单个元素的最大值。
 *
 * 复杂度：O(N log(sum))，sum <= 1e5 * 1e8 = 1e13，约 44 次 check。
 */

const int MAXN = 100005;  // N <= 1e5
int n, m;
long long a[MAXN];

/* check(X)：每段和都不超过 X 时，最少能切成几段？返回"是否 <= M"。 */
bool check(long long X) {
  long long cur = 0;   // 当前这一段已累加的和
  int cnt = 1;         // 段数至少为 1
  for (int i = 1; i <= n; ++i) {
    if (cur + a[i] > X) {   // 再加就超了 -> 当前段收口，从 a[i] 另起一段
      ++cnt;
      cur = a[i];
    } else {
      cur += a[i];          // 塞得下就继续塞：贪心让当前段尽可能长
    }
  }
  return cnt <= m;          // 段数不够可以再拆细，拆只会让最大段和更小
}

int main() {
  if (scanf("%d%d", &n, &m) != 2) return 0;

  long long lo = 0, hi = 0;
  for (int i = 1; i <= n; ++i) {
    scanf("%lld", &a[i]);
    hi += a[i];                 // 上界：全部合成一段
    if (a[i] > lo) lo = a[i];   // 下界：每段至少含一个数
  }

  long long ans = hi;
  while (lo <= hi) {                          // 闭区间模板，找【最小可行值】
    long long mid = lo + (hi - lo) / 2;
    if (check(mid)) {
      ans = mid;                              // mid 可行 -> 记下来，往更小探
      hi = mid - 1;
    } else {
      lo = mid + 1;                           // mid 不可行 -> 只能更大
    }
  }
  printf("%lld\n", ans);
  return 0;
}
