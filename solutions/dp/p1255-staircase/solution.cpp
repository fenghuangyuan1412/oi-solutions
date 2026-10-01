#include <bits/stdc++.h>
using namespace std;

// 数楼梯：f[n] = f[n-1] + f[n-2]（最后一步跨 1 阶或跨 2 阶，两类不重不漏）
// 命门是 N <= 5000：F(5001) 有 1045 位，long long 只有 19 位，必须手写高精度加法。
// 这里用 string 存十进制、按位加，是最容易讲清、也最容易背下来的一种写法。
string addString(const string& x, const string& y) {
    string r;
    int carry = 0;
    for (int i = (int)x.size() - 1, j = (int)y.size() - 1;
         i >= 0 || j >= 0 || carry; i--, j--) {                 // carry 也要进循环，否则最高位丢 1
        int a = i >= 0 ? x[i] - '0' : 0;
        int b = j >= 0 ? y[j] - '0' : 0;
        int s = a + b + carry;
        r.push_back(char('0' + s % 10));
        carry = s / 10;
    }
    reverse(r.begin(), r.end());                                 // 低位先算，最后整体翻转
    return r;
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    if (n == 0) { cout << 0 << '\n'; return 0; }                 // 递推式推不出 f[0]，按题意（1<=N）本不该出现
    string f1 = "1", f2 = "2";                                   // f[1]=1 种，f[2]=2 种
    if (n == 1) { cout << f1 << '\n'; return 0; }
    if (n == 2) { cout << f2 << '\n'; return 0; }
    string cur;
    for (int i = 3; i <= n; i++) {
        cur = addString(f2, f1);
        f1 = f2;
        f2 = cur;
    }
    cout << f2 << '\n';
    return 0;
}
