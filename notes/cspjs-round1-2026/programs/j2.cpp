// J2 — 冒泡排序交换次数（逆序对的等价计算）
// 考察：双层循环、swap 操作、循环不变量
#include <iostream>
using namespace std;

int main() {
    int a[6] = {5, 3, 8, 1, 6, 2};
    int cnt = 0;
    for (int i = 1; i <= 5; ++i) {
        for (int j = 5; j >= i; --j) {
            if (a[j] < a[j - 1]) {
                int t = a[j];
                a[j] = a[j - 1];
                a[j - 1] = t;
                ++cnt;
            }
        }
    }
    cout << cnt << "\n";
    for (int i = 0; i < 6; ++i) cout << a[i] << " ";
    cout << "\n";
    return 0;
}
// 预期输出：
// 9
// 1 2 3 5 6 8
