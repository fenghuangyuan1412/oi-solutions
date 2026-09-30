#include <iostream>
using namespace std;
int main() {
  int n;
  cin >> n;
  int x = 1, y = 1;
  while (n > 0) {
    if (n % 2 == 0) {
      ++x;
    } else {
      ++x;
      ++y;
    }
    n = n / 2;
  }
  cout << x << ' ' << y << endl;
  return 0;
}
// ↑ 卷面第 1—18 行原样。文件行号 == 卷面行号，题目说"第 11 行"就在这个数第 11 行。
