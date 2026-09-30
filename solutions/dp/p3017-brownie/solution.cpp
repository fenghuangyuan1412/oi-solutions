#include <bits/stdc++.h>
using namespace std;

/*
 * P3017 [USACO11MAR] Brownie Slicing G
 *
 * 二分答案 X + check(X)：
 *   check(X) = "能否先切出 A 条水平带，使得每条带都能（独立地）竖切出至少 B 块巧克力豆数 >= X 的块"
 *   条带内部的竖向计数用贪心（从左往右累加列和，一旦 >= X 立刻切一刀），
 *   带与带之间的划分用"每条带只取最短可行长度"的贪心 —— 它是
 *   状态 f[i] = 前 i 行能切出的最多合格带数 这个 DP 的单调情形（见 README 正确性论证）。
 *
 * 复杂度：单次 check O(R*C)，二分 log2(total/(A*B)) <= 31 次 => O(R*C*log V)
 */

const int MAXN = 505;

int R, C, A, B;
int a[MAXN][MAXN];        // 布朗尼本体，0 <= a[i][j] <= 4000，int 足够
long long colsum[MAXN];   // 当前条带的"列和"：colsum[c] = 条带内第 c 列所有行之和

/* 条带内部：从左到右贪心，能切出多少块和 >= X 的块。
 * 贪心之所以正确（交换论证见 README）：每一块都尽可能早地收尾，
 * 剩下的列就尽可能多，块数不可能比任何其它切法少。
 * 尾部不足 X 的零头会被并入最后一块，所以 cnt 就是"最多块数"。 */
static inline int greedyPieces(long long X) {
  int cnt = 0;
  long long cur = 0;
  for (int c = 1; c <= C; ++c) {
    cur += colsum[c];
    if (cur >= X) {          // 达到 X 立刻收口，绝不"再等等"
      ++cnt;
      cur = 0;
    }
  }
  return cnt;
}

/* check(X)：能不能保证每块都 >= X */
bool check(long long X) {
  int strips = 0;
  int i = 1;
  while (i <= R) {
    // 从第 i 行开始向下扩展，找"最短"的一条能让块数 >= B 的带 [i..end]
    for (int c = 1; c <= C; ++c) colsum[c] = 0;
    int end = -1;
    for (int j = i; j <= R; ++j) {
      for (int c = 1; c <= C; ++c) colsum[c] += a[j][c];
      if (greedyPieces(X) >= B) {   // 第一次满足就停：贪心要把行省给后面的带
        end = j;
        break;
      }
    }
    if (end == -1) return false;      // 连一条合格带都凑不出来，更不用说 A 条
    ++strips;
    i = end + 1;
    if (strips >= A) return true;     // 剩下的行并进最后一条带：列和只增不减，仍合格
  }
  return false;                          // 走到这里说明合格带数 < A
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  if (!(cin >> R >> C >> A >> B)) return 0;
  long long total = 0;
  for (int i = 1; i <= R; ++i)
    for (int j = 1; j <= C; ++j) {
      cin >> a[i][j];
      total += a[i][j];
    }

  // 二分上界：最小块不可能超过平均块，所以 total/(A*B) 是安全上界
  long long lo = 0, hi = total / (1LL * A * B), ans = 0;
  while (lo <= hi) {
    long long mid = (lo + hi) >> 1;
    if (check(mid)) {                  // mid 可行 -> 答案至少这么大，往右上探
      ans = mid;
      lo = mid + 1;
    } else {
      hi = mid - 1;
    }
  }
  cout << ans << '\n';
  return 0;
}
