int funtion(int n)
{
    // 整除分块（数论分块）
    // 计算 i=[1,n]的 n/i 值
    int ans=0;
    for(int l=1,r;l<=n;l=r+1)
    {
        r=n/(n/l);//这里的 r,l 就是两个相邻的因子，理解一下为什么 r 要这样算
        ans+=(r-l+1)*(n/l);//在 (r-l+1) 这个长度中每个数都是一样的，都是 n/l，同理可以加一堆能做前缀和的东西进去
    }
    return ans;
}