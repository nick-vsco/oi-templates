原题：最长公共子序列（本质转化成最长上升子序列）

#include<bits/stdc++.h>
using namespace std;

const int maxn = 1e5 + 10;
int n;
int p1[maxn], p2[maxn];
int a[maxn];
int t[maxn];
int ans;
int dp[maxn];

struct Segment{
    int var[maxn << 2];

    void pushup(int p)
    {
        var[p] = max(var[p << 1], var[p << 1 | 1]);
    }

    void change(int p, int l, int r, int pos, int x)
    {
        if (l == r)
        {
            var[p] = max(var[p], x);
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
        if (L <= mid) res = max(res, query(p << 1, l, mid, L, R));
        if (mid < R) res = max(res, query(p << 1 | 1, mid + 1, r, L, R));
        return res;
    }
}T;

int main()
{
	cin >> n;
	for(int i = 1; i <= n; i++)
		cin >> p1[i];
	for(int i = 1; i <= n; i++)
		cin >> p2[i], a[p2[i]] = i;
	for(int i = 1; i <= n; i++)
		p1[i] = a[p1[i]];
    // 等价于求 p1 的最长上升子序列
	for(int i = 1; i <= n; i++)
	{
		dp[i] = T.query(1, 1, n, 1, p1[i]) + 1;
		T.change(1, 1, n, p1[i], dp[i]);
		ans = max(ans, dp[i]);
	}
	cout << ans;
	return 0;
}