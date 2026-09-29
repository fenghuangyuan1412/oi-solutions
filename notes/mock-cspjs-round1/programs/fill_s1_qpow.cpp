#include <iostream>
using namespace std;

long long qpow(long long a, long long b, long long mod) {
    long long res = 1;                    // 空①
    a %= mod;
    while (b > 0) {
        if (b & 1)                        // 空②
            res = res * a % mod;
        a = a * a % mod;                  // 空③
        b = b >> 1;                       // 空④
    }
    return res;                           // 空⑤
}

int main() {
    cout << qpow(3, 13, 1000000007LL) << "\n";
    cout << qpow(2, 10, 1000000007LL) << "\n";
    return 0;
}
