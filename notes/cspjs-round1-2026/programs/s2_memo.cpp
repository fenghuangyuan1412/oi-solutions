#include <iostream>
#include <cstring>
using namespace std;
int memo[20][20];
int calls = 0;
int C(int n, int m) {
    ++calls;
    if (m == 0 || n == m) return 1;
    if (memo[n][m]) return memo[n][m];
    return memo[n][m] = C(n - 1, m) + C(n - 1, m - 1);
}
int main() {
    memset(memo, 0, sizeof memo);
    cout << C(6, 3) << " " << calls << "\n";
    return 0;
}
