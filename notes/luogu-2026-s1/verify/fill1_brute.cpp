// fill1_brute.cpp -- 完善程序(1) 的朴素对照：逐个 qpow(i, i) mod 998244353
#include <bits/stdc++.h>
using namespace std;
const int mod = 998244353;
int qpow(long long a, long long b) {
    long long r = 1;
    while (b) { if (b & 1) r = r * a % mod; a = a * a % mod; b >>= 1; }
    return (int)r;
}
int main(int argc, char** argv) {
    int n = 3000;
    if (argc > 1) n = atoi(argv[1]);
    else if (scanf("%d", &n) != 1) n = 3000;     // 也支持从 stdin 读，和 fill1 一致
    for (int i = 1; i <= n; i++) printf("%d ", qpow(i, i));
    return 0;
}
