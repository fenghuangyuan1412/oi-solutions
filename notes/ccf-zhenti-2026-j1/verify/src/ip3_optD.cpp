// 题 32：把卷面 (3) 的输出全收集起来，逐条检验 D 选项（删掉末位后还是不是质数），
// 顺便记录 A/B/C 的反例。DFS 逻辑与卷面第 3—23 行一致。
#include <cstdio>
#include <set>
#include <vector>
using namespace std;

bool check_prime(int x) {
  if (x <= 1) return false;
  for (int i = 2; i * i <= x; i++) if (x % i == 0) return false;
  return true;
}
int n;
vector<int> out;
void search_result(int x) {
  if (!check_prime(x)) return;
  if (x >= n) { out.push_back(x); return; }
  for (int i = 0; i <= 9; i++) search_result(x * 10 + i);
}

int main() {
  long long rows_prev = -1;
  int mono_bad = 0;
  for (n = 2; n <= 3000; n++) {
    out.clear();
    for (int i = 1; i <= 9; i++) search_result(i);
    // D：每个 >=10 的输出，删掉末位后必须是质数
    for (int v : out)
      if (v >= 10) {
        int head = v / 10;
        if (!check_prime(head)) { printf("题32-D 反例 n=%d 输出 %d，删末位得 %d 不是质数\n", n, v, head); return 1; }
      }
    // C：末位是否只出现 3/7
    // A：是否升序
    for (size_t i = 1; i < out.size(); i++)
      if (out[i] < out[i - 1]) { if (mono_bad == 0) printf("题32-A 反例 n=%d：%d 后面跟着更小的 %d\n", n, out[i-1], out[i]); break; }
    if (rows_prev >= 0 && (long long)out.size() > rows_prev) {
      if (mono_bad == 0) printf("题32-B 反例：n=%d 时 %lld 行，n=%d 时 %d 行（变大了）\n", n - 1, rows_prev, n, (int)out.size());
      mono_bad++;
    }
    rows_prev = out.size();
    for (int v : out) if (v >= 10 && v % 10 != 3 && v % 10 != 7) { if (mono_bad >= 0) {} break; }
  }
  printf("题32-D  n=2..3000 全部输出逐个检验：删末位后仍是质数，反例 0 个\n");
  // C 的反例单独给一个具体输入
  n = 24; out.clear();
  for (int i = 1; i <= 9; i++) search_result(i);
  printf("题32-C 反例：n=24 的输出里有");
  for (int v : out) if (v % 10 != 3 && v % 10 != 7) { printf(" %d", v); }
  printf("（末位不是 3 也不是 7）\n");
  return 0;
}
