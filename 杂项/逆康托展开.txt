逆康托展开：由精确排名还原排列 p[1..n]
线段树优化，O(n log n)
注意：rank 必须是精确值（long long），不能用取模后的排名

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int maxn = 1e6 + 10;
int n;
ll fact[maxn];      // 注意：n <= 20 时 fact 不溢出 64 位

struct Segment{
    int var[maxn << 2];

    void pushup(int p)
    {
        var[p] = var[p << 1] + var[p << 1 | 1];
    }

    void build(int p, int l, int r, int v)
    {
        if (l == r)
        {
            var[p] = v;
            return ;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, v);
        build(p << 1 | 1, mid + 1, r, v);
        pushup(p);
    }

    void change(int p, int l, int r, int pos, int x)
    {
        if (l == r)
        {
            var[p] = x;
            return ;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid) change(p << 1, l, mid, pos, x);
        else change(p << 1 | 1, mid + 1, r, pos, x);
        pushup(p);
    }

    int kth(int p, int l, int r, int k)
    {
        if (l == r)
            return l;
        int mid = (l + r) >> 1;
        if (var[p << 1] >= k) return kth(p << 1, l, mid, k);
        return kth(p << 1 | 1, mid + 1, r, k - var[p << 1]);
    }
}T;

void initFact(int m)
{
    fact[0] = 1;
    for (int i = 1; i <= m; i++) fact[i] = fact[i - 1] * i;
}

// 逆康托展开：由精确排名生成排列 p[1..n]
void invCantor(ll rank, int p[])
{
    T.build(1, 1, n, 1);
    for (int i = 1; i <= n; i++)
    {
        ll f = fact[n - i];
        int k = (int)(rank / f) + 1;
        rank %= f;
        p[i] = T.kth(1, 1, n, k);
        T.change(1, 1, n, p[i], 0);
    }
}

int main()
{
    scanf("%d", &n);
    initFact(n);
    ll rank;
    scanf("%lld", &rank);
    static int p[maxn];
    invCantor(rank, p);
    for (int i = 1; i <= n; i++) printf("%d ", p[i]);
    printf("\n");
    return 0;
}