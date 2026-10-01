#include <bits/stdc++.h>
using namespace std;

const int N = 1005;
string s;
int f[N][N];            // f[i][j]（0 起址，闭区间）= 让 s[i..j] 变成回文最少要插入的字符数

// 区间 DP：看两端。
//   s[i] == s[j] ⇒ 两端已经配对，中间怎么处理与两端无关：f[i][j] = f[i+1][j-1]
//   否则         ⇒ 让 s[i] 去和左端配（在 i 右边插一个 s[i]）或让 s[j] 去和右端配，取小的 +1
// 转移用到更小区间 ⇒ 按区间长度 len 从小到大填表。
int main() {
    cin >> s;
    int L = (int)s.size();

    for (int len = 2; len <= L; len++)              // len == 1 的区间天然是回文，f = 0（全局零初始化）
        for (int i = 0; i + len - 1 < L; i++) {
            int j = i + len - 1;
            if (s[i] == s[j]) f[i][j] = f[i + 1][j - 1];
            else f[i][j] = min(f[i + 1][j], f[i][j - 1]) + 1;
        }

    cout << f[0][L - 1] << '\n';
    return 0;
}
