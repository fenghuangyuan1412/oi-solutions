#include <bits/stdc++.h>
using namespace std;

int cnt[10];

int main() {
    string s;
    cin >> s;
    for (char c : s) {
        if (c >= '0' && c <= '9') cnt[c - '0']++;
    }
    // 首位：最小的非零数字
    int first = -1;
    for (int d = 1; d <= 9; d++) {
        if (cnt[d]) { first = d; break; }
    }
    putchar('0' + first);
    cnt[first]--;
    // 第二位起先放所有的 0，让位数尽量"往后堆"，数才最小
    for (int i = 0; i < cnt[0]; i++) putchar('0');
    for (int d = 1; d <= 9; d++) {
        for (int i = 0; i < cnt[d]; i++) putchar('0' + d);
    }
    putchar('\n');
    return 0;
}
