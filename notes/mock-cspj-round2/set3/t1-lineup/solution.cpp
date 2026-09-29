#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, x;
    scanf("%lld %lld", &n, &x);
    // 一轮取走位置 1,4,7,... 共 ceil(t/3) 个人，剩下 2/3 左右 → 轮数是 O(log n) 级
    long long t = n, rounds = 0;
    while (t > 0) {
        rounds++;
        t -= (t + 2) / 3;
    }
    long long p = x, mine = 0;
    while (true) {
        mine++;
        if (p % 3 == 1) break;            // 本轮位置是 1,4,7,... 中的一个，被取走
        p -= (p + 2) / 3;                 // 否则算出自己在下一轮的新位置
    }
    printf("%lld %lld\n", rounds, mine);
    return 0;
}
