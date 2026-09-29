// ip1.cpp —— 只有三行核心逻辑：验证 isPrime(1) 的返回值
#include <iostream>
using namespace std;
bool isPrime(int n) { for (int i = 2; i < n; ++i) if (n % i == 0) return false; return true; }
int main() {
    cout << "isPrime(1) = " << boolalpha << isPrime(1) << "  (numeric: " << isPrime(1) << ")" << endl;
    cout << "isPrime(0) = " << isPrime(0) << endl;
    cout << "isPrime(2) = " << isPrime(2) << endl;
    cout << "isPrime(3) = " << isPrime(3) << endl;
    return 0;
}
