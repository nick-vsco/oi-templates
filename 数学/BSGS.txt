BSGS解方程求x： b^x=n (mod p)

#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
unordered_map<ll, ll> mp;
ll p,b,n,s;
int main() {
	cin>>p>>b>>n, s=sqrt(p)+1;
	for(ll y=1, pw=n*b%p; y<=s; y++, pw=pw*b%p) mp[pw%p]=y;
	
	ll t=1;
	for(ll i=1; i<=s; i++) t=t*b%p;
	for(ll x=1, pw=t; x<=s; x++, pw=pw*t%p)
		if(mp[pw])
			return cout<<x*s-mp[pw], 0;
    
	cout<<"no solution";
	return 0;
}
