// J3 — 十进制转二进制（递归 + 全局计数器）
// 考察：递归展开顺序、全局变量、输出时机
#include <iostream>
using namespace std;

int cnt = 0;

void f(int n) {
    if (n <= 0) return;     // 递归出口
    f(n / 2);               // 先处理更高位
    ++cnt;                  // 记录输出次数
    cout << n % 2;          // 再输出当前位
}

int main() {
    f(13);
    cout << " " << cnt << "\n";
    return 0;
}
// 13 = 1101(2)，递归深度 4 层
// 预期输出：1101 4
