#include <bits/stdc++.h>
using namespace std;

const int N = 105;
int n, t[N];
int up[N], down[N];

// P1091 合唱队形：选出先升后降（严格，且峰只能一个人）的最长序列，答案 = n - 最长
// 关键转化：枚举"中间那位最高的同学" k，
//   左边要能以 t[k] 结尾取到最长上升 ⇒ up[k]；右边要能以 t[k] 开头取到最长下降 ⇒ down[k]。
//   k 被两边共用，所以长度是 up[k] + down[k] - 1。
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%d", &t[i]);

    for (int i = 1; i <= n; i++) {                       // up[i]：以 i 结尾的 LIS 长度
        up[i] = 1;
        for (int j = 1; j < i; j++)
            if (t[j] < t[i]) up[i] = max(up[i], up[j] + 1);
    }
    for (int i = n; i >= 1; i--) {                       // down[i]：以 i 开头的 LDS 长度（倒着看就是 LIS）
        down[i] = 1;
        for (int j = n; j > i; j--)
            if (t[j] < t[i]) down[i] = max(down[i], down[j] + 1);
    }

    int best = 0;
    for (int k = 1; k <= n; k++) best = max(best, up[k] + down[k] - 1);
    printf("%d\n", n - best);
    return 0;
}
