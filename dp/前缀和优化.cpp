原题：求n的排列中有多少个排列恰好有k个逆序对
转移：dp_{i,j}=dp_{i-1,j}+...+dp_{i-1}{j-i+1}

#include<bits/stdc++.h>
using namespace std;

int n, k;
int dp[1010][1010];// 在 1-i 的排列中，逆序对数为 j 的方案数
const int mod = 10000;

int main() {
	scanf("%d%d", &n, &k);
	dp[1][0] = 1;
	for (int i = 2; i <= n; i++) {
		int sum = 0;
		for (int j = 0; j <= k; j++) {
			sum = (sum + dp[i - 1][j]) % mod;
			dp[i][j] = sum;
			if (j >= i - 1)
				sum = (sum + mod - dp[i - 1][j - i + 1]) % mod;
		}
	}
	cout << dp[n][k];
}