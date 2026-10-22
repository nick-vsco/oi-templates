// 可持久化线段树 ( 主席树 )
// 例题 1 修改某一历史版本的某个值、查询某一历史版本的某个值 
// 可持久化数据结构，就是可以记录每次历史版本，并且进行修改
// 分为完全可持久：历史版本可以查询、修改
//   与不完全可持久：历史版本只能查询，不能修改

// 那么这个为了保存历史版本，可以建 q( 询问数 ) 棵树，一一修改，一一查询
// 但是很容易 MLE , 因为每次修改的点只有一个，放在线段树上，最多是 logn 个节点要修改
// 所以完全可以只新建这一条链。
// 具体地说，就是新建一个根节点，子节点一个是原来的值，一个是要修改的那边的值。 
// 注意空间要开 32~40 倍 

#include<bits/stdc++.h>
using namespace std;
//define int long long

const int N=1e6+10;
const int M=1e6+10;
int n,m;
int a[N];
int opt,v,x,k;

int top; // 动态开点 
int rt[M]; 
// 记录每一个版本的根节点编号，以找到每一个历史版本 
struct Tree{
	int lson,rson,val;
}tree[N*40];
int clone(int p)
{
	// 把节点完全复制给新节点
	top++;
	tree[top]=tree[p];
	return top;
}
int build(int p,int l,int r)
{
	// 建树
	p=++top; 
	if(l==r)
	{
		tree[p].val=a[l];
		return p;
	}
	int mid=l+r>>1;
	tree[p].lson=build(tree[p].lson,l,mid);
	tree[p].rson=build(tree[p].rson,mid+1,r);
	return p;
}
int update(int p,int l,int r,int x,int k)
{
	// 把在 x 位置上的值修改为 k 
	// 首先要新建版本
	p=clone(p);
	if(l==r)
		tree[p].val=k;
	else
	{
		int mid=l+r>>1;
		if(x<=mid) // 左子树被修改 
			tree[p].lson=update(tree[p].lson,l,mid,x,k);
		else // 右子树被修改
			tree[p].rson=update(tree[p].rson,mid+1,r,x,k); 
	}
	return p;
}
int query(int p,int l,int r,int k)
{
	// 查询 k 位置上的值
	if(l==r)
		return tree[p].val;
	else
	{
		int mid=l+r>>1;
		if(k<=mid)
			return query(tree[p].lson,l,mid,k);
		else 
			return query(tree[p].rson,mid+1,r,k);
	 } 
}

int main()
{
	scanf("%d%d",&n,&m); 
	for(int i=1;i<=n;i++)
		scanf("%d",&a[i]);
	rt[0]=build(0,1,n); // 初始版本的编号为 0
	for(int i=1;i<=m;i++)
	{
		scanf("%d%d",&v,&opt);
		if(opt==1)
		{
			scanf("%d%d",&x,&k);
			rt[i]=update(rt[v],1,n,x,k); // 获取新的版本编号，从 v 版本修改来的 -> rt[v] 转过来 
		}
		else
		{
			scanf("%d",&k);
			cout<<query(rt[v],1,n,k)<<'\n';
			rt[i]=rt[v];
		}
	}
    return 0;
}

