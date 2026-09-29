// r3_bits.cpp —— 第 29 题 C / 第 17 问的两个小验证
//   1) cur[i] ^= 1 与 cur[i] = 'a' - cur[i] 对 '0' '1' 是否等价（ASCII 层面）
//   2) i = 0 时 s.substr(n-i,i) == t.substr(0,i) 是否恒为真
#include <iostream>
#include <string>
#include <cstdio>
using namespace std;
int main() {
    cout << "--- 1) ASCII 验证 ---\n";
    for (char c : string("01")) {
        char x = c, y = c;
        x ^= 1;
        y = 'a' - y;
        printf("  '%c'(=%d):  ^=1  -> '%c'(%d) ;  'a'-'%c' -> '%c'(%d) ;  相等? %s\n",
               c, (int)c, x, (int)x, c, y, (int)y, (x == y ? "YES" : "NO"));
    }
    printf("  注: '0'=48=0b110000, 48^1=49='1'; '1'=49, 49^1=48='0'; 'a'=97, 97-48=49, 97-49=48\n");
    // 只有 '0'/'1' 两个字符会出现在 t 里吗？t 初值全是 '0'，每次只翻转一位
    cout << "\n--- 2) i=0 时 substr 条件 ---\n";
    string s = "00000001", t = "000000000";
    int n = s.size();
    printf("  s.size()=%d, s.substr(n-0,0) = \"%s\" (长度%zu)\n", n, s.substr(n - 0, 0).c_str(), s.substr(n, 0).size());
    printf("  t.substr(0,0)          = \"%s\" (长度%zu)\n", t.substr(0, 0).c_str(), t.substr(0, 0).size());
    cout << "  -> 两个空串相等，条件恒为真，所以第 0 位永远允许翻（只要 i != lst 不拦它）\n";
    int bad = 0;
    for (int mask = 0; mask < (1 << 9); mask++) {
        string tt;
        for (int i = 0; i < 9; i++) tt += char('0' + ((mask >> i) & 1));
        if (!(s.substr(n, 0) == tt.substr(0, 0))) bad++;
    }
    cout << "  穷举 2^9 个 t 验证 i=0 条件为假的个数 = " << bad << "\n";
    return 0;
}
