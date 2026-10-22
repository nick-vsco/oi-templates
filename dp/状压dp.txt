原题：互不侵犯（8连通块内不能有其他棋子）

#include<bits/stdc++.h>
using namespace std;
#define ll long long
int n, K;
ll dp[15][1026][210], ans;
int nn, i, j, k, l, L;

int get(ll x) {
	int res = 0;
	while (x) {
		if (x & 1) res++;
		x >>= 1;
	}
	return res;
}
bool check(int x, int y) {
	// 判断相邻两行是否合法
	if ((y >> 1)&y)return 1;
	if (x & y) return 1;
	if ((x << 1)&y) return 1;
	if ((x >> 1)&y) return 1;
	return 0;
}

int main() {
	scanf("%d%d", &n, &K);
	dp[0][0][0] = 1;
	nn = (1 << n) - 1;
	for (i = 1; i <= n; i++) { //枚举每一行
		for (j = 0; j <= K; j++) {
			for (k = 0; k <= nn; k++) { // 枚举每一种状态
				L = get(k);
				if (L > j) continue;
				if ((k >> 1)&k) continue; // 本行不合法
				for (l = 0; l <= nn; l++) { // 上一行的状态
					if (check(l, k)) continue;
					dp[i][k][j] += dp[(i - 1) ][l][j - L];
				}
			}

		}
	}
	for (i = 0; i <= nn; i++)
		ans += dp[n][i][K];
	cout << ans;
	return 0;
}