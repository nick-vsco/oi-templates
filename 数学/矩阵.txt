struct Matrix{
	int m[110][110];
}base,ans;

Matrix operator * (const Matrix &x,const Matrix &y)
{
	Matrix z;
	memset(z.m,0,sizeof(z.m));
	for(int i=1;i<=siz;i++)
		for(int j=1;j<=siz;j++)
			for(int k=1;k<=siz;k++)
				z.m[i][j]=(z.m[i][j]+x.m[i][k]*y.m[k][j])%mod;
	return z; 
}

Matrix quick_pow(Matrix a,int b)
{
	Matrix res;
	memset(res.m,0,sizeof(res.m));
	for(int i=1;i<=siz;i++)
		res.m[i][i]=1;
	while(b)
	{
		if(b&1) res=res*a;
		b>>=1;
		a=a*a;
	}
	return res;
}
