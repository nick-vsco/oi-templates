维护多重集合：插入、删除一个数、查询x的排名、查询排名为x的数、查询x的前驱和后继

#include<bits/stdc++.h>
using namespace std;
#define INF (1<<30)

int n;
int opt,x;
const int N=1e5+10;

struct Treap{
	int l[N],r[N],val[N],rnd[N],size[N],w[N];
	// l[i],r[i] -> i的左右孩子
	// val[i] -> i的值
	// rnd[i] -> 分配的随机优先级 
	// w[i] -> i出现的次数
	// size[i] -> i为根的子树大小 
	int sz,ans,rt;
	// sz -> 给编号 
	// rt -> root
	// ans -> 答案（主要用于找前驱/后继） 
	 
	void pushup(int x) // 获取子树大小 
	{
		size[x]=size[l[x]]+size[r[x]]+w[x];
	}
	void Zip(int &k) // 左旋 k 节点  左旋就是让右孩子来做父亲 
	{
		int t=r[k]; // t 是左孩子
		r[k]=l[t];
		l[t]=k;
		size[t]=size[k];
		pushup(k);
		k=t;
	}
	void Zap(int &k) // 右旋 k 节点 
	{
		int t=l[k];
		l[k]=r[t];
		r[t]=k;
		size[t]=size[k];
		pushup(k);
		k=t;
	}
	void insert(int &k,int x) // 插入 x 
	{
		if(!k)
		{
			sz++; // 新建一个节点 给编号 
			k=sz;
			size[k]=1; // 因为是子节点，所以初始的size是1 
			w[k]=1; // 出现次数是 1 
			val[k]=x; // 节点的值是 x 
			rnd[k]=rand(); // Treap 特有的随机优先级 
			return ;
		}
		size[k]++;
		if(val[k]==x)
			w[k]++;
		else if(val[k]<x)
		{
			insert(r[k],x);// 到右子树 
			if(rnd[r[k]]<rnd[k])
				Zip(k); // 左旋维护小根堆特性 
		}
		else
		{
			insert(l[k],x);
			if(rnd[l[k]]<rnd[k])
				Zap(k); // 右旋维护小根堆特性
		}	
	}
	bool del(int &k,int x) // 删除值为 x 的节点
	{
		if(!k) // 找不到这个数 
			return false;
		if(val[k]==x) // 找到了这个数 
		{
			if(w[k]>1) // 该节点有多个数 
			{
				w[k]--;
				size[k]--;
				return true;
			}
			if(l[k]==0||r[k]==0) // 是叶子节点/只有一个孩子 
			{
				k=l[k]+r[k];
				return true;
			}
			else if(rnd[l[k]]<rnd[r[k]])
			{
				Zap(k);
				return del(k,x);
			}
			else
			{
				Zip(k);
				return del(k,x);
			}
		}
		else if(val[k]<x) // 去右子树找 
		{
			bool succ=del(r[k],x);
			if(succ) size[k]--;// 能找到就将自己的size -1 
			return succ;
		}
		else // 去左子树找
		{
			bool succ=del(l[k],x);
			if(succ) size[k]--;
			return succ;
		}
	}
	
	int queryrank(int k,int x) // 已知值找排名 
	{
		if(!k) return 0;
		if(val[k]==x)
			return size[l[k]];
		else if(val[k]<x)
			return size[l[k]]+w[k]+queryrank(r[k],x);
		else 
			return queryrank(l[k],x);
	}
	
	int querynum(int k,int x) // 已知排名找值
	{
		if(!k) return 0;
		if(x<=size[l[k]])
			return querynum(l[k],x);
		else if(x>size[l[k]]+w[k])
			return querynum(r[k],x-size[l[k]]-w[k]);
		else
			return val[k];
	}
	
	void querypre(int k,int x) // 找 x 的前驱 
	{
		if(!k) return ;
		if(val[k]<x)
			ans=k,querypre(r[k],x);
		else querypre(l[k],x);
	}
	
	void querysub(int k,int x) // 找 x 的后继
	{
		if(!k) return ;
		if(val[k]>x)
			ans=k,querysub(l[k],x);
		else querysub(r[k],x);
	}
}T;

int main()
{
	srand(123);
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
	{
		scanf("%d%d",&opt,&x);
		if(opt==1)
			T.insert(T.rt,x);
		else if(opt==2)
			T.del(T.rt,x);
		else if(opt==3)
			printf("%d\n",T.queryrank(T.rt,x)+1);
		else if(opt==4)
			printf("%d\n",T.querynum(T.rt,x));
		else if(opt==5)
		{
			T.ans=0;
			T.querypre(T.rt,x);
			printf("%d\n",T.val[T.ans]);
		}
		else if(opt==6)
		{
			T.ans=0;
			T.querysub(T.rt,x);
			printf("%d\n",T.val[T.ans]);
		}
	}
	return 0;
}