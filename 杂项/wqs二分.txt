// 规定必须选 k 个东西，加惩罚然后正常做，记录一下选的个数

// 求MST，但是必须选 k 条白边

#include<bits/stdc++.h>
using namespace std;

const int maxn = 5e4 + 10;
const int maxm = 1e5 + 10;
int n, m, need;
struct node{
    int u, v, w, flag;
}edge[maxm];
int head[maxn], cnt, sum;
int fa[maxn];

bool cmp(node x, node y)
{
    if (x.w == y.w) return x.flag < y.flag; // 优先选白色
    return x.w < y.w;
}

int find(int x)
{
    if (fa[x] == x) return x;
    return fa[x] = find(fa[x]);
}

int kruskal(int add_cost)
{
    for (int i = 1; i <= m; i++)
        if (!edge[i].flag)
            edge[i].w += add_cost;
    int white = 0;
    cnt = sum = 0;
    sort(edge + 1, edge + m + 1, cmp);
    for (int i = 0; i <= n; i++)
        fa[i] = i;
    for (int i = 1; i <= m; i++)
    {
        int u = edge[i].u, v = edge[i].v;
        if (find(u) == find(v)) continue;
        fa[find(u)] = find(v);
        sum += edge[i].w;
        cnt++;
        if (!edge[i].flag)
            white++;
        if (cnt == n - 1)
            break;
    }
    for (int i = 1; i <= m; i++)
        if (!edge[i].flag)
            edge[i].w -= add_cost;
    return white;
}

int main()
{
    cin >> n >> m >> need;
    for (int i = 1; i <= m; i++)
        cin >> edge[i].u >> edge[i].v >> edge[i].w >> edge[i].flag;
    int l = -101, r = 101, lam = 0;
    while (l <= r)
    {
        int mid = (l + r) >> 1;
        if (kruskal(mid) >= need)
        {
            l = mid + 1;
            lam = mid;
        }
        else r = mid - 1;
    }
    kruskal(lam);
    cout << sum - lam * need;
    return 0;
}