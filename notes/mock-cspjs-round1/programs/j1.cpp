// J1 — 约数统计
// 考察：for 循环、取余判断、累加器变量
#include <iostream>
using namespace std;

int main() {
    int n = 12, cnt = 0, s = 0;
    for (int i = 1; i <= n; ++i) {
        if (n % i == 0) {   // i 是 n 的约数
            ++cnt;           // 约数个数 +1
            s += i;          // 约数之和累加
        }
    }
    cout << cnt << " " << s << "\n";
    return 0;
}
// 预期输出：6 28
