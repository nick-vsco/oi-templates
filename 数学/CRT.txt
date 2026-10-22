给 n 个同余方程组 x=a(mod b)，要解出最小的x

#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;
int n;
ull ans;
long long b[15],a[15];
ull A=1,Ai[15];
ull invA[15];

void exgcd(ull a,ull b,ull &x,ull &y)
{
	if(b==0){
		x=1;
		y=0;
		return ;
	}
	exgcd(b,a%b,x,y);
	ull gcd=x;
	x=y,y=gcd-y*(a/b);
}
int main()
{
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
		scanf("%lld%lld",&a[i],&b[i]),A=(ull)A*a[i];
	for(int i=1;i<=n;i++)
		Ai[i]=A/a[i];
	// 整数解为：x= (b1*A1*invA1)*...*(bn*An*invAn) 
	// 此处的 invA 表示在 mod a[i] 意义下 Ai[i] 的逆元
	for(int i=1;i<=n;i++)
	{
		ull x=0,y=0;
		exgcd(Ai[i],a[i],x,y);
		invA[i]=(x+a[i])%a[i];
	}
	for(int i=1;i<=n;i++)
	{
		ans+=b[i]*invA[i]*Ai[i]%A;
		ans%=A;
	}
	cout<<ans%A;
	return 0;
}