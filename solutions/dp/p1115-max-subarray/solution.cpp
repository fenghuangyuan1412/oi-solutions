#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    long long x;
    cin >> x;
    long long cur = x, ans = x;      // 命门：初值取第一个数，不是 0。全负数据下 0 会给出"空段"这个假答案

    for (int i = 2; i <= n; i++) {
        cin >> x;
        // 以 i 结尾的最大子段，要么接在前面那段后面（cur + x），要么从 i 重新开始（x）
        cur = max(x, cur + x);
        ans = max(ans, cur);         // 答案是所有"结尾"里最好的那个，不是最后的 cur
    }
    cout << ans << '\n';
    return 0;
}
