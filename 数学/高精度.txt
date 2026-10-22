// 高精度全家桶：加、减、乘、除（除法只实现高精除低精）
// 存储：逆序十进制逐位，a[0] 为个位
// 复杂度：加/减 O(n)，乘 O(n*m)，除低精度 O(n)
#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    vector<int> a;

    BigInt() { a.push_back(0); }
    BigInt(long long x) {
        if (x == 0) a.push_back(0);
        while (x > 0) { a.push_back(x % 10); x /= 10; }
    }
    BigInt(const string &s) {
        for (int i = (int)s.size() - 1; i >= 0; i--) a.push_back(s[i] - '0');
        trim();
    }
    void trim() {
        while (a.size() > 1 && a.back() == 0) a.pop_back();
    }
    bool zero() const { return a.size() == 1 && a[0] == 0; }

    // 三路比较（非负）
    int cmp(const BigInt &b) const {
        if (a.size() != b.a.size()) return a.size() < b.a.size() ? -1 : 1;
        for (int i = (int)a.size() - 1; i >= 0; i--)
            if (a[i] != b.a[i]) return a[i] < b.a[i] ? -1 : 1;
        return 0;
    }
    bool operator<(const BigInt &b) const { return cmp(b) < 0; }
    bool operator==(const BigInt &b) const { return cmp(b) == 0; }

    BigInt operator+(const BigInt &b) const {
        BigInt c; c.a.clear();
        int carry = 0;
        for (size_t i = 0; i < a.size() || i < b.a.size() || carry; i++) {
            int s = carry;
            if (i < a.size()) s += a[i];
            if (i < b.a.size()) s += b.a[i];
            c.a.push_back(s % 10);
            carry = s / 10;
        }
        c.trim();
        return c;
    }

    // 要求 *this >= b
    BigInt operator-(const BigInt &b) const {
        BigInt c; c.a.clear();
        int borrow = 0;
        for (size_t i = 0; i < a.size(); i++) {
            int s = a[i] - borrow - (i < b.a.size() ? b.a[i] : 0);
            if (s < 0) { s += 10; borrow = 1; } else borrow = 0;
            c.a.push_back(s);
        }
        c.trim();
        return c;
    }

    BigInt operator*(const BigInt &b) const {
        if (zero() || b.zero()) return BigInt(0);
        BigInt c;
        c.a.assign(a.size() + b.a.size(), 0);
        for (size_t i = 0; i < a.size(); i++) {
            long long carry = 0;
            for (size_t j = 0; j < b.a.size() || carry; j++) {
                long long cur = c.a[i + j] + carry;
                if (j < b.a.size()) cur += (long long)a[i] * b.a[j];
                c.a[i + j] = cur % 10;
                carry = cur / 10;
            }
        }
        c.trim();
        return c;
    }

    // 高精除低精：返回 (商, 余数)，要求 d > 0
    pair<BigInt, long long> divmod(long long d) const {
        BigInt q;
        q.a.assign(a.size(), 0);
        long long r = 0;
        for (int i = (int)a.size() - 1; i >= 0; i--) {
            r = r * 10 + a[i];
            q.a[i] = r / d;
            r %= d;
        }
        q.trim();
        return make_pair(q, r);
    }

    string str() const {
        string s;
        for (int i = (int)a.size() - 1; i >= 0; i--) s += char('0' + a[i]);
        return s;
    }
};