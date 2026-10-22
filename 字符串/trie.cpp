const int maxn=1e6+10;
int trie[maxn][40];
int cnt;

void build(string s)
{
	int p=0;
	for(int i=0;i<s.size();i++)
	{
		int c=s[i]-'A';
		if(!trie[p][c])
			trie[p][c]=(++cnt);
		p=trie[p][c];
	}
}