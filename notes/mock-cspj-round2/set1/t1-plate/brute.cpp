#include <bits/stdc++.h>
using namespace std;

// 暴力：把数字全排列，跳过前导零，取字典序最小（长度固定，所以字典序即数值序）
int main() {
    string s;
    cin >> s;
    string d;
    for (char c : s) if (isdigit((unsigned char)c)) d += c;
    sort(d.begin(), d.end());
    string best = "";
    do {
        if (d[0] == '0') continue;
        if (best == "" || d < best) best = d;
    } while (next_permutation(d.begin(), d.end()));
    cout << best << "\n";
    return 0;
}
