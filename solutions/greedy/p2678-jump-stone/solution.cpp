#include <bits/stdc++.h>
using namespace std;

/*
 * P2678 [NOIP 2015 提高组] 跳石头 —— 二分答案的"最大化最小值"模板
 *
 * 题目：河道长 L，中间 N 块石头在 d[1..N]，至多移走 M 块（起点 0 与终点 L 不能移），
 *       使【相邻落点距离的最小值】尽可能大，求这个最大值。
 *
 * 与 P1182 数列分段是同一套框架、方向相反：
 *   P1182：最小化最大值，check 里"装不下才开新段"；
 *   本题  ：最大化最小值，check 里"太近就拆掉这块"。
 *
 * 二分答案 X = "相邻落点距离至少为 X" 这个目标。
 * check(X)：从左往右贪心，维护上一个保留的落点 last（初始为起点 0）：
 *     若 d[i] - last < X，则这块石头必须移走（cnt++）；
 *     否则保留它，last = d[i]。
 *   扫描完再看终点：若 L - last < X，还得再移走一块（就是刚保留的 last，
 *   因为 L - 前一块 >= (L - last) + (last - 前一块) > X，移走 last 一定够），cnt++。
 *   cnt <= M 即可行。
 * 可行性对 X 单调（X 越小越容易），所以二分最大的可行 X。
 *
 * 复杂度：O(N log L)，log L <= 30，N <= 5e4，约 1.5e6 次操作。
 */

const int MAXN = 50005;  // N <= 50000
int L, n, m;
int a[MAXN];             // a[0] = 0（起点），a[1..n] = 各石头到起点的距离

/* check(X)：让相邻落点距离都 >= X，最少要移走几块石头？返回"是否 <= M"。 */
bool check(int X) {
  int cnt = 0;    // 需要移走的石头数
  int last = 0;   // 上一个被保留的落点（下标；a[0] 是起点 0）
  for (int i = 1; i <= n; ++i) {
    if (a[i] - a[last] < X) {
      ++cnt;              // 离上一落脚点太近 -> 这块必须移走
    } else {
      last = i;           // 够远 -> 保留它，落脚点前移
    }
  }
  if (L - a[last] < X) ++cnt;   // 终点前的最后一段也不够：移走刚保留的那块即可
  return cnt <= m;
}

int main() {
  if (scanf("%d%d%d", &L, &n, &m) != 3) return 0;
  a[0] = 0;
  for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);

  // 二分最大的可行 X。答案落在 [0, L] 内（把石头全移走就是 L）
  int lo = 0, hi = L, ans = 0;
  while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;   // 不写 (lo+hi)/2：lo+hi 最大 2e9，贴着 int 上限
    if (check(mid)) {
      ans = mid;                    // mid 可行 -> 记下，往更大探
      lo = mid + 1;
    } else {
      hi = mid - 1;                 // mid 不可行 -> 只能更小
    }
  }
  printf("%d\n", ans);
  return 0;
}
