#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int n;
string s;
int dfs(string t, int lst) {
    int ret = 0;
    for (int i = 0; i <= n; i++)
        if (t[i] != '1')
            ret = 1e9;
    if (ret == 0)return 0;
    for (int i = 0; i <= n; i++)
        if (i != lst && s.substr(n - i, i) == t.substr(0, i)) {
            string cur = t;
            cur[i] ^= 1;
            ret = min(ret, dfs(cur, i) +1);
        }
    return ret;
}
int main() {
    cin >> s;
    n = s.size();
    string t0(n + 1, '0');
    cout << dfs(t0, -1);
    return 0;
}
