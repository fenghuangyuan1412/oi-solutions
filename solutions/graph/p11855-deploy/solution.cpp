#include<bits/stdc++.h>
using namespace std;
const int N=1e6+10;
vector<int>q[N];
int n,a[N],c1[N],c2[N],m,ans[N],k;
void dfs(int x,int fa) {
ans[x]+=(c1[x]+c2[x]);
for(int i=0; i<q[x].size(); i++) {
int y=q[x][i];
ans[y]+=c2[x];
if(y==fa)continue;
c1[y]+=c1[x];
dfs(y,x);
}
}
int main() {
cin>>n;
for(int i=1; i<=n; i++) {
cin>>a[i];
}
for(int i=1; i<n; i++) {
int u,v;
cin>>u>>v;
q[u].push_back(v);
q[v].push_back(u);
}
cin>>m;
while(m--) {
int u,v,x;
cin>>x>>u>>v;
if(x==1) {
c1[u]+=v;
} else {
c2[u]+=v;
}
}
dfs(1,0);
cin>>k;
while(k--) {
int p;
cin>>p;
cout<<a[p]+ans[p]<<"\n";
}
return 0;
}
