// 题 18 / 题 21：把卷面 (1) 的循环体原样搬进函数，对 0..2^31-1 逐个跑。
// 为了能在几秒内跑完 21 亿个数，用 __builtin_popcount 先证明它和卷面循环等价（见下面 verify 段），
// 等价性在 0..2^24 上逐个核对；核对通过后才在全范围用等价式统计。
#include <cstdio>
#include <cstring>
using namespace std;

void paper(int n, int& x, int& y) {  // 卷面第 6—15 行，一字不改
  x = 1; y = 1;
  while (n > 0) {
    if (n % 2 == 0) { ++x; } else { ++x; ++y; }
    n = n / 2;
  }
}
int bitlen(int n) { int b = 0; while (n > 0) { b++; n >>= 1; } return b; }

int main() {
  // 1) 等价性核对：x = 位数+1（n=0 时 1），y = popcount+1
  const int LIM = 1 << 24;
  int bad = 0;
  for (int n = 0; n < LIM; n++) {
    int x, y;
    paper(n, x, y);
    int ex = (n == 0) ? 1 : bitlen(n) + 1;
    int ey = __builtin_popcount((unsigned)n) + 1;
    if (x != ex || y != ey) { if (bad < 5) printf("不等价: n=%d 卷面(%d,%d) 公式(%d,%d)\n", n, x, y, ex, ey); bad++; }
  }
  printf("等价性核对 0..%d：不一致 %d 个\n", LIM - 1, bad);

  // 2) 题 18：第一个数是否总 >= 第二个数（在 0..2^24 上逐个看）
  int viol18 = 0;
  for (int n = 0; n < LIM; n++) { int x, y; paper(n, x, y); if (x < y) { if (viol18 < 5) printf("题18反例 n=%d 输出 %d %d\n", n, x, y); viol18++; } }
  printf("题18  0..%d 中 x<y 的反例 %d 个\n", LIM - 1, viol18);

  // 3) 题 21：0..2^31-1 中第二个数恰好为 2 的次数
  long long cnt = 0;
  for (long long n = 0; n < (1LL << 31); n++)
    if (__builtin_popcount((unsigned)n) + 1 == 2) cnt++;
  printf("题21  0..2^31-1 中 y==2 共 %lld 次\n", cnt);

  // 4) 题 20：n=6
  int x, y; paper(6, x, y);
  printf("题20  n=6 -> %d %d\n", x, y);
  return 0;
}
