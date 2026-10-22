// 原题：f(i) 表示对 i 前面的 j 的三维偏序 (a,b,c) 的数量，问：给定 d = [0,n)，求 f(i)=d 的数量 -> ans[d]


#include<bits/stdc++.h>
using namespace std;

const int maxn = 1e5 + 10;
const int maxk = 2e5 + 10;
int n, k, m, tot;
int ans[maxn];
struct node{
    int a, b, c, cnt, res;

    bool operator != (node x) const {
        if (x.a != a) return 1;
        if (x.b != b) return 1;
        if (x.c != c) return 1;
        return 0;
    }
}e[maxn], ne[maxn];
struct bitT{
    int var[maxk];

    int lowbit(int x)
    {
        return x & (-x);
    }

    void add(int pos, int x)
    {
        for ( ; pos <= k; pos += lowbit(pos))
            var[pos] += x;
    }

    int ask(int pos)
    {
        int res = 0;
        for ( ; pos; pos -= lowbit(pos))
            res += var[pos];
        return res;
    }
}BIT;

bool cmpA(node x, node y)
{
    if (x.a != y.a) return x.a < y.a;
    if (x.b != y.b) return x.b < y.b;
    return x.c < y.c;
}

bool cmpB(node x, node y)
{
    if (x.b != y.b) return x.b < y.b;
    return x.c < y.c;
}

void CDQ(int l, int r)
{
    if (l == r) return ;
    int mid = (l + r) >> 1;
    CDQ(l, mid);
    CDQ(mid + 1, r);

    // 开始处理两个区间之间的问题
    sort(ne + l, ne + mid + 1, cmpB);
    sort(ne + mid + 1, ne + r + 1, cmpB);
    int i = l, j = mid + 1;
    while (j <= r)
    {
        while (i <= mid && ne[i].b <= ne[j].b)
        {
            BIT.add(ne[i].c, ne[i].cnt);
            i++;
        }
        ne[j].res += BIT.ask(ne[j].c);
        j++;
    }

    // 清除贡献
    for (int k = l; k < i; k++)
        BIT.add(ne[k].c, -ne[k].cnt);
    return ;
}

int main()
{
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
        cin >> e[i].a >> e[i].b >> e[i].c;
    sort(e + 1, e + n + 1, cmpA);

    for (int i = 1; i <= n; i++)
    {
        tot++;
        if (e[i] != e[i + 1])
        {
            m++;
            ne[m] = e[i];
            ne[m].cnt = tot;
            tot = 0;
        }
    }

    CDQ(1, m);

    for (int i = 1; i <= m; i++)
        ans[ne[i].res + ne[i].cnt - 1] += ne[i].cnt;
    
    for (int i = 0; i < n; i++)
        cout << ans[i] << '\n';
    return 0;
}