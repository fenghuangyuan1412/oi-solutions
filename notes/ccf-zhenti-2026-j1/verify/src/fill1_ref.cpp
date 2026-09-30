// 独立参考实现：把 mn 进制数按定义转成 n 进制，不碰卷面那套"移位+进位"写法。
// 输入格式与卷面一致：n m d，然后 d 个数从高位到低位。
#include <iostream>
#include <vector>
using namespace std;
int main() {
  long long n, m, d;
  cin >> n >> m >> d;
  long long base = m * n, v = 0;
  for (int i = 0; i < d; i++) { long long x; cin >> x; v = v * base + x; }
  vector<long long> dig;
  if (v == 0) dig.push_back(0);
  while (v > 0) { dig.push_back(v % n); v /= n; }
  for (int i = (int)dig.size() - 1; i >= 0; i--) cout << dig[i] << ' ';
  return 0;
}
