输入一个 N，判定其是否为素数（求其最大质因子）时间复杂度O(N^0.25)

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef __int128 lll;

ll RandInt(ll l , ll r) {
	static mt19937 Rand(time(0));
	uniform_int_distribution<ll> dis(l, r);
	return dis(Rand);
}

ll Quick_Power(ll a , ll b , ll p) {
	ll res = 1;
	while(b) {
		if(b & 1) res = (lll)res * a % p;
		a = (lll)a * a % p;
		b >>= 1;
	}
	return res;
}

bool Miller_Rabin(ll n) {
	if(n < 3 || n % 2 == 0) return n == 2;
	ll a = n - 1 , b = 0;
	while(a % 2 == 0) {
		a /= 2;
		b++;
	}
	for(int i = 1 , j; i <= 10; i++) {
		ll x = RandInt(2 , n - 1);
		ll v = Quick_Power(x , a , n);
		if(v == 1) continue;
		for(j = 0; j < b; j++) {
			if(v == n - 1) break;
			v = (lll) v * v % n;
		}
		if(j == b) return 0;
	}
	return 1; 
}

ll F(ll x , ll c , ll n) {
	return ((lll)x * x + c) % n;
}

ll Pollard_Rho(ll n) {
	ll s = 0 , t = 0;
	ll c = RandInt(1 , n - 1);
	int step = 0 , goal = 1;
	ll value = 1;
	for(goal = 1;; goal <<= 1, s = t , value = 1) {
		for(step = 1; step <= goal; step++) {
			t = F(t , c , n);
			value = ((lll)value * abs(t - s)) % n;
			if(step % 127 == 0) {
				ll d = __gcd(value , n);
				if(d > 1) return d;
			}
		}
		ll d = __gcd(value , n);
		if(d > 1) return d;
	}
}

ll Ans;

void Fac(ll n) {
	if(n <= Ans || n < 2) return;
	if(Miller_Rabin(n)) {
		Ans = max(Ans , n);
		return;
	}
	ll p = n;
	while(p == n) p = Pollard_Rho(n);
	while((n % p) == 0) n /= p;
	Fac(n);
	Fac(p); 
}

ll T , N;

int main() {
	cin >> T;
	while(T--) {
		Ans = 0;
		cin >> N;
		Fac(N);
		if(Ans == N) {
			cout << "Prime" << endl;
		}
		else {
			cout << Ans << endl;
		}
	}
	return 0;
} 
