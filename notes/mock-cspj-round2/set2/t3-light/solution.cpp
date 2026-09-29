#include <bits/stdc++.h>
using namespace std;

int cost[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

int main() {
    int n;
    scanf("%d", &n);
    int L = (n + 6) / 7;  // 一位最多花 7 颗，所以至少要 L 位
    if (2 * L > n) {      // 一位最少花 2 颗；只有 n = 1 会卡在这里
        puts("-1");
        return 0;
    }
    int rem = n;
    for (int pos = 0; pos < L; pos++) {
        int left = L - pos - 1;  // 后面还剩几个位置
        for (int d = (pos == 0 ? 1 : 0); d <= 9; d++) {
            int r2 = rem - cost[d];
            if (r2 >= 2 * left && r2 <= 7 * left) {  // 剩下的 LED 还能不能填满后面的位
                putchar('0' + d);
                rem = r2;
                break;
            }
        }
    }
    putchar('\n');
    return 0;
}
