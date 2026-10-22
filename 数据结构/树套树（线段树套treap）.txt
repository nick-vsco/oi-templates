维护一个有序数列，其中需要提供以下操作：

1. 查询 $k$ 在区间内的排名；
2. 查询区间内排名为 $k$ 的值；
3. 修改某一位置上的数值；
4. 查询 $k$ 在区间内的前驱（前驱定义为严格小于 $x$，且最大的数，若不存在输出 `-2147483647`）
5. 查询 $k$ 在区间内的后继（后继定义为严格大于 $x$，且最小的数，若不存在输出 `2147483647`）

时空复杂度均为 $O(n\log^2n)$

#include<bits/stdc++.h>
using namespace std;

const int N=1e5+10;
const int inf=INT_MAX;
int n,m,opt,l,r,pos,k;
int a[N];

struct BST{
	int s[N*20][2],size[N*20],w[N*20],val[N*20],rnd[N*20];
	int rt,sz,ans;
	
	int newnode(int x)
	{
		++rt;
		s[rt][0]=s[rt][1]=0;
		size[rt]=1;
		w[rt]=1;
		val[rt]=x;
		rnd[rt]=(rand()<<10)+rand();
		return rt;
	}
	void pushup(int p)
	{
		size[p]=size[s[p][0]]+size[s[p][1]]+w[p];
	}
	void rotate(int &p,int d)
	{
		int t=s[p][d];
		s[p][d]=s[t][d^1];
		s[t][d^1]=p;
		pushup(p);
		pushup(t);
		p=t;
	}
	void insert(int &p,int x)
	{
		if(!p)
			p=newnode(x);
		else if(val[p]==x)
			w[p]++;
		else if(val[p]>x)
		{
			insert(s[p][0],x);
			if(rnd[s[p][0]]>rnd[p])
				rotate(p,0);
		}
		else
		{
			insert(s[p][1],x);
			if(rnd[s[p][1]]>rnd[p])
				rotate(p,1);
		}
		pushup(p);
	}
	void del(int &p,int x)
	{
		if(val[p]<x)
			del(s[p][1],x);
		else if(val[p]>x)
			del(s[p][0],x);
		else
		{
			if(w[p]>1)
				w[p]--;
			else
			{
				if(!s[p][0]&&!s[p][1])
					p=0;
				else if(!s[p][0])
				{
					rotate(p,1);
					del(s[p][0],x);
				}
				else if(!s[p][1])
				{
					rotate(p,0);
					del(s[p][1],x);
				}
				else
				{
					if(rnd[s[p][0]]>rnd[s[p][1]])
					{
						rotate(p,0);
						del(s[p][1],x);
					}
					else
					{
						rotate(p,1);
						del(s[p][0],x);
					}
				}
			}
		}
		if(p)
			pushup(p);
	}
	int querynum(int p,int x)
	{
		if(size[s[p][0]]+1==x)
			return val[p];
		else if(size[s[p][0]]+1>x)
			return querynum(s[p][0],x);
		else
			return querynum(s[p][1],x-size[s[p][0]]-1);
	}
	int queryrank(int p,int x)
	{
		if(!p)
			return 0;
		if(val[p]==x)
			return size[s[p][0]];
		else if(val[p]>x)
			return  queryrank(s[p][0],x);
		else
			return w[p]+size[s[p][0]]+queryrank(s[p][1],x);
	}
	int querypre(int p,int x)
	{
		if(!p)
			return -inf;
		else if(val[p]>=x)
			return querypre(s[p][0],k);
		else
			return max(val[p],querypre(s[p][1],k));
	}
	int querysub(int p,int k)
	{
		if(!p)
			return inf;
		else if(val[p]<=k)
			return querysub(s[p][1],k);
		else
			return min(val[p],querysub(s[p][0],k));
	}
}Treap;

struct node{
	int l,r,root;
};
struct SGT{
	node tree[N<<3];
	void build(int p,int l,int r)
	{
		tree[p].l=l;
		tree[p].r=r;
		for(int i=l;i<=r;i++)
			Treap.insert(tree[p].root,a[i]);
		if(l==r)
			return ;
		int mid=l+r>>1;
		build(p<<1,l,mid);
		build(p<<1|1,mid+1,r);
	}
	void update(int p,int x,int y)
	{
		Treap.del(tree[p].root,a[x]);
		Treap.insert(tree[p].root,y);
		if(tree[p].l==tree[p].r)
			return ;
		int mid=tree[p].l+tree[p].r>>1;
		if(x>mid)
			update(p<<1|1,x,y);
		else
			update(p<<1,x,y);
	}
	int queryrank(int p,int l,int r,int k)
	{
		if(tree[p].l>r||tree[p].r<l)
			return 0;
		if(tree[p].l>=l&&tree[p].r<=r)
			return Treap.queryrank(tree[p].root,k);
		else
			return queryrank(p<<1,l,r,k)+queryrank(p<<1|1,l,r,k);
	}
	int querynum(int u,int v,int k)
	{
		int l=0,r=1e8;
		while(l<r)
		{
			int mid=l+r+1>>1;
			if(queryrank(1,u,v,mid)<k)
				l=mid;
			else
				r=mid-1;
		}
		return r;
	}
	int querypre(int p,int l,int r,int k)
	{
		if(tree[p].l>r||tree[p].r<l)
			return -inf;
		else if(tree[p].l>=l&&tree[p].r<=r)
			return Treap.querypre(tree[p].root,k);
		else
			return max(querypre(p<<1,l,r,k),querypre(p<<1|1,l,r,k)); 
	}
	int querysub(int p,int l,int r,int k)
	{
		if(tree[p].l>r||tree[p].r<l)
			return inf;
		else if(tree[p].l>=l&&tree[p].r<=r)
			return Treap.querysub(tree[p].root,k);
		else
			return min(querysub(p<<1,l,r,k),querysub(p<<1|1,l,r,k));
	}
}T;

int main()
{
	srand(233);
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)
		scanf("%d",&a[i]);
	T.build(1,1,n);
	while(m--)
	{
		scanf("%d",&opt);
		if(opt==1)
		{
			scanf("%d%d%d",&l,&r,&k);
			cout<<T.queryrank(1,l,r,k)+1<<'\n';
		}
		else if(opt==2)
		{
			scanf("%d%d%d",&l,&r,&k);
			cout<<T.querynum(l,r,k)<<'\n';
		}
		else if(opt==3)
		{
			scanf("%d%d",&pos,&k);
			T.update(1,pos,k);
			a[pos]=k;
		}
		else if(opt==4)
		{
			scanf("%d%d%d",&l,&r,&k);
			cout<<T.querypre(1,l,r,k)<<'\n';
		}
		else
		{
			scanf("%d%d%d",&l,&r,&k);
			cout<<T.querysub(1,l,r,k)<<'\n';
		}
	}
	return 0;
}
