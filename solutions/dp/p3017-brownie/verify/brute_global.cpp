#include <bits/stdc++.h>
using namespace std;
/* 反例暴力（仅验证用，非讲解代码）：把题意误读成"全局竖切"的版本。
 * 区别只在：brute.cpp 里每条水平带各自枚举 B-1 个垂直切点（题面说的 independently），
 * 这份代码改成整块布朗尼共用同一套垂直切点。
 * 用途：给 README"易错点"提供可复现的实测数字 —— 同一组数据两者的答案可以不同。
 * 编译：g++ -static -O2 -std=c++14 brute_global.cpp -o bglobal.exe
 */
int main(){
    int R,C,A,B;
    if(!(cin>>R>>C>>A>>B)) return 0;
    vector<vector<int> > a(R+1, vector<int>(C+1));
    for(int i=1;i<=R;i++) for(int j=1;j<=C;j++) cin>>a[i][j];

    long long best=-1;
    int hlim = 1 << (R-1), vlim = 1 << (C-1);
    for(int hm=0; hm<hlim; ++hm){
        vector<int> cuts;
        for(int k=0;k<R-1;k++) if(hm>>k&1) cuts.push_back(k+1);
        if((int)cuts.size()!=A-1) continue;
        vector<pair<int,int> > strips;
        int l=1;
        for(size_t t=0;t<cuts.size();++t){ strips.push_back(make_pair(l,cuts[t])); l=cuts[t]+1; }
        strips.push_back(make_pair(l,R));
        // 垂直切点全局唯一：先枚举这一套切点，再看所有带里的最小块
        long long worst=-1;
        for(int vm=0; vm<vlim; ++vm){
            int cc=0;
            for(int k=0;k<C-1;k++) if(vm>>k&1) cc++;
            if(cc!=B-1) continue;
            long long gmin=(long long)4e18;
            for(int s=0;s<A;++s){
                int r1=strips[s].first, r2=strips[s].second;
                long long cur=0, mn=(long long)4e18;
                for(int j=1;j<=C;j++){
                    long long v=0;
                    for(int i=r1;i<=r2;i++) v+=a[i][j];
                    cur+=v;
                    bool cut=(j<C && (vm>>(j-1)&1));
                    if(cut){ if(cur<mn) mn=cur; cur=0; }
                }
                if(cur<mn) mn=cur;
                if(mn<gmin) gmin=mn;
            }
            if(gmin>worst) worst=gmin;
        }
        if(worst>best) best=worst;
    }
    cout<<best<<"\n";
    return 0;
}
