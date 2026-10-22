康托展开：排列 -> 字典序排名（0-based，从 1 开始的排名再 +1），排名对 mod 取模
线段树优化，O(n log n)

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int mod = 998244353;
const int maxn = 1e6 + 10;
int n;
ll fact[maxn];      // fact[i] = i! mod 998244353（模意义下计算，n 可到 1e6）

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

    int query(int p, int l, int r, int L, int R)
    {
        if (L <= l && r <= R)
            return var[p];
        int res = 0;
        int mid = (l + r) >> 1;
        if (L <= mid) res += query(p << 1, l, mid, L, R);
        if (mid < R) res += query(p << 1 | 1, mid + 1, r, L, R);
        return res;
    }
}T;

void initFact(int m)
{
    fact[0] = 1;
    for (int i = 1; i <= m; i++) fact[i] = fact[i - 1] * i % mod;
}

// 康托展开：p[1..n] 为排列，返回 rank % mod
ll cantor(int p[])
{
    T.build(1, 1, n, 0);
    ll rank = 0;
    for (int i = n; i >= 1; i--)
    {
        int c = (p[i] > 1) ? T.query(1, 1, n, 1, p[i] - 1) : 0;
        rank = (rank + (ll)c * fact[n - i]) % mod;
        T.change(1, 1, n, p[i], 1);
    }
    return rank;
}

int main()
{
    scanf("%d", &n);
    initFact(n);
    static int p[maxn];
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
    ll r = (cantor(p) + 1) % mod;
    cout << r;
    return 0;
}