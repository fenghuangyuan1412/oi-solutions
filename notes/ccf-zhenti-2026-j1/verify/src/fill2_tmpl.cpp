// 完善程序 (2) 平衡分割。
// 【重要】卷面照片到第 35 行就断了（原卷第 9 页缺失），第 36 行以后和 39—43 题的选项都没有。
// 这里 5 个空的填法取自整理者给出的参考答案 B D C A D 和简析里写明的表达式，
// 结尾的 return 0; } 是按 C++ 语法必然存在的最小补全，不属于卷面内容。
#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;

#ifndef BLANK1
#define BLANK1 c - (c < 'A' ? '0' : 'A' - 10)
#endif
#ifndef BLANK2
#define BLANK2 int r = l; r <= n; ++r
#endif
#ifndef BLANK3
#define BLANK3 sum * 1.0 / (r - l + 1)
#endif
#ifndef BLANK4
#define BLANK4 r + 1, cnt + (r < n), min(mnb, nwb), max(mxb, nwb)
#endif
#ifndef BLANK5
#define BLANK5 1, 0, 1e100, -1e100
#endif

constexpr int N = 25;

int n, a[N];
char s[N];

double ans = 1e100;

int value(char c) { return BLANK1; }
void split(int l, int cnt, double mnb, double mxb) {
  if (l > n) {
    if (cnt == 0) return;
    ans = min(ans, mxb - mnb);
    return;
  }
  int sum = 0;
  for (BLANK2) {
    sum += a[r];
    double nwb = BLANK3;
    split(BLANK4);
  }
}

int main() {
  cin >> n >> s + 1;
  for (int i = 1; i <= n; ++i)
    a[i] = value(s[i]);
  split(BLANK5);
  cout << fixed << setprecision(6) << ans;
  return 0;
}
