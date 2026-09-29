#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll LIM = 1000000000LL;

int main() {
    ll a, b;
    scanf("%lld %lld", &a, &b);
    if (b == 0 || a == 1) {  // 1 的任何次方都是 1，单独处理才不会循环 10^9 次
        printf("1\n");
        return 0;
    }
    ll res = 1;
    for (ll i = 0; i < b; i++) {
        if (res > LIM / a) {  // 再乘一次就要爆掉存档字段
            puts("over");
            return 0;
        }
        res *= a;
    }
    printf("%lld\n", res);
    return 0;
}
