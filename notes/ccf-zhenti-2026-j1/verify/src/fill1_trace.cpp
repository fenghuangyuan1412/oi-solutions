// 完善程序 (1) 进制减半的**逐步打印版**：卷面 28 行原样，5 个空按参考答案填好，
// 只在关键位置插 std::cout（用 show() 一行调用，不打断卷面结构）。
// 用途：README 里那张手推表的每一个中间状态都来自这里真跑出来的输出，不是心算。
#include <iostream>

constexpr int N = 100005;
long long b[N];

long long n_, m_;
void show(const char* tag, int len) {
  std::cout << tag << "  len=" << len << "  b =";
  for (int j = 0; j < len; j++) std::cout << ' ' << b[j];
  long long v = 0;
  for (int j = len - 1; j >= 0; j--) v = v * n_ + b[j];
  std::cout << "   （按 n=" << n_ << " 位权读回 = " << v << "）" << std::endl;
}

int main() {
  long long n, m, d;
  std::cin >> n >> m >> d;
  n_ = n; m_ = m;
  int len = 1;
  show("初始", len);
  for (int i = 0; i < d; i++) {
    long long x;
    std::cin >> x;
    std::cout << "--- 读入位 x=" << x << "（移位+放置，还没进位）" << std::endl;
    for (int j = len; j >= 1; j--)
      b[j] = b[j - 1] * m;
    b[0] = x;
    len++;
    show("  移位后", len);
    for (int j = 0; j < len; j++)
      if (b[j] >= n) {
        b[j + 1] += b[j] / n;
        b[j] = b[j] % n;
        if (j + 1 == len) len++;
      }
    show("  规格化", len);
  }
  while (len > 1 && b[len - 1] == 0) len--;
  std::cout << "输出（高位到低位）：";
  for (int i = len - 1; i >= 0; i--)
    std::cout << b[i] << ' ';
  std::cout << std::endl;
  return 0;
}
