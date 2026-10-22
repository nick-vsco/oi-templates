#include<bits/stdc++.h>
using namespace std;

int n;
struct line{
	int l,r,h;
	int pd;
	friend bool operator <(const line a,const line b)
	{
		return a.h<b.h;
	}
}a[200005];
int cnt;
int w[200005];
struct node{
	int l,r,len,var;
}t[200007<<2];

void build(int p,int l,int r)
{
	t[p].l=l,t[p].r=r;
	if(l==r)return ;
	int mid=l+r>>1;
	build(p<<1,l,mid);
	build(p<<1|1,mid+1,r);
}
void pushup(int p)
{
	if(t[p].var)
		t[p].len=w[t[p].r+1]-w[t[p].l];
	else if(t[p].l!=t[p].r)
		t[p].len=t[p<<1].len+t[p<<1|1].len;
	else 
		t[p].len=0;
}
void change(int p,int l,int r,int k)
{
	if(t[p].l>=l&&t[p].r<=r)
	{
		t[p].var+=k;
		pushup(p);
		return ;
	}
	int mid=t[p].l+t[p].r>>1;
	if(l<=mid)change(p<<1,l,r,k);
	if(r>mid)change(p<<1|1,l,r,k);
	pushup(p);
}
long long ans;
int main()
{
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
	{
		int X1,X2,Y1,Y2;
		scanf("%d%d%d%d",&X1,&Y1,&X2,&Y2);
		a[2*i-1]=(line){X1,X2,Y1,1};
		a[2*i]=(line){X1,X2,Y2,-1};
		w[2*i-1]=X1;
		w[2*i]=X2;
	}
	sort(a+1,a+n*2+1);
	sort(w+1,w+n*2+1);
	int reu=unique(w+1,w+n*2+1)-w-1;
	build(1,1,reu-1);
	ans=0;
	for(int i=1;i<n*2;i++)
	{
		int d1=lower_bound(w+1,w+reu+1,a[i].l)-w;
		int d2=lower_bound(w+1,w+reu+1,a[i].r)-w;
		change(1,d1,d2-1,a[i].pd);
		ans+=1ll*t[1].len*(a[i+1].h-a[i].h);		
	}
	cout<<ans;
	return 0;
}