解方程：ax+by=gcd(a,b)


int exgcd(int a, int b, int &x, int &y)
{
    if(b==0){
        x=1; y=0;
        return a;
    }
    int g = exgcd(b, a%b, x, y); 
    int tmp = x;
    x = y, y = tmp - y*(a/b);
    return g;
}