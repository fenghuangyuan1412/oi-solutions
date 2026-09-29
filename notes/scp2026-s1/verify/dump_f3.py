import sys
a=[[1,2,3],[2,3,1],[3,1,2]]
n=3
f=[0]*(1<<n); f[0]=1
rows=[]
for i in range(1<<n):
    for j in range(n):
        if (i>>j)&1:
            f[i]=(f[i]+f[i^(1<<j)]*a[bin(i).count('1')-1][j])
    rows.append((i,bin(i)[2:][::-1],bin(i).count('1'),f[i]))
for r in rows:
    print('mask=%2d bits=%s k=%d f=%d'%(r[0],r[1],r[2],r[3]))
