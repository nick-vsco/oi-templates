const ull base=13331;
ull a[maxn];
int ans;

// 求一个字符串的 hash
ull get_hash(string s)
{
	ull res=0;
	for(int i=0;i<s.size();i++)
		res=(res*base+int(s[i]));
	return res;
}

// 求所有前缀的hash
ull hs[maxn], p[maxn];
void init(string s)
{
	p[0] = 1;
	for (int i = 1; i <= maxn - 10; i++)
		p[i] = p[i - 1] * base;
	hs[0] = s[0];
	for (int i = 1; i < s.size(); i++)
		hs[i] = hs[i - 1] * base + s[i];
}

ull query(int l, int r)
{
	if (l == 0)
		return hs[r];
	int len = r - l + 1;
	return hs[r] - hs[l - 1] * p[len]
}