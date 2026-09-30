// 独立参考实现：按题面定义枚举所有切分位置集合（k>=1），算每段平均值，取极差最小值。
#include <cstdio>
#include <algorithm>
using namespace std;
int main() {
  int n;
  char s[64];
  scanf("%d %s", &n, s);
  int a[64];
  for (int i = 0; i < n; i++) a[i + 1] = (s[i] < 'A') ? s[i] - '0' : s[i] - 'A' + 10;
  double best = 1e100;
  for (int mask = 1; mask < (1 << (n - 1)); mask++) {  // 至少切 1 刀
    int pos[64], k = 0;
    pos[k++] = 0;
    for (int j = 0; j < n - 1; j++)
      if (mask >> j & 1) pos[k++] = j + 1;
    pos[k++] = n;
    double mn = 1e100, mx = -1e100;
    for (int i = 0; i < k - 1; i++) {
      int sum = 0, len = pos[i + 1] - pos[i];
      for (int t = pos[i] + 1; t <= pos[i + 1]; t++) sum += a[t];
      double b = sum * 1.0 / len;
      mn = min(mn, b);
      mx = max(mx, b);
    }
    best = min(best, mx - mn);
  }
  printf("%.6f\n", best);
  return 0;
}
