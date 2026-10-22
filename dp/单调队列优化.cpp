// 原题：琪露诺

dp[0] = 0;
	for (int i = l; i <= n; i++)
	{
		while (!p.empty() && dp[p.back()] <= dp[i - l])
			p.pop_back();
		while (!p.empty() && p.front() < i - r)
			p.pop_front();
		p.push_back(i - l);
		dp[i] = dp[p.front()] + a[i];
		if (i + r > n)
			ans = max(ans, dp[i]);
	}