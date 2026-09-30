#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;  // 全大写字母，长度 1~1e4

    long long cntA = 0;  // 已经扫过的 A 的个数
    long long ans = 0;   // 已经形成的 (A 在前, C 在后) 对数
    for (char ch : s) {
        if (ch == 'A') {
            ++cntA;              // 这个 A 将来能和它右边的每个 C 各配一对
        } else if (ch == 'C') {
            ans += cntA;         // 右边的 C 每出现一个，就把左边的 A 全配掉
        }
        // 其余字母既不开头也不结尾，直接跳过
    }

    cout << ans << '\n';
    return 0;
}
