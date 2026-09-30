// r2_23.cpp -- 题 23：len[i-1]&1 与 i%3<2 是否等价
// 打印 len 数组、len[i]%2 序列、(i%3<2) 序列，并检查 init() 里 i=2..86 全范围
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll len[90];
int main() {
    len[0] = 1, len[1] = 2;
    for (int i = 2; i <= 86; i++) len[i] = len[i - 1] + len[i - 2];
    printf("i in [2,86] parity table (30 columns):\n");
    printf("idx        i :"); for (int i = 0; i < 30; i++) printf("%4d", i); printf("\n");
    printf("     len[i] :"); for (int i = 0; i < 30; i++) printf("%4lld", len[i]); printf("\n");
    printf(" len[i]%2   :"); for (int i = 0; i < 30; i++) printf("%4d", (int)(len[i] & 1)); printf("\n");
    printf(" i%3<2     :"); for (int i = 0; i < 30; i++) printf("%4d", i % 3 < 2); printf("\n");
    printf("(第11行) i    :"); for (int i = 2; i < 32; i++) printf("%4d", i); printf("\n");
    printf(" len[i-1]&1 :"); for (int i = 2; i < 32; i++) printf("%4d", (int)(len[i - 1] & 1)); printf("\n");
    printf(" i%3<2      :"); for (int i = 2; i < 32; i++) printf("%4d", i % 3 < 2); printf("\n");
    int bad = 0;
    for (int i = 2; i <= 86; i++) if ((int)(len[i - 1] & 1) != (i % 3 < 2)) bad++;
    printf("i in [2,86]: 不相等的个数 = %d\n", bad);
    int bad2 = 0;
    for (int i = 0; i <= 86; i++) if ((int)(len[i] & 1) != (i % 3 != 1)) bad2++;
    printf("len[i] 为奇 <=> i%%3!=1 ：不相等的个数 = %d\n", bad2);
    printf("len[] 奇偶周期检查: len[i]%%2 序列前 24 项 = ");
    for (int i = 0; i < 24; i++) printf("%d", (int)(len[i] & 1));
    printf("\n");
    printf("len[86]=%lld (需要 <= 1e18)\n", len[86]);
    return 0;
}
