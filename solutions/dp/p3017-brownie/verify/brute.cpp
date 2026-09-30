#include <bits/stdc++.h>
using namespace std;
/* 暴力：完全按题意枚举。
 * 1) 枚举 A-1 个水平切点（在 R-1 个行间隙里选）
 * 2) 对每条带单独枚举 B-1 个垂直切点（在 C-1 个列间隙里选）—— 带与带互相独立
 * 3) 每种切法算出 A*B 块的和，取最小值；Bessie 最大化这个最小值
 * 仅用于对拍，R,C 必须很小。
 * 编译：g++ -static -O2 -std=c++14 brute.cpp -o brute.exe
 */
int main(){
    int R,C,A,B; 
    if(!(cin>>R>>C>>A>>B)) return 0;
    vector<vector<int> > a(R+1, vector<int>(C+1));
    for(int i=1;i<=R;i++) for(int j=1;j<=C;j++) cin>>a[i][j];

    long long best = -1;
    // 枚举水平切点集合：从 1..R-1 选 A-1 个
    vector<int> gapH(R);           // 0..R-1 位置索引，用二进制枚举
    int hlim = 1 << (R-1);
    int vlim = 1 << (C-1);
    for(int hm=0; hm<hlim; ++hm){
        vector<int> cuts; 
        for(int k=0;k<R-1;k++) if(hm>>k&1) cuts.push_back(k+1);   // 在第 cuts[k] 行下面切
        if((int)cuts.size()!=A-1) continue;
        // 行段范围：[1..cuts0], [cuts0+1..cuts1], ...
        vector<pair<int,int> > strips;
        int l=1;
        for(size_t t=0;t<cuts.size();++t){ strips.push_back(make_pair(l,cuts[t])); l=cuts[t]+1; }
        strips.push_back(make_pair(l,R));
        // 每条带独立枚举垂直切点，取该带最优（带之间互不影响，故可各自取 max）
        long long worst = (long long)4e18;
        for(int s=0;s<A;++s){
            int r1=strips[s].first, r2=strips[s].second;
            vector<long long> col(C+1,0);
            for(int j=1;j<=C;j++) for(int i=r1;i<=r2;i++) col[j]+=a[i][j];
            long long locBest=-1;
            for(int vm=0; vm<vlim; ++vm){
                int cnt=0;
                for(int k=0;k<C-1;k++) if(vm>>k&1) cnt++;
                if(cnt!=B-1) continue;
                long long cur=0, mn=(long long)4e18;
                for(int j=1;j<=C;j++){
                    cur+=col[j];
                    bool cut=(j<C && (vm>>(j-1)&1));
                    if(cut){ if(cur<mn) mn=cur; cur=0; }
                }
                if(cur<mn) mn=cur;
                if(mn>locBest) locBest=mn;
            }
            if(locBest<worst) worst=locBest;
        }
        if(worst>best) best=worst;
    }
    cout<<best<<"\n";
    return 0;
}
