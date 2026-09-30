// r1_parity.cpp —— 第 18 题：无论 n 取 5..1000 中哪个值，输出的整数是否全是奇数？
// 同时统计每个 n 的输出对数、是否出现过偶数。
#include <iostream>
using namespace std;

bool isPrime(int n) {
    for(int i = 2; i < n; ++i) if(n % i == 0) return false;
    return true;
}

int main() {
    int evenCnt = 0, firstEvenN = -1;
    long long sum50 = 0;
    for (int n = 5; n <= 1000; n++) {
        long long sum = 0; int pairs = 0;
        for (int i = 2; i + 2 <= n; ++i) {
            if (isPrime(i) && isPrime(i + 2)) {
                pairs++;
                sum += i + i + 2;
                if (i % 2 == 0) { evenCnt++; if (firstEvenN < 0) { firstEvenN = n; printf("  出现偶数 %d (n=%d)\n", i, n); } }
                if ((i + 2) % 2 == 0) { evenCnt++; if (firstEvenN < 0) { firstEvenN = n; printf("  出现偶数 %d (n=%d)\n", i + 2, n); } }
            }
        }
        if (n == 14 || n == 50 || n == 1000) printf("  n=%-4d 对数=%-4d 所有整数之和=%lld\n", n, pairs, sum);
        if (n == 50) sum50 = sum;
    }
    printf("第18题: 5<=n<=1000 中输出偶数的次数 = %d  (0 => 命题 T)\n", evenCnt);
    printf("第16题 n=14 之和 / 第19题 n=50 之和 = 见上一行, n=50 时 sum = %lld\n", sum50);
    printf("isPrime(2)=%d isPrime(4)=%d  => (2,4) 这一对不会打印\n", isPrime(2), isPrime(4));
    return 0;
}
