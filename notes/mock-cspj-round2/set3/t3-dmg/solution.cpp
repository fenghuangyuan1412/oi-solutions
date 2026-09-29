#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

ll absll(ll x) { return x < 0 ? -x : x; }
ll gcdll(ll x, ll y) {
    x = absll(x);
    y = absll(y);
    while (y) {
        ll t = x % y;
        x = y;
        y = t;
    }
    return x;
}

ll isqrt(ll x) {
    if (x < 0) return -1;
    ll r = (ll)sqrt((long double)x);
    while (r * r > x) r--;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

string fmt(ll P, ll Q, ll r, ll S) {
    string root = (Q == 1 ? "" : to_string(Q)) + "√" + to_string(r);
    if (P == 0) return S == 1 ? root : root + "/" + to_string(S);
    string body = to_string(P) + "+" + root;
    return S == 1 ? body : "(" + body + ")/" + to_string(S);
}

// 正规解法：判别式 → 提取平方因子 → 三个量一起约分
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        ll a, b, c;
        scanf("%lld %lld %lld", &a, &b, &c);
        ll D = b * b - 4 * a * c;
        if (D < 0) {
            puts("NO");
            continue;
        }
        ll g = isqrt(D);
        if (g * g == D) {  // 有理根
            ll N = -b + (a > 0 ? g : -g);  // a < 0 时"较大根"取减号那一支
            ll Den = 2 * a;
            if (Den < 0) {
                Den = -Den;
                N = -N;
            }
            ll h = gcdll(N, Den);
            N /= h;
            Den /= h;
            if (Den == 1) printf("%lld\n", N);
            else printf("%lld/%lld\n", N, Den);
            continue;
        }
        ll k = 1, r = D;  // √D = k·√r，r 无平方因子
        for (ll t = 2; t * t <= r; t++) {
            while (r % (t * t) == 0) {
                r /= t * t;
                k *= t;
            }
        }
        ll A = absll(2 * a), P = (a > 0 ? -b : b), Q = k;
        ll h = gcdll(gcdll(P, Q), A);
        P /= h;
        Q /= h;
        ll S = A / h;
        string out = fmt(P, Q, r, S);
        printf("%s\n", out.c_str());
    }
    return 0;
}
