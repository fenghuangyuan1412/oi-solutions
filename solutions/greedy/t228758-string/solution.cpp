#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s1, s2;
    cin >> s1 >> s2;  // 一行两个字符串，空格隔开

    // 观察 2：s2' 取多长都不如只取第一个字符，所以 c 就是 s2 的首字符
    const char c = s2[0];

    // 观察 3：s1' 从最短的合法前缀 s1[0] 开始，
    // 只有下一个字符严格小于 c 时才值得再吃进去一个
    int k = 1;
    while (k < (int)s1.size() && s1[k] < c) ++k;

    cout << s1.substr(0, k) << c << '\n';
    return 0;
}
