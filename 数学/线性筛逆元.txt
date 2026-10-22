void get_inv()
{
    inv[1]=1;
    for(int i=2;i<=n;i++)
    {
        inv[i]=-(p/i)*inv[p%i];
        inv[i]=(inv[i]%p+p)%p;
    }
}