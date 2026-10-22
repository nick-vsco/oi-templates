// 一定要注意斜率的判断方向，是上凸还是下凸，维护max还是min


// x, k 都是单调
#include<bits/stdc++.h>
using namespace std;
#define int long long

const int maxn = 1e6 + 10;
int n;
int a, b, c;
int sum[maxn], dp[maxn];
int q[maxn], l = 0, r = 0;

int X(int id)
{
	return sum[id];
}

int Y(int id)
{
	return dp[id] + a * sum[id] * sum[id] - b * sum[id];
}

double solpe(int x, int y, int X, int Y)
{
	if (x == X)
		return (Y > y) ? 1e18 : -1e18;
	return (1.0 * (Y - y)) / (1.0 * (X - x));
}

signed main()
{
	cin >> n >> a >> b >> c;
	for (int i = 1; i <= n; i++)
		cin >> sum[i], sum[i] += sum[i - 1];
	dp[1] = a * sum[1] * sum[1] + b * sum[1] + c;
	q[++r] = 1;

	for (int i = 2; i <= n; i++)
	{
		int k = 2 * a * sum[i];
		while (l < r && solpe(X(q[l]), Y(q[l]), X(q[l + 1]), Y(q[l + 1])) >= k)
			l++;
		int j = q[l];
		dp[i] = dp[j] + a * (sum[i] - sum[j]) * (sum[i] - sum[j]) + b * (sum[i] - sum[j]) + c;
		while (l < r && solpe(X(q[r - 1]), Y(q[r - 1]), X(q[r]), Y(q[r])) <= solpe(X(q[r]), Y(q[r]), X(i), Y(i)))
			r--;
		q[++r] = i;
	}
	cout << dp[n];
	return 0;
}

// x 单调，k 不单调，做单调队列上二分，凸壳正常维护即可

// xk都不单调，考虑 cdq 分治或者李超线段树
// cdq 分治：
#include<bits/stdc++.h>
using namespace std;
#define int long long

const int maxn = 1e5 + 10;
int n;
int  h[maxn], sum[maxn];
int dp[maxn];
struct Query{
    int x, y, id;
}q[maxn];
int que[maxn];

double slope(int x, int y, int X, int Y)
{
    if (x == X)
        return -1e18;
    return (1.0 * (Y - y)) / (1.0 * (X - x));
}

int X(int id)
{
    return h[id];
}

int Y(int id)
{
    return dp[id] - sum[id] + h[id] * h[id];
}

bool cmpx(Query a, Query b)
{
    if (a.x == b.x)
        return a.y > b.y;
    return a.x < b.x;
}

bool cmpk(Query a, Query b)
{
    return h[a.id] < h[b.id];
}

bool cmpid(Query a, Query b)
{
    return a.id < b.id;
}

void cdq(int L, int R)
{
    if (L == R)
    {
        q[L].x = X(q[L].id);
        q[L].y = Y(q[L].id);
        return ;
    }
    int mid = (L + R) >> 1;
    cdq(L, mid);

    int l = 1, r = 0;
    // 这里左边 x 有序，我们先维护左边的一个下凸壳，用左边的凸壳去求右边的答案
    for (int i = L; i <= mid; i++)
    {
        while (l < r && slope(q[que[r]].x, q[que[r]].y, q[que[r - 1]].x, q[que[r - 1]].y) >= slope(q[i].x, q[i].y, q[que[r]].x, q[que[r]].y))
            r--;
        que[++r] = i;
    }

    // 右边要 k 递增
    sort(q + mid + 1, q + R + 1, cmpk);
    // 然后求右边的答案
    for (int i = mid + 1; i <= R; i++)
    {
        while (l < r && slope(q[que[l]].x, q[que[l]].y, q[que[l + 1]].x, q[que[l + 1]].y) <= 2 * h[q[i].id])
            l++;
        int j = que[l];
        dp[q[i].id] = min(dp[q[i].id], dp[q[j].id] + (h[q[i].id] - h[q[j].id]) * (h[q[i].id] - h[q[j].id]) + sum[q[i].id - 1] - sum[q[j].id]);
    }
    // 重新回到初始状态
    sort(q + mid + 1, q + R + 1, cmpid);
    cdq(mid + 1, R);
    // 保证上面的区间的左边 x 有序
    sort(q + L, q + R + 1, cmpx);
}

signed main()
{
    memset(dp, 0x3f, sizeof dp);
    dp[1] = 0;
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> h[i], q[i].id = i;
    for (int i = 1; i <= n; i++)
        cin >> sum[i], sum[i] += sum[i - 1];
    cdq(1, n);
    cout << dp[n];
    return 0;
}