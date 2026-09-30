#include <bits/stdc++.h>
using namespace std;

// ============ 骗分版：只保证"有理根"这一档 ============
// 思路：Δ<0 输出 NO；Δ 是完全平方数时根一定是有理数，用整数分数精确算出来并约分。
// 这两步是**有正确性保证**的，能拿下：
//   测试点 1（M<=1）、3、5（特殊性质 A/B）、6（c=0，根为 -b/a 的分数）、7、8（特殊性质 C，整数根）
// 因为只要两个根是有理数，Δ 就一定是完全平方数，上面这些点全部落在"完全平方"分支里。
// Δ 不是完全平方数（无理根，要输出 sqrt 形式）时，本程序**没有能力正确处理**，
// 只能拿 double 算一个值四舍五入成整数蒙一下：考场里不要指望它得分，它的作用只是"别输出空行 RE"。
// 预计档位：约 60 分（满分需要 solution_full.cpp 的根式化简）。

// 分数格式化：value = p / q，要求 q > 0，gcd(|p|, q) = 1
// q == 1 输出 "{p}"，否则输出 "{p}/{q}"（负号只在分子上）
void printRational(long long p, long long q) {
    if (q < 0) { p = -p; q = -q; }                 // 保证分母为正
    long long g = __gcd(llabs(p), q);              // p = 0 时 g = q，约完正好是 0/1
    p /= g; q /= g;
    if (q == 1) cout << p;
    else cout << p << '/' << q;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    long long M;
    cin >> T >> M;                     // M 本题用不上，只是必须读掉
    while (T--) {
        long long a, b, c;
        cin >> a >> b >> c;

        long long D = b * b - 4 * a * c;           // |系数| <= 1000，D 在 long long 里很安全
        if (D < 0) { cout << "NO\n"; continue; }   // 无实数解

        long long t = (long long)(sqrt((long double)D) + 0.5L);   // 四舍五入猜测整数平方根
        while (t * t > D) --t;                     // 浮点误差可能猜大，往下修
        while ((t + 1) * (t + 1) <= D) ++t;        // 猜小了就往上修，保证 t = floor(sqrt(D))

        if (t * t == D) {
            // Δ 是完全平方数 => 两个根都是有理数，取较大者
            // a > 0 时大根是 (-b+t)/(2a)；a < 0 时除以负数会翻方向，大根是 (-b-t)/(2a)
            long long num = -b + (a > 0 ? t : -t);
            printRational(num, 2 * a);
            cout << '\n';
            continue;
        }

        // ---- 以下这段是"骗"，不是"分"：无理根处理不了 ----
        // 用 double 直接求较大根，再四舍五入成整数。
        // 只有当正确答案恰好是个整数时才碰巧对（而那种情况 Δ 本来就是完全平方数，轮不到这里）。
        double x1 = (-b + sqrt((double)D)) / (2.0 * a);
        double x2 = (-b - sqrt((double)D)) / (2.0 * a);
        cout << (long long)llround(max(x1, x2)) << '\n';
    }
    return 0;
}
