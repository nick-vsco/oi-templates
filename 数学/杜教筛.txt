// O(n^{0.75}) 筛出欧拉函数、莫比乌斯函数的前缀和


#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
unordered_map<int,ll>ans_mu;
unordered_map<int,ll>ans_phi;
const int maxn=1e7+5e6;        // 这里 maxn 取实际 n 的 0.75 次方即可，节省空间
int prime[maxn+5],cnt;
bool vis[maxn+5];
ll mu[maxn+5];
ll phi[maxn+5];
void pretreat()
{
    phi[1]=1;
    mu[1]=1;
    for(int i=2;i<=maxn;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            phi[i]=i-1;
            mu[i]=-1;
        }
        for(int j=1;j<=cnt&&i*prime[j]<=maxn;j++)
        {
            vis[i*prime[j]]=1;
            if(i%prime[j]==0)
            {
                phi[i*prime[j]]=1ll*prime[j]*phi[i];
                break;
            }
            phi[i*prime[j]]=1ll*phi[i]*phi[prime[j]];
            mu[i*prime[j]]=-mu[i];
        }
    }
    for(int i=1;i<=maxn;i++)
    {
        mu[i]+=mu[i-1];
        phi[i]+=phi[i-1];
    }
}
ll ask_phi(ll n)
{
    if(n<=maxn)return phi[n];
    if(ans_phi[n])return ans_phi[n];
    ll res=(1ll+n)*n>>1;
    for(ll l=2,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        res-=1ll*(r-l+1)*ask_phi(n/l);
    }
    return ans_phi[n]=res;
}
ll ask_mu(ll n)
{
    if(n<=maxn)return mu[n];
    if(ans_mu[n])return ans_mu[n];
    ll res=1;
    for(ll l=2,r;l<=n;l=r+1)
    {
        r=n/(n/l);
        res-=(r-l+1)*ask_mu(n/l);
    }
    return ans_mu[n]=res;
}
int t;
long long n;
int main(){
    pretreat();
    cin>>t;
    while(t--)
    {
        cin>>n;
        cout<<ask_phi(n)<<' '<<ask_mu(n)<<'\n';
    }
    return 0;
}