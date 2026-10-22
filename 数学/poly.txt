#include<bits/stdc++.h>
using namespace std;
#define int long long

// ============================================================
// 多项式全家桶板子（一档 + 二档 + 姊妹变换）
// 本文件依赖 #define int long long，不要删
// 模数看下面的全局 const mod（默认 998244353，原根 3）；换模数直接改那一行
// 3 对 998244353 / 1004535809 / 469762049 都是原根
// 函数清单（各函数注释里写了用途和前提条件）：
//   mul            模板1  卷积 / 多项式乘法
//   mtt            模板2  任意模数卷积（三模 NTT + CRT）
//   inv            模板3  多项式求逆
//   ln / exp       模板4/5  多项式对数 / 指数
//   sqrt           模板6  多项式开方（二档）
//   div / rem      模板7  多项式除法 / 取余（二档）
//   kth            模板8  常系数线性递推第 k 项，k 到 1e18（二档）
//   multipoint     模板9  多点求值（二档）
//   interp         模板10  插值（二档）
//   cdq            模板11  CDQ 分治解在线递推
//   fwt            模板12  FWT 与 / 或 / 异或卷积（姊妹变换）
// ============================================================
const int mod = 998244353;
struct Poly{
    int  g;   // 原根（模数是全局 const mod）

    Poly(int _g = 3) : g(_g) {}

    int qpow(int a, int b, int p)
    {
        int res = 1 % p;
        a %= p;
        while (b)
        {
            if (b & 1) res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }

    // 去掉末尾多余的零
    vector<int> trim(vector<int> a)
    {
        while (a.size() > 1 && a.back() == 0) a.pop_back();
        return a;
    }

    // NTT 原地变换。op=1 正变换，op=-1 逆变换
    // 前提：a 的长度是 2 的幂，且长度整除 (p-1)
    void ntt(vector<int> &a, int op, int p, int gr)
    {
        int n = a.size();
        vector<int> rev(n);
        for (int i = 0; i < n; i++)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
        for (int i = 0; i < n; i++)
            if (i < rev[i]) swap(a[i], a[rev[i]]);
        for (int len = 2; len <= n; len <<= 1)
        {
            int w = qpow(gr, (p - 1) / len, p);
            if (op == -1) w = qpow(w, p - 2, p);
            for (int i = 0; i < n; i += len)
            {
                int wn = 1;
                for (int j = 0; j < (len >> 1); j++, wn = wn * w % p)
                {
                    int u = a[i + j], v = a[i + j + (len >> 1)] * wn % p;
                    a[i + j] = (u + v) % p;
                    a[i + j + (len >> 1)] = (u - v + p) % p;
                }
            }
        }
        if (op == -1)
        {
            int invn = qpow(n, p - 2, p);
            for (int i = 0; i < n; i++) a[i] = a[i] * invn % p;
        }
    }

    // 模板1：卷积 / 多项式乘法（当前模数下，默认 998244353）
    // 用途：c[k] = Σ a[i]*b[k-i]，几乎所有多项式题的地基
    // 长度上限：结果长度 ≤ 2^23
    vector<int> mul(vector<int> a, vector<int> b)
    {
        if (a.empty() || b.empty()) return {};
        int n = a.size() + b.size() - 1, len = 1;
        while (len < n) len <<= 1;
        a.resize(len);
        b.resize(len);
        ntt(a, 1, mod, g);
        ntt(b, 1, mod, g);
        for (int i = 0; i < len; i++) a[i] = a[i] * b[i] % mod;
        ntt(a, -1, mod, g);
        a.resize(n);
        return a;
    }

    // 模板2：任意模数卷积（三模 NTT + CRT）
    // 用途：模数是 1e9+7 等任意模数时的卷积；参数 p 是题目模数
    // 长度上限：结果长度 ≤ 2^21（受 1004535809 限制）；比 mul 慢，只有模数不对时才用它
    vector<int> mtt(vector<int> a, vector<int> b, int p)
    {
        if (a.empty() || b.empty()) return {};
        int n = a.size() + b.size() - 1, len = 1;
        while (len < n) len <<= 1;
        int M1 = 998244353, M2 = 1004535809, M3 = 469762049;
        vector<int> x(len), y(len), r(len);
        // 第一个模
        for (int i = 0; i < a.size(); i++) x[i] = (a[i] % M1 + M1) % M1;
        for (int i = 0; i < b.size(); i++) y[i] = (b[i] % M1 + M1) % M1;
        ntt(x, 1, M1, 3);
        ntt(y, 1, M1, 3);
        for (int i = 0; i < len; i++) x[i] = x[i] * y[i] % M1;
        ntt(x, -1, M1, 3);
        for (int i = 0; i < n; i++) r[i] = x[i];
        // 第二个模，CRT 合并：真值 = r + M1 * t
        fill(x.begin(), x.end(), 0);
        fill(y.begin(), y.end(), 0);
        int t1 = qpow(M1 % M2, M2 - 2, M2);
        for (int i = 0; i < a.size(); i++) x[i] = (a[i] % M2 + M2) % M2;
        for (int i = 0; i < b.size(); i++) y[i] = (b[i] % M2 + M2) % M2;
        ntt(x, 1, M2, 3);
        ntt(y, 1, M2, 3);
        for (int i = 0; i < len; i++) x[i] = x[i] * y[i] % M2;
        ntt(x, -1, M2, 3);
        for (int i = 0; i < n; i++)
        {
            int t = (x[i] - r[i] + M2) % M2 * t1 % M2;
            r[i] = r[i] + M1 * t;
        }
        // 第三个模：答案 = (r + M1*M2*t) mod p
        fill(x.begin(), x.end(), 0);
        fill(y.begin(), y.end(), 0);
        int t2 = qpow((__int128)M1 * M2 % M3, M3 - 2, M3);
        int mm = (__int128)M1 * M2 % p;
        for (int i = 0; i < a.size(); i++) x[i] = (a[i] % M3 + M3) % M3;
        for (int i = 0; i < b.size(); i++) y[i] = (b[i] % M3 + M3) % M3;
        ntt(x, 1, M3, 3);
        ntt(y, 1, M3, 3);
        for (int i = 0; i < len; i++) x[i] = x[i] * y[i] % M3;
        ntt(x, -1, M3, 3);
        vector<int> ans(n);
        for (int i = 0; i < n; i++)
        {
            int t = (x[i] - r[i] % M3 + M3) % M3 * t2 % M3;
            ans[i] = (r[i] % p + (__int128)mm * t % p) % p;
        }
        return ans;
    }

    // 模板3：多项式求逆。给 a，返回 B 的前 n 项，满足 a*B ≡ 1 (mod x^n)
    // 前提：a[0] != 0。用途：递推生成函数、任意段拼接计数、除法和 ln 的地基
    vector<int> inv(vector<int> a, int n)
    {
        vector<int> b(1, qpow(a[0] % mod, mod - 2, mod));
        for (int m = 2; m < (n << 1); m <<= 1)
        {
            int len = 1;
            while (len < (m << 1)) len <<= 1;
            vector<int> f(len), gb(len);
            for (int i = 0; i < m && i < a.size(); i++) f[i] = (a[i] % mod + mod) % mod;
            for (int i = 0; i < (m >> 1); i++) gb[i] = b[i];
            ntt(f, 1, mod, g);
            ntt(gb, 1, mod, g);
            for (int i = 0; i < len; i++)
                gb[i] = gb[i] * ((2 - f[i] * gb[i] % mod + mod) % mod) % mod;
            ntt(gb, -1, mod, g);
            gb.resize(m);
            b = gb;
        }
        b.resize(n);
        return b;
    }

    // 求导（ln 和插值的零件）
    vector<int> deriv(vector<int> a)
    {
        if (a.size() <= 1) return {0};
        for (int i = 0; i + 1 < a.size(); i++) a[i] = a[i + 1] * (i + 1) % mod;
        a.pop_back();
        return a;
    }

    // 积分（ln 的零件）
    vector<int> integ(vector<int> a)
    {
        int n = a.size();
        vector<int> invs(n + 1);
        if (n >= 1) invs[1] = 1;
        for (int i = 2; i <= n; i++) invs[i] = (mod - mod / i) * invs[mod % i] % mod;
        a.push_back(0);
        for (int i = n; i >= 1; i--) a[i] = a[i - 1] * invs[i] % mod;
        a[0] = 0;
        return a;
    }

    // 模板4：多项式对数。返回 $\ln$ (a) = $\int$ a'/a 的前 n 项
    // 前提：a[0] == 1。用途：带标号计数里"全体方案 -> 连通方案"
    vector<int> ln(vector<int> a, int n)
    {
        a.resize(n);
        vector<int> res = integ(mul(deriv(a), inv(a, n)));
        res.resize(n);
        return res;
    }

    // 模板5：多项式指数。返回 e^a 的前 n 项
    // 前提：a[0] == 0。用途：带标号计数里"任取连通分量拼成整体"
    vector<int> exp(vector<int> a, int n)
    {
        a.resize(n);
        vector<int> b(1, 1);
        for (int m = 2; m < (n << 1); m <<= 1)
        {
            vector<int> t = ln(b, m);
            for (int i = 0; i < m; i++) t[i] = (a[i] - t[i] + mod) % mod;
            t[0] = (t[0] + 1) % mod;
            b = mul(b, t);
            b.resize(m);
        }
        b.resize(n);
        return b;
    }

    // 模板6（二档）：多项式开方。返回 B 的前 n 项，满足 B^2 ≡ a (mod x^n)
    // 前提：a[0] == 1。用途：二次型函数方程、无向结构计数
    vector<int> sqrt(vector<int> a, int n)
    {
        a.resize(n);
        vector<int> b(1, 1);
        int inv2 = qpow(2, mod - 2, mod);
        for (int m = 2; m < (n << 1); m <<= 1)
        {
            vector<int> t = mul(inv(b, m), a);
            t.resize(m);
            b.resize(m);
            for (int i = 0; i < m; i++) b[i] = (b[i] + t[i]) % mod * inv2 % mod;
        }
        b.resize(n);
        return b;
    }

    // 模板7（二档）：多项式除法，返回商 Q（F = G*Q + R）
    // 前提：g 末位（最高次系数）!= 0
    vector<int> div(vector<int> f, vector<int> g)
    {
        f = trim(f);
        g = trim(g);
        int n = f.size(), m = g.size();
        if (n < m) return {0};
        reverse(f.begin(), f.end());
        reverse(g.begin(), g.end());
        int k = n - m + 1;
        f.resize(k);
        g.resize(k);
        vector<int> q = mul(f, inv(g, k));
        q.resize(k);
        reverse(q.begin(), q.end());
        return q;
    }

    // 模板7（二档）：多项式取余，返回 R = F - G*Q
    vector<int> rem(vector<int> f, vector<int> g)
    {
        vector<int> q = div(f, g), t = mul(g, q);
        vector<int> r = f;
        for (int i = 0; i < r.size() && i < t.size(); i++) r[i] = (r[i] - t[i] + mod) % mod;
        return trim(r);
    }

    // 模板8（二档）：常系数线性递推第 k 项，k 可以到 1e18
    // 递推：a[n] = c[0]*a[n-1] + c[1]*a[n-2] + ... + c[d-1]*a[n-d]
    // init 给 a[0..d-1]，返回 a[k]
    int kth(vector<int> c, vector<int> init, int k)
    {
        int d = c.size();
        if (k < d) return init[k] % mod;
        vector<int> chi(d + 1);
        chi[d] = 1;
        for (int i = 0; i < d; i++) chi[i] = (mod - c[d - 1 - i] % mod) % mod;
        vector<int> r = {1}, x = {0, 1};
        while (k)
        {
            if (k & 1) r = rem(mul(r, x), chi);
            x = rem(mul(x, x), chi);
            k >>= 1;
        }
        r.resize(d);
        int ans = 0;
        for (int i = 0; i < d; i++) ans = (ans + r[i] * init[i]) % mod;
        return ans;
    }

    // 模板9（二档）：多点求值。给多项式 f 和点集 xs，返回所有 f(xs[i])
    // 复杂度 O(n log^2 n)。tr 是内部用的乘积树，不用管
    vector<vector<int> > tr;

    void build(int p, int l, int r, vector<int> &xs)
    {
        if (l == r)
        {
            tr[p] = {(mod - xs[l] % mod) % mod, 1};
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, xs);
        build(p << 1 | 1, mid + 1, r, xs);
        tr[p] = mul(tr[p << 1], tr[p << 1 | 1]);
    }

    void evalrec(int p, int l, int r, vector<int> f, vector<int> &xs, vector<int> &res)
    {
        if (l == r)
        {
            res[l] = f.empty() ? 0 : f[0] % mod;
            return;
        }
        int mid = (l + r) >> 1;
        evalrec(p << 1, l, mid, rem(f, tr[p << 1]), xs, res);
        evalrec(p << 1 | 1, mid + 1, r, rem(f, tr[p << 1 | 1]), xs, res);
    }

    vector<int> multipoint(vector<int> f, vector<int> xs)
    {
        int n = xs.size();
        tr.assign(n << 2, vector<int>());
        build(1, 0, n - 1, xs);
        vector<int> res(n);
        evalrec(1, 0, n - 1, rem(f, tr[1]), xs, res);
        return res;
    }

    // 模板10（二档）：插值。给点集 (xs[i], ys[i])，返回次数 ≤ n-1 的多项式 P 满足 P(xs[i]) = ys[i]
    // 前提：xs 两两不同
    vector<int> solve(int p, int l, int r, vector<int> &w)
    {
        if (l == r) return {w[l]};
        int mid = (l + r) >> 1;
        vector<int> L = solve(p << 1, l, mid, w);
        vector<int> R = solve(p << 1 | 1, mid + 1, r, w);
        vector<int> a = mul(L, tr[p << 1 | 1]), b = mul(R, tr[p << 1]);
        if (a.size() < b.size()) a.resize(b.size());
        for (int i = 0; i < b.size(); i++) a[i] = (a[i] + b[i]) % mod;
        return a;
    }

    vector<int> interp(vector<int> xs, vector<int> ys)
    {
        int n = xs.size();
        tr.assign(n << 2, vector<int>());
        build(1, 0, n - 1, xs);
        vector<int> dv(n);
        evalrec(1, 0, n - 1, rem(deriv(tr[1]), tr[1]), xs, dv);
        vector<int> w(n);
        for (int i = 0; i < n; i++) w[i] = ys[i] % mod * qpow(dv[i], mod - 2, mod) % mod;
        return solve(1, 0, n - 1, w);
    }

    // 模板11：CDQ 分治 + NTT，解在线递推 f_i = h_i + Σ_{j<i} f_j * g_{i-j}
    // 用途：递推两边都有 f、生成函数求逆不顺手时的固定套路
    // 注意：g[0] 不会被用到（可置 0）；下面的 cdq 是封装，直接调它
    void cdqrec(int l, int r, vector<int> &f, vector<int> &g, vector<int> &h)
    {
        if (l == r)
        {
            f[l] = (f[l] + h[l]) % mod;
            return;
        }
        int mid = (l + r) >> 1;
        cdqrec(l, mid, f, g, h);
        vector<int> a(f.begin() + l, f.begin() + mid + 1);
        int k = min((int)g.size(), r - l + 1);
        vector<int> b(g.begin(), g.begin() + k);
        vector<int> c = mul(a, b);
        for (int i = mid + 1; i <= r; i++)
            if (i - l < c.size()) f[i] = (f[i] + c[i - l]) % mod;
        cdqrec(mid + 1, r, f, g, h);
    }

    vector<int> cdq(vector<int> g, vector<int> h)
    {
        vector<int> f(h.size(), 0);
        if (!h.empty()) cdqrec(0, (int)h.size() - 1, f, g, h);
        return f;
    }

    // 模板12（姊妹变换）：FWT。type：0=异或 1=或 2=与；op：1 正变换 -1 逆变换
    // 用途：位运算卷积。用法：两边正变换 -> 点乘 -> 逆变换。数组长度须是 2 的幂
    void fwt(vector<int> &a, int type, int op)
    {
        int n = a.size(), inv2 = qpow(2, mod - 2, mod);
        for (int len = 2; len <= n; len <<= 1)
            for (int i = 0; i < n; i += len)
                for (int j = i; j < i + (len >> 1); j++)
                {
                    int u = a[j] % mod, v = a[j + (len >> 1)] % mod;
                    if (type == 0)
                    {
                        if (op == 1)
                        {
                            a[j] = (u + v) % mod;
                            a[j + (len >> 1)] = (u - v + mod) % mod;
                        }
                        else
                        {
                            a[j] = (u + v) % mod * inv2 % mod;
                            a[j + (len >> 1)] = (u - v + mod) % mod * inv2 % mod;
                        }
                    }
                    else if (type == 1)
                    {
                        if (op == 1) a[j + (len >> 1)] = (u + v) % mod;
                        else a[j + (len >> 1)] = (v - u + mod) % mod;
                    }
                    else
                    {
                        if (op == 1) a[j] = (u + v) % mod;
                        else a[j] = (u - v + mod) % mod;
                    }
                }
    }
}T;



signed main()
{
    
    // 用法示例：
    // vector<int> a = {...}, b = {...};
    // vector<int> c = T.mul(a, b);              // NTT卷积（模 998244353）
    // vector<int> d = T.mtt(a, b, 1000000007);  // 任意模数卷积
    // vector<int> e = T.inv(a, n);              // 求逆，前 n 项
    // vector<int> q = T.exp(b, n);              // exp，要求 b[0] == 0
    // 题目模数不是 998244353 但是 NTT 友好模数时（友好模数：p=c2^k+1，ck是某个整数）：
    // 直接改全局 const mod（3 对 1004535809 / 469762049 也是原根）

    string a, b;
    vector<int>f, g, h;
    cin >> a >> b;
    for (int i = 0; i < a.size(); i++)
        g.push_back(a[i] - '0');
    for (int i = 0; i < b.size(); i++)
        h.push_back(b[i] - '0');
    f = T.mul(g, h);
    for (int i = f.size() - 1; i > 0; i--)
    {
        f[i - 1] += f[i] / 10;
        f[i] %= 10;
    }
    int st = 0;
    while (st != f.size() && f[st] == 0)
        st++;
    if (st == f.size()) cout << 0;
    else
    {
        for (int i = st; i < f.size(); i++)
            cout << f[i];
    }
    
    return 0;
}
