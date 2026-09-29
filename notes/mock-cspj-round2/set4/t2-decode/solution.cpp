#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll S(ll x) {
    ll s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

int main() {
    ll N;
    scanf("%lld", &N);
    // x + S(x) = N，而 N ≤ 10^9 时 S(x) 最多 9×10 = 90，所以 x 不可能比 N-90 更小
    for (ll x = max(1LL, N - 200); x <= N; x++) {
        if (x + S(x) == N) {
            printf("%lld\n", x);
            return 0;
        }
    }
    puts("-1");
    return 0;
}
