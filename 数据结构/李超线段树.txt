// 加入直线、查询最大值的情况
const int maxn = 1e5 + 10;
int n;
struct Line{
    double k, b;

    double cal(int x)
    {
        return k * x + b;
    }
}line[maxn];
int cnt;
struct LCSGT{
    int var[maxn << 2];             // var 记录最高的直线的编号
    void update(int p, int l, int r, int id)
    {
        if (l == r)
            return ;
        int mid = (l + r) >> 1;
        if (line[var[p]].k > line[id].k)
        {
            if (line[var[p]].cal(mid) < line[id].cal(mid))
            {
                update(p << 1 | 1, mid + 1, r, var[p]);
                var[p] = id;
            }
            else update(p << 1, l, mid, id);
        }
        else
        {
            if (line[var[p]].cal(mid) < line[id].cal(mid))
            {
                update(p << 1, l, mid, var[p]);
                var[p] = id;
            }
            else update(p << 1 | 1, mid + 1, r, id);
        }
    }
    double query(int p, int l, int r, int x)
    {
        if (l == r)
            return line[var[p]].cal(x);
        int mid = (l + r) >> 1;
        double ans = line[var[p]].cal(x);
        if (x <= mid)
            ans = max(ans, query(p << 1, l, mid, x));
        else
            ans = max(ans, query(p << 1 | 1, mid + 1, r, x));
        return ans;
    }
}Tree;

int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> s;
        if (s == "Query")         // 查询最大值
        {
            int x;
            cin >> x;
            cout << Tree.query(1, 1, maxn - 10, x) << '\n';
        }
        else                              // 添加直线
        {
            double S, P;
            cin >> S >> P;
            line[++cnt] = (Line){P, S - P}; // 这里是对应 k, b
            Tree.update(1, 1, maxn - 10, cnt);
        }
    }
    return 0;
}

// 加入线段、查询最大值的情况（这个原题是面对相同最大值要输出编号最小的，所以用了pair）
#include<bits/stdc++.h>
using namespace std;
#define mp(x, y) make_pair(x, y)

const double eps = 1e-10;
const int maxn = 1e5 + 10;
const int mod1 = 39989;
const int mod2 = 1e9;
int n;
struct line{
    double k, b;
    int id;

    double calc(int x)
    {
        return k * x + b;
    }

    void init(int X0, int Y0, int X1, int Y1)
    {
        if (X0 == X1)
        {
            k = 0;
            b = max(Y0, Y1);
            return ;
        }
        k = 1.0 * (Y1 - Y0) / (X1 - X0);
        b = 1.0 * Y0 - k * X0;
    }
}p[maxn];
int cnt, lastans;

struct LCST{
    int var[maxn << 2];

    pair<double, int>mx(pair<double, int>a, pair<double, int>b)
    {
        if (a.first - b.first > eps)
            return a;
        else if (b.first - a.first > eps)
            return b;
        else
            return a.second < b.second ? a : b;
    }

    void update(int point, int l, int r, int L, int R, int id)
    {
        if (r < L || l > R)
            return ;
        int mid = (l + r) >> 1;
        if (L <= l && r <= R) // 被完全覆盖
        {
            if (!var[point])
            {
                var[point] = id;
                return ;
            }
            if (p[id].calc(mid) - p[var[point]].calc(mid) > eps)
                swap(id, var[point]);
            if (p[id].calc(l) - p[var[point]].calc(l) > eps || (p[id].calc(l) == p[var[point]].calc(l) && id < var[point]))
                update(point << 1, l, mid, L, R, id);
            if (p[id].calc(r) - p[var[point]].calc(r) > eps || (p[id].calc(r) == p[var[point]].calc(r) && id < var[point]))
                update(point << 1 | 1, mid + 1, r, L, R, id);
            return ;
        }
        // 没有完全覆盖，只能两个子区间全部递归下去
        update(point << 1, l, mid, L, R, id);
        update(point << 1 | 1, mid + 1, r, L, R, id);
    }

    pair<double, int>query(int point, int l, int r, int x)
    {
        if (r < x || l > x)
            return mp(0.0, 0);
        pair<double, int>res = mp(p[var[point]].calc(x), var[point]);
        if (l == r)
            return res;
        int mid = (l + r) >> 1;
        return mx(res, mx(query(point << 1, l, mid, x), query(point << 1 | 1, mid + 1, r, x)));
    }
}T;

int main()
{
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        int opt;
        cin >> opt;
        if (!opt)
        {
            int k;
            cin >> k;
            k = (k + lastans - 1) % mod1 + 1;
            lastans = T.query(1, 1, mod1, k).second;
            cout << lastans << '\n';
        }
        else
        {
            int X0, X1, Y0, Y1;
            cin >> X0 >> Y0 >> X1 >> Y1;

		// 这里是强制在线的要求，可以不管
            X0 = (X0 + lastans - 1) % mod1 + 1;
            Y0 = (Y0 + lastans - 1) % mod2 + 1;
            X1 = (X1 + lastans - 1) % mod1 + 1;
            Y1 = (Y1 + lastans - 1) % mod2 + 1;
		// 添加线段
            p[++cnt].init(X0, Y0, X1, Y1);
            T.update(1, 1, mod1, min(X0, X1), max(X0, X1), cnt); 
        }
    }
    return 0;
}