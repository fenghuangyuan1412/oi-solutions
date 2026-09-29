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

string fmt(ll P, ll Q, ll r, ll S) {
    string root = (Q == 1 ? "" : to_string(Q)) + "√" + to_string(r);
    if (P == 0) return S == 1 ? root : root + "/" + to_string(S);
    string body = to_string(P) + "+" + root;
    return S == 1 ? body : "(" + body + ")/" + to_string(S);
}

bool squarefree(ll x) {
    for (ll t = 2; t * t <= x; t++) if (x % (t * t) == 0) return false;
    return true;
}

// 完全另一条路：不去解方程，而是"猜"出答案 (p,q,r,s)，再用整数恒等式反验。
// x = (p+q√r)/s 是 at²+bt+c=0 的根 <=> 2ap+bs=0 且 a(p²+q²r)+bps+cs²=0
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
        long double V = (a > 0) ? ((-b + sqrtl((long double)D)) / (2.0L * a))
                                : ((-b - sqrtl((long double)D)) / (2.0L * a));
        bool done = false;
        for (ll q = 1; q <= 2000 && !done; q++) {  // 先试有理数 p/q
            ll p = (ll)llround(V * q);
            if (fabsl((long double)p / (long double)q - V) > 1e-8L) continue;  // 必须是"较大"那个根
            if (a * p * p + b * p * q + c * q * q == 0) {
                ll g = gcdll(p, q);
                p /= g;
                q /= g;
                if (q == 1) printf("%lld\n", p);
                else printf("%lld/%lld\n", p, q);
                done = true;
            }
        }
        if (done) continue;
        ll bp = 0, bq = 0, br = 0, bs = 0;
        bool found = false;
        for (ll s = 1; s <= 2000; s++) {
            ll num = -b * s, den = 2 * a;
            if (num % den != 0) continue;
            ll p = num / den;
            ll W = -(a * p * p + b * p * s + c * s * s);
            if (W % a != 0) continue;
            ll q2r = W / a;  // = q²·r
            if (q2r <= 0) continue;
            for (ll r = 2; r <= 5000; r++) {
                if (!squarefree(r) || q2r % r != 0) continue;
                ll t2 = q2r / r;
                ll q = (ll)sqrtl((long double)t2);
                while (q * q > t2) q--;
                while ((q + 1) * (q + 1) <= t2) q++;
                if (q * q != t2 || q <= 0) continue;
                long double val = ((long double)p + (long double)q * sqrtl((long double)r)) / (long double)s;
                if (fabsl(val - V) > 1e-7L) continue;
                if (!found || s < bs || (s == bs && r < br) || (s == bs && r == br && q < bq)) {
                    found = true;
                    bp = p;
                    bq = q;
                    br = r;
                    bs = s;
                }
            }
        }
        if (!found) {
            puts("BUG");
            continue;
        }
        string out = fmt(bp, bq, br, bs);
        printf("%s\n", out.c_str());
    }
    return 0;
}
