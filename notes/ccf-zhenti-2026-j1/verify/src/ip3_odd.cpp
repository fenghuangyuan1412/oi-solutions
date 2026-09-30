#include <iostream>
using namespace std;
bool check_prime(int x) {
  if (x <= 1) return false;
  for (int i = 2; i * i <= x; i++) {
    if (x % i == 0) return false;
  }
  return true;
}
int n;
void search_result(int x) {
  if (!check_prime(x)) return;
  if (x >= n) {
    cout << x << endl;
    return;
  }
  for (int i = 1; i <= 9; i += 2) {
    search_result(x * 10 + i);
  }
}
int main() {
  cin >> n;
  for (int i = 1; i <= 9; i++) search_result(i);
  return 0;
}
// ↑ 只把卷面第 17 行的 for (int i = 0; i <= 9; i++) 换成 for (int i = 1; i <= 9; i += 2)，其余原样（题 30）。
