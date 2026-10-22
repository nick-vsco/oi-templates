给n个点，确定一条曲线f，然后求f(k)

#include<bits/stdc++.h>
using namespace std;

long long n,k;
long long y[2007],x[2007];
const long long mod=998244353;
long long ans;
long long quickpow(long long a,long long b)
{
	long long res=1;
	while(b)
	{
		if(b&1)res=res*a%mod;
		a=a*a%mod;
		b>>=1;
	}
	return res;
}
long long inv(long long k)
{
	return quickpow(k,mod-2);
}

int main()
{
	cin>>n>>k;
	for(int i=1;i<=n;i++)
		cin>>x[i]>>y[i];
	for(int i=1;i<=n;i++)
	{
		long long s1=y[i]%mod;
		long long s2=1ll;
		for(int j=1;j<=n;j++)
		{
			if(i==j)continue;
			s1=s1*(k-x[j])%mod;
			s2=s2*((x[i]-x[j])%mod)%mod;
		}
		ans+=s1*inv(s2)%mod;
		ans=(ans+mod)%mod;
	}
	cout<<ans;
	return 0;
}