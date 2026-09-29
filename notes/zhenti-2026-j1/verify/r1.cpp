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
    for(int i = 2; i + 2 <= n; ++i) {
        if(isPrime(i) && isPrime(i + 2)) {
            cout << i << " " << i + 2 << endl;
        }
    }
    return 0;
}
