#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll MOD = 998244353;
string s;
int pos = 0;

ll parseExpr();

ll parseFactor() {
    if (s[pos] == '(') {
        pos++;
        ll v = parseExpr();
        pos++;  // 跳过 ')'
        return v;
    }
    ll x = 0;
    while (pos < (int)s.size() && isdigit((unsigned char)s[pos])) {
        x = (x * 10 + (s[pos] - '0')) % MOD;
        pos++;
    }
    return x;
}

ll parseTerm() {  // 乘法层
    ll v = parseFactor();
    while (pos < (int)s.size() && s[pos] == '*') {
        pos++;
        v = v * parseFactor() % MOD;
    }
    return v;
}

ll parseExpr() {  // 加法层
    ll v = parseTerm();
    while (pos < (int)s.size() && s[pos] == '+') {
        pos++;
        v = (v + parseTerm()) % MOD;
    }
    return v;
}

// 暴力：递归下降，靠"函数调用栈"天然实现括号与优先级
int main() {
    cin >> s;
    int depth = 0, now = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == '(') {
            now++;
            depth = max(depth, now);
        } else if (s[i] == ')') now--;
    }
    printf("%lld\n%d\n", parseExpr(), depth);
    return 0;
}
