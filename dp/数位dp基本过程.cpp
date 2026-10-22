#include<bits/stdc++.h>
using namespace std;
#define int long long

int a, b;
int num[25], cnt;
int dp[25][5][5][15];

int dfs(int site, bool limit, bool first, int dig)
{
    // site -> 考虑到第 site 位
    // limit -> 是否顶到上界
    // first -> 是否开始填数
    // dig -> 上一位的数码是多少
    if (!site)
        return first; // 开始填数了才有 1 种方案
    if (dp[site][limit][first][dig] != -1)
        return dp[site][limit][first][dig];
    
    int res = 0;
    int up = limit ? num[site] : 9;

    for (int i = 0; i <= up; i++)
    {
        bool newlimit = limit && (i == up);
        bool newfirst = first || i;
        if (!first) // 没开始填数，这个填什么都行
            res += dfs(site - 1, newlimit, newfirst, i);
        else // 开始填数了，必须满足 （某些限制条件）|i - dig| >= 2
        {
            if (abs(i - dig) < 2) continue;
            res += dfs(site - 1, newlimit, newfirst, i);
        }
    }

    return dp[site][limit][first][dig] = res;
}

int solve(int x)
{
    cnt = 0;
    while (x)
    {
        num[++cnt] = x % 10;
        x /= 10;
    }
    memset(dp, -1, sizeof(dp));
    return dfs(cnt, 1, 0, 0);
}

signed main()
{
    cin >> a >> b;
    cout << solve(b) - solve(a - 1);
    return 0;
}