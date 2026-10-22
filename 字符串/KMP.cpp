#include<bits/stdc++.h>
using namespace std;

const int maxn = 2e6 + 10;
string S,T;
int pi[maxn];

int main()
{
	cin >> S >> T;
	int len1 = S.size(), len2 = T.size();
	int len = len1 + len2 + 1;
	S = T + '#' + S;
	for(int i = 1; i < len; i++)
	{
		int j = pi[i - 1];
		while (j && S[i] != S[j]) j = pi[j - 1];
		if (S[i] == S[j]) j++;
		pi[i] = j;
		if(pi[i] == len2)
			cout << i - 2 * len2 + 1 << '\n';
		// 输出的就是匹配的开头的位置（1-index）
	}
	memset(pi, 0, sizeof(pi));
	cout << 0 << ' ';
	for(int i = 1; i < len2; i++)
	{
		int j = pi[i - 1];
		while(j && T[i] != T[j]) j = pi[j - 1];
		if(T[i] == T[j]) j++;
		pi[i] = j;
		cout << pi[i] << ' ';
	}
	return 0;
}