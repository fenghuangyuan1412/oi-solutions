// S1 — lowbit 枚举二进制中所有 1 位
// 考察：位运算 n&(-n)、补码、循环不变量
#include <iostream>
using namespace std;

int main() {
    int n = 2026, cnt = 0, s = 0;
    while (n > 0) {
        int lb = n & (-n);  // 取最低位的 1
        ++cnt;               // 已剥离位的个数
        s += lb;             // 已剥离位之和
        n -= lb;             // 删除最低位
    }
    cout << cnt << " " << s << "\n";
    return 0;
}
// 2026 = 11111101010(2)，含 8 个 1 位
// 预期输出：8 2026
