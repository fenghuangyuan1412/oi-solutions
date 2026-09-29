#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

const int N = 305;
int n, a[N], pre[N], dp[N][N];

int main() {
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        pre[i] = pre[i - 1] + a[i];
    }
    for (int len = 2; len <= n; ++len) {
        for (int i = 1; i + len - 1 <= n; ++i) {   // 空①
            int j = i + len - 1;                    // 空②
            dp[i][j] = INT_MAX;                     // 空③
            for (int k = i; k < j; ++k) {
                dp[i][j] = min(dp[i][j],
                    dp[i][k] + dp[k + 1][j] + pre[j] - pre[i - 1]);   // 空④
            }
        }
    }
    cout << dp[1][n] << "\n";                       // 空⑤
    return 0;
}
