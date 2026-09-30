#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
int a[100007], b[100007], c[100007], carry[100007];
string input_str;
int a_len, b_len;
int main() {
  cin >> input_str;
  a_len = input_str.size();
  for (int i = 0; i < a_len; i++) {
    a[i] = input_str[a_len - i - 1] - '0';
  }
  cin >> input_str;
  b_len = input_str.size();
  for (int i = 0; i < b_len; i++) {
    b[i] = input_str[b_len - i - 1] - '0';
  }
  carry[0] = 0;
  for (int i = 0; i < max(a_len, b_len) + 1; i++) {
    c[i] = a[i] + b[i] + carry[i];
    if (c[i] >= 10) {
      carry[i + 1] = 1;
      c[i] -= 10;
    } else {
      carry[i + 1] = 0;
    }
  }
  for (int i = max(a_len, b_len); i >= 0; i--) {
    cout << c[i];
  }
  cout << endl;
  return 0;
}
