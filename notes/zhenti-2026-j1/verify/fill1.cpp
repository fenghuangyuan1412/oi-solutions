// fill1.cpp —— 完善程序(1) 序列第 k 小，按官方答案 33~37 = A C D D A 补全
//   ① A: a[i] <= x      ② C: k <= c       ③ D: 2e9
//   ④ D: l + (r - l) / 2                  ⑤ A: l = x + 1
// 编译： g++ -static -O2 -std=c++14 fill1.cpp -o fill1.exe
#include <iostream>

using namespace std;

int n, k, a[100'005];
bool check(int x) {
    int c = 0;
    for (int i = 1; i <= n; i++)
        if (a[i] <= x)          // ① A
            c++;
    return k <= c;              // ② C
}
int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
        cin >> a[i];
    int l = 1, r = 2e9;         // ③ D  (2e9 是 double，赋给 int 得 2000000000，未超 INT_MAX)
    while (l < r) {
        int x = l + (r - l) / 2; // ④ D  (不写 (l+r)/2 是为了避免 l+r 溢出)
        if (check(x))
            r = x;
        else
            l = x + 1;           // ⑤ A
    }
    cout << l << endl;
    return 0;
}
