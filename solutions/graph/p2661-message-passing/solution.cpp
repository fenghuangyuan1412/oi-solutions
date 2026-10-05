#include<bits/stdc++.h>
using namespace std;
const int N=2e5+10;
int a[N],minn=N,dx[N];
bool vis[N],vis1[N];
void dfs(int now,int num) {
	if(vis1[now]) {
		return ;
	}
	if(vis[now]) {
		minn=min(minn,num-dx[now]);
	} else {
		vis[now]=1;
		dx[now]=num;
		dfs(a[now],num+1);
		vis1[now]=1;
	}
	return ;
}
int main() {
	int n;
	cin>>n;
	for(int i=1; i<=n; i++) {
		cin>>a[i];
	}
	for(int i=1; i<=n; i++) {
		dfs(i,0);
	}
	cout<<minn;
	return 0;
}
