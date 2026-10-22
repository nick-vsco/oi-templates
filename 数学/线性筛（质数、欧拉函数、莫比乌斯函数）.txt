// 质数
bool vis[maxn];
int prime[maxn],cnt;

void init()
{
	vis[1]=1;
	for(int i=2;i<=n;i++)
	{
		if(!vis[i])
			prime[++cnt]=i;
		for(int j=1;prime[j]*i<=n;j++)
		{
			vis[prime[j]*i]=1;
			if(i%prime[j]==0) break;
		}
	}
}

// 欧拉函数，就是在 [1,n] 中跟 n 互质的数字的个数
const int maxn=4e4;
int prime[maxn],cnt;
bool vis[maxn];
int phi[maxn];
void get_phi()
{
    phi[1]=1;
    for(int i=2;i<=maxn;i++)
    {
        if(!vis[i])
        {
            prime[++cnt]=i;
            phi[i]=i-1;        
	}
        for(int j=1;j<=cnt&&i*prime[j]<=maxn;j++)
        {
            vis[i*prime[j]]=1;
            if(i%prime[j]==0)
            {
                phi[i*prime[j]]=phi[i]*prime[j];
                break;
            }
            phi[i*prime[j]]=phi[i]*phi[prime[j]];
        }
    }
}

// 莫比乌斯函数
bool vis[maxn];
int prime[maxn]; //筛质数 质数的莫比乌斯函数值=-1
int mu[maxn];
int cnt;
void get_mu()
{
    mu[1]=1;
    for(int i=2;i<=maxn;i++)
    {
        if(!vis[i])
        {
            mu[i]=-1;
            prime[++cnt]=i;
        }
        for(int j=1;j<=cnt&&prime[j]*i<=maxn;j++)
        {
            vis[prime[j]*i]=1;
            if(i%prime[j]==0)break;
            else mu[i*prime[j]]=-mu[i];
        }
    }
    return ;
}