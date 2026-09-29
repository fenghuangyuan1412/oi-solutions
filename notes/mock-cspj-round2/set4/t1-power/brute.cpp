#include <bits/stdc++.h>
using namespace std;

int main() {
    long long a, b;
    scanf("%lld %lld", &a, &b);
    __int128 res = 1;  // 暴力：用 128 位整数真的乘到底，再和 10^9 比
    for (long long i = 0; i < b; i++) {
        res *= a;
        if (res > (__int128)1000000000) break;
    }
    if (res > (__int128)1000000000) puts("over");
    else printf("%lld\n", (long long)res);
    return 0;
}
