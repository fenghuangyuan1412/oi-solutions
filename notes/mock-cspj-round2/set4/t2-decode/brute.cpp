#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ll N;
    scanf("%lld", &N);
    for (ll x = 1; x <= N; x++) {  // 暴力：从小到大一个个试，第一个命中的就是最小解
        ll t = x, s = 0;
        while (t) {
            s += t % 10;
            t /= 10;
        }
        if (x + s == N) {
            printf("%lld\n", x);
            return 0;
        }
    }
    puts("-1");
    return 0;
}
