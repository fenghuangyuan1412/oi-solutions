// r1_divcount.cpp —— 统计阅读程序(1) 里 isPrime 的试除次数
// 对应 j1-prime.html 的"试除次数统计"面板。
// 编译： g++ -static -O2 -std=c++14 r1_divcount.cpp -o r1_divcount.exe
#include <bits/stdc++.h>
using namespace std;
long long divs;

bool isPrime(int n) {          // 与 r1.cpp 完全一致，只多了一行计数
    for (int i = 2; i < n; ++i) {
        divs++;
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 20;
    long long real = 0, naive = 0, calls = 0, skipped = 0;

    for (int i = 2; i + 2 <= n; ++i) {
        divs = 0; bool p1 = isPrime(i); real += divs; calls++;
        if (p1) { divs = 0; isPrime(i + 2); real += divs; calls++; }
        else skipped++;                       // && 短路，第二个调用没发生
    }
    for (int i = 2; i + 2 <= n; ++i) {        // 假想没有短路：两个都算
        divs = 0; isPrime(i); naive += divs;
        divs = 0; isPrime(i + 2); naive += divs;
    }
    cout << "n=" << n << " 实际试除 " << real << " 次, 不短路需要 " << naive
         << " 次, 短路省下 " << naive - real << " 次, isPrime 被调用 " << calls
         << " 次, 被短路跳过 " << skipped << " 次" << endl;
    return 0;
}
