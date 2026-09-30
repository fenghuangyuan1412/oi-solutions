#include <bits/stdc++.h>
using namespace std;

// ============ 满分版：有理根 + 无理根（根式化简）全都要正确输出 ============
// 大根公式：x = (-b + sqrt(D)) / (2a)，但 a 可能是负数，除以负数会改变大小方向。
// 统一写成  x = q1 + q2 * sqrt(r)，其中 q2 > 0、r 是无平方因子的正整数：
//   q1 = -b / (2a)                        （分母带符号，最后归一到分母为正）
//   q2 = s / (2 * |a|)  （s = 从 D 里提出的最大平方根因子，D = s^2 * r）
// 为什么 q2 一定正：a>0 时大根取 +sqrt(D)，a<0 时取 -sqrt(D)，两种情况合起来正好是 +s/(2|a|)*sqrt(r)。

long long gN, gD;   // 一个分数，约定 gD > 0

// 把 p/q 约分到最简，写进全局的 gN/gD；q 为负时把负号挪到分子
void makeRational(long long p, long long q) {
    if (q < 0) { p = -p; q = -q; }
    long long g = __gcd(llabs(p), q);   // p == 0 时 g == q，约完恰好是 0/1，符合题意
    gN = p / g;
    gD = q / g;
}

// 有理数输出格式：分母为 1 就只输出分子，否则输出 p/q
void printRational(long long p, long long q) {
    makeRational(p, q);
    if (gD == 1) cout << gN;
    else cout << gN << '/' << gD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    long long M;
    cin >> T >> M;
    while (T--) {
        long long a, b, c;
        cin >> a >> b >> c;

        long long D = b * b - 4 * a * c;         // |系数| <= 1000 => |D| <= 5e6，int 都够，用 long long 更稳
        if (D < 0) { cout << "NO\n"; continue; }

        // 先求 floor(sqrt(D))，不用 sqrt 的结果直接用，要靠两次修正消掉浮点误差
        long long t = (long long)(sqrt((long double)D) + 0.5L);
        while (t * t > D) --t;
        while ((t + 1) * (t + 1) <= D) ++t;

        if (t * t == D) {                        // D 是完全平方数 => 根是有理数
            long long num = -b + (a > 0 ? t : -t);
            printRational(num, 2 * a);
            cout << '\n';
            continue;
        }

        // 无理根：把 D 拆成 s^2 * r，r 不含平方因子
        long long s = 1, r = D;
        for (long long d = 2; d * d <= r; ++d) { // r 会越除越小，退出时 r 必然无平方因子
            while (r % (d * d) == 0) { r /= d * d; s *= d; }
        }
        // 此时 r > 1（否则 D 就是完全平方数了）

        makeRational(-b, 2 * a);                 // q1
        long long q1n = gN, q1d = gD;
        makeRational(s, 2 * llabs(a));           // q2，必定为正
        long long q2n = gN, q2d = gD;

        if (q1n != 0) {                          // 第 1 步：q1 != 0 才输出 q1 和加号
            if (q1d == 1) cout << q1n;
            else cout << q1n << '/' << q1d;
            cout << '+';
        }
        // 第 2 步：按 q2 的形状分四种写法（q2n / q2d 已最简，q2n > 0）
        if (q2d == 1) {
            if (q2n == 1) cout << "sqrt(" << r << ')';                  // q2 = 1
            else cout << q2n << "*sqrt(" << r << ')';                   // q2 是整数
        } else {
            if (q2n == 1) cout << "sqrt(" << r << ")/" << q2d;          // 1/q2 是整数
            else cout << q2n << "*sqrt(" << r << ")/" << q2d;           // 一般 c*sqrt(r)/d
        }
        cout << '\n';
    }
    return 0;
}
