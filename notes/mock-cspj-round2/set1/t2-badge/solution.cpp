#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    long long x;
    scanf("%d %d %lld", &n, &m, &x);
    long long rank = 1;  // 喜爱度比 x 大的个数 + 1，就是 x 上墙的次序
    for (int i = 1, tot = n * m; i <= tot; i++) {
        long long a;
        scanf("%lld", &a);
        if (a > x) rank++;
    }
    int top = 1, bot = n, lft = 1, rgt = m, step = 0;
    while (top <= bot && lft <= rgt) {
        for (int j = lft; j <= rgt; j++) if (++step == rank) { printf("%d %d\n", top, j); return 0; }
        top++;
        for (int i = top; i <= bot; i++) if (++step == rank) { printf("%d %d\n", i, rgt); return 0; }
        rgt--;
        if (top <= bot) {
            for (int j = rgt; j >= lft; j--) if (++step == rank) { printf("%d %d\n", bot, j); return 0; }
            bot--;
        }
        if (lft <= rgt) {
            for (int i = bot; i >= top; i--) if (++step == rank) { printf("%d %d\n", i, lft); return 0; }
            lft++;
        }
    }
    return 0;
}
