// isprime_variants.cpp —— 第 20 题：把第 5 行 i < n 换成 4 种写法，逐个对照
// 1) isPrime(9) / isPrime(25) / isPrime(49) 在 5 种写法下的返回值
// 2) 每种写法与原写法第一个产生分歧的整数
// 3) 跑完整"孪生素数"程序，n 从 5 到 1000，看主程序输出是否发生变化
#include <iostream>
#include <sstream>
using namespace std;

bool O    (int n) { for (int i = 2; i <  n;     ++i) if (n % i == 0) return false; return true; } // 原题
bool A    (int n) { for (int i = 2; i <= n;     ++i) if (n % i == 0) return false; return true; } // A. i <= n
bool B    (int n) { for (int i = 2; i * i <  n; ++i) if (n % i == 0) return false; return true; } // B. i*i < n
bool C    (int n) { for (int i = 2; i * i <= n; ++i) if (n % i == 0) return false; return true; } // C. i*i <= n
bool D    (int n) { for (int i = 2; i*i*i < n;  ++i) if (n % i == 0) return false; return true; } // D. i*i*i < n

// 把整份程序（第 16~20 行）跑一遍，输出压成一个字符串
template <bool (*f)(int)>
string run(int n) {
    ostringstream os;
    for (int i = 2; i + 2 <= n; ++i)
        if (f(i) && f(i + 2)) os << i << " " << i + 2 << "\n";
    return os.str();
}

template <bool (*f)(int)>
void report(const char* name, int LIM) {
    cout << "--- " << name << " ---\n";
    cout << "  isPrime(1)=" << f(1) << " isPrime(2)=" << f(2)
         << " isPrime(9)=" << f(9) << " isPrime(25)=" << f(25)
         << " isPrime(49)=" << f(49) << " isPrime(2)=" << f(2) << "\n";
    int first = -1;
    for (int m = 1; m <= 2000; ++m) if (f(m) != O(m)) { first = m; break; }
    if (first < 0) cout << "  与原版在 1..2000 内完全一致\n";
    else cout << "  与原版第一个不同的整数 m = " << first << " (原版=" << O(first)
              << ", 改版=" << f(first) << ")\n";
    cout << "  1..1000 内所有与原版判定不同的整数: ";
    for (int m = 1; m <= 1000; ++m) if (f(m) != O(m)) cout << m << " ";
    cout << "\n";
    int firstN = -1, diffCnt = 0;
    for (int n = 5; n <= LIM; ++n) {
        if (run<f>(n) != run<O>(n)) { if (firstN < 0) firstN = n; diffCnt++; }
    }
    cout << "  主程序输出(5<=n<=" << LIM << ")不同的 n 个数 = " << diffCnt
         << ", 第一个这样的 n = " << firstN << "\n";
}

int main() {
    cout << "对照表（true=1, false=0）\n";
    cout << "x      原(i<n)  A(i<=n)  B(i*i<n)  C(i*i<=n)  D(i*i*i<n)\n";
    int xs[] = {1, 2, 3, 4, 8, 9, 25, 27, 49, 64, 121, 169, 361, 529, 841, 961};
    for (int x : xs)
        cout << x << "\t" << O(x) << "\t" << A(x) << "\t\t" << B(x)
             << "\t\t" << C(x) << "\t\t" << D(x) << "\n";
    cout << "\n";
    report<O>("原版 O: i < n", 1000);
    report<A>("A: i <= n", 1000);
    report<B>("B: i*i < n", 1000);
    report<C>("C: i*i <= n", 1000);
    report<D>("D: i*i*i < n", 1000);
    return 0;
}
