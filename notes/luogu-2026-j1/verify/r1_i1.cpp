// r1_i1.cpp —— 第 17 题：把第 16 行的 int i = 2 改成 int i = 1，其余逐字不变。
// 另外多打一行 sum 便于统计。
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
    for(int i = 1; i + 2 <= n; ++i) {          // <== 唯一改动：int i = 1
        if(isPrime(i) && isPrime(i + 2)) {
            cout << i << " " << i + 2 << endl;
            sum += i + (i + 2);
        }
    }
    cout << "[sum]=" << sum << endl;
    return 0;
}
