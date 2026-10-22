/*
可持久化线段树 2 （主席树 2） - 静态区间第 k 小 
做法：可以用可持久化线段树，第 i 棵线段树维护 [1,i] 区间里面每一个数的出现次数 
注意实际上维护的是对应数字的 离散化后的下标 的出现次数 
那么询问 [l,r] 区间中第 k 小的数字，可以用 [1,l-1] 和 [1,r] 两棵树来得到 

具体：
1. 将 a[] 离散化为 b[] 
2. 容易二分找到 a[i]，每次更新一个版本，就把 a[i] 加入到新版本里面 

加入方法：
因为 b 是离散化数组，因此 b 是有序且连续的，即下标越小，值越小。假设 k 为 a[i] 对应的 b 下标
在每一棵线段树里面，左节点都比右节点小（类似于bst）
所以直接比较 k 和 mid 的大小即可决定 a[i] 应在左子树还是在右子树

3. 查询。
查询找 [1,r] 和 [1,l-1] 相减即可。

相减：
首先算出左子树大小差 x ，因为算第 k 小，小的数字都在左子树
大小差就是 [l,r] 之间小的数字的个数
(1) x>k 去左子树找即可 
(2) x<k 去右子树找第 k-x 小 
*/

#include<bits/stdc++.h>
using namespace std;

int n,m;
const int N=2e5+10;
int a[N],b[N],c[N];

struct node{
	int l,r,val;
}Tree[N<<5];
int rt[N<<5],point;
void build(int &p,int l,int r) // 建一个空树，一切都是 0 
{
	p=++point;
	if(l==r) return ;
	int mid=l+r>>1;
	build(Tree[p].l,l,mid);
	build(Tree[p].r,mid+1,r);
}
int update(int p,int l,int r,int k) // 加点，k 就是对应的下标 
{
	int x=++point;
	Tree[x]=Tree[p];
	Tree[x].val++;
	if(l==r)
		return x;
	int mid=l+r>>1;
	if(k<=mid)
		Tree[x].l=update(Tree[x].l,l,mid,k);
	else
		Tree[x].r=update(Tree[x].r,mid+1,r,k);
	return x;
}
int query(int u,int v,int l,int r,int k)
{
	if(l==r)
		return l;
	int mid=l+r>>1,x=Tree[Tree[v].l].val-Tree[Tree[u].l].val;
	if(x>=k)
		return query(Tree[u].l,Tree[v].l,l,mid,k);
	else
		return query(Tree[u].r,Tree[v].r,mid+1,r,k-x);
}

int main()
{
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)
		scanf("%d",&a[i]),
		b[i]=a[i];
	sort(b+1,b+n+1);
	int reu=unique(b+1,b+n+1)-b-1;
	build(rt[0],1,reu);
	for(int i=1;i<=n;i++)
	{
		int k=lower_bound(b+1,b+reu+1,a[i])-b; // 二分找下标 
		rt[i]=update(rt[i-1],1,reu,k); // 加入下标，并且标记 i 版本的根节点 
	}
	while(m--)
	{
		int l,r,k;
		scanf("%d%d%d",&l,&r,&k);
		cout<<b[query(rt[l-1],rt[r],1,reu,k)]<<'\n'; // 询问，注意主席树里面的是下标 
	}
	return 0;
}

