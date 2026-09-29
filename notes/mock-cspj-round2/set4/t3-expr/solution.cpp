#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll MOD = 998244353;
vector<ll> val;      // 数字栈
vector<char> op;     // 运算符栈

int prec(char c) {
    return c == '+' ? 1 : 2;
}

void apply() {
    ll b = val.back();
    val.pop_back();
    ll a = val.back();
    val.pop_back();
    char c = op.back();
    op.pop_back();
    if (c == '+') val.push_back((a + b) % MOD);
    else val.push_back(a * b % MOD);
}

int main() {
    string s;
    cin >> s;
    int depth = 0, now = 0;
    for (int i = 0; i < (int)s.size();) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            ll x = 0;
            while (i < (int)s.size() && isdigit((unsigned char)s[i])) {
                x = (x * 10 + (s[i] - '0')) % MOD;  // 数字可能很长，边读边取模
                i++;
            }
            val.push_back(x);
            continue;
        }
        if (c == '(') {
            op.push_back(c);
            now++;
            depth = max(depth, now);
        } else if (c == ')') {
            while (op.back() != '(') apply();
            op.pop_back();
            now--;
        } else {  // '+' 或 '*'：栈顶优先级不低于我就先算掉
            while (!op.empty() && op.back() != '(' && prec(op.back()) >= prec(c)) apply();
            op.push_back(c);
        }
        i++;
    }
    while (!op.empty()) apply();
    printf("%lld\n%d\n", val.back(), depth);
    return 0;
}
