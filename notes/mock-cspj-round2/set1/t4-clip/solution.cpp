#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int N = 90005;
int f[N];  // f[s] = 总时长恰好为 s 秒的选法数

int main() {
    int n;
    long long S;
    scanf("%d %lld", &n, &S);
    vector<int> a(n + 1);
    long long sum = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        sum += a[i];
    }
    if (S >= sum) {  // 所有子集都不够长
        puts("0");
        return 0;
    }
    int lim = (int)min(S, sum);
    f[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (int s = lim; s >= a[i]; s--) {
            f[s] += f[s - a[i]];
            if (f[s] >= MOD) f[s] -= MOD;
        }
    }
    long long le = 0;  // 总时长 <= S 的子集数（含空集）
    for (int s = 0; s <= lim; s++) {
        le += f[s];
        if (le >= MOD) le -= MOD;
    }
    long long all = 1;  // 2^n mod MOD
    for (int i = 1; i <= n; i++) all = all * 2 % MOD;
    // 答案 = (全体子集 - 时长 <= S 的子集)；空集在两边各出现一次，正好抵消
    printf("%lld\n", (all - le + MOD) % MOD);
    return 0;
}
