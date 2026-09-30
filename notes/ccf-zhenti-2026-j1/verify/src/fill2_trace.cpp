// 完善程序 (2) 平衡分割：把递归过程逐帧打印出来的版本，供 c-split-avg.html 和 README 的手推表用。
// 5 个空按整理者给出的参考答案 B D C A D 填好；结尾 return 0; } 是语法必需的最小补全（卷面第 9 页缺失）。
// 与 src/fill2_tmpl.cpp 的差异只有两处：split 多一个 depth 参数（算缩进用），以及插入 cout 打印。
#include <algorithm>
#include <iomanip>
#include <iostream>
using namespace std;
constexpr int N = 25;
int n, a[N]; char s[N];
double ans = 1e100;
int value(char c) { return c - (c < 'A' ? '0' : 'A' - 10); }
void split(int l, int cnt, double mnb, double mxb, int depth) {
  string ind(depth * 2, ' ');
  cout << ind << "进入 split(l=" << l << ", cnt=" << cnt << ", mnb=" << mnb << ", mxb=" << mxb << ")" << endl;
  if (l > n) {
    if (cnt == 0) { cout << ind << "  cnt==0 -> 一刀没切，题面禁止，return" << endl; return; }
    cout << ind << "  收尾：极差 = " << mxb - mnb << "，ans = min(ans, 极差) = " << min(ans, mxb - mnb) << endl;
    ans = min(ans, mxb - mnb);
    return;
  }
  int sum = 0;
  for (int r = l; r <= n; ++r) {
    sum += a[r];
    double nwb = sum * 1.0 / (r - l + 1);
    cout << ind << "  本段 [" << l << "," << r << "] sum=" << sum << " 段长=" << (r - l + 1) << " 平均=" << nwb << endl;
    split(r + 1, cnt + (r < n), min(mnb, nwb), max(mxb, nwb), depth + 1);
  }
}
int main() {
  cin >> n >> s + 1;
  for (int i = 1; i <= n; ++i) a[i] = value(s[i]);
  cout << "a[1.." << n << "] =";
  for (int i = 1; i <= n; ++i) cout << ' ' << a[i];
  cout << endl;
  split(1, 0, 1e100, -1e100, 0);
  cout << "答案 = " << fixed << setprecision(6) << ans << endl;
  return 0;
}
