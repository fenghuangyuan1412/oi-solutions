// r1_sum.cpp  —— 与 r1.cpp 逻辑完全相同，只是在末尾多输出"所有整数之和"，
// 方便核对第 16 / 19 题。用法: echo 14 | ./r1_sum.exe
#include <iostream>
using namespace std;

bool isPrime(int n) {
    for(int i = 2; i < n; ++i) {
        if(n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    cin >> n;
    long long sum = 0;
    int cnt = 0;
    for(int i = 2; i + 2 <= n; ++i) {
        if(isPrime(i) && isPrime(i + 2)) {
            cout << i << " " << i + 2 << endl;
            sum += i + (i + 2);
            cnt++;
        }
    }
    cout << "[pairs]=" << cnt << " [sum]=" << sum << endl;
    return 0;
}
