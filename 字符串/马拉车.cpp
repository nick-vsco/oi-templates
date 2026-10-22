#include<bits/stdc++.h>
using namespace std;

const int maxn=3e7+10;
int n;
string s,ss;
int p[maxn];

int manacher(string s)
{
	int len=s.size();
	int res=0;
	for(int c=0,r=0,i=0;i<len;i++)
	{
		int curlen = (i<r ? min(p[2*c-i],r-i) : 1);

		while(i+curlen<len && i-curlen>=0 && s[i+curlen]==s[i-curlen])
			curlen++;
		
		if(i+curlen>r)
		{
			c=i;
			r=i+curlen;
		}
		res=max(res,curlen);
		p[i]=curlen;
	}
	return res-1;
}

int main()
{
	cin>>s;
	n=s.size();
	for(int i=0;i<n;i++)
		ss+='#',ss+=s[i];
	ss+='#';
	cout<<manacher(ss);
	return 0;
}