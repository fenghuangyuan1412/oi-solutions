// 完善程序 (1) 进制减半：5 个空用宏传进来，方便把 A/B/C/D 四个选项各编一遍跑对拍。
// 编译示例：g++ ... -DBLANK1='b[j-1]*m' -DBLANK2=x -DBLANK3='b[j]/n' -DBLANK4='b[j]%n' -DBLANK5='len>1&&b[len-1]==0'
#include <iostream>

#ifndef BLANK1
#define BLANK1 b[j - 1] * m
#endif
#ifndef BLANK2
#define BLANK2 x
#endif
#ifndef BLANK3
#define BLANK3 b[j] / n
#endif
#ifndef BLANK4
#define BLANK4 b[j] % n
#endif
#ifndef BLANK5
#define BLANK5 len > 1 && b[len - 1] == 0
#endif

constexpr int N = 100005;
long long b[N];

int main() {
  long long n, m, d;
  std::cin >> n >> m >> d;
  int len = 1;
  for (int i = 0; i < d; i++) {
    long long x;
    std::cin >> x;
    for (int j = len; j >= 1; j--)
      b[j] = BLANK1;
    b[0] = BLANK2;
    len++;
    for (int j = 0; j < len; j++)
      if (b[j] >= n) {
        b[j + 1] += BLANK3;
        b[j] = BLANK4;
        if (j + 1 == len) len++;
      }
  }
  while (BLANK5) len--;
  for (int i = len - 1; i >= 0; i--)
    std::cout << b[i] << ' ';
  return 0;
}
