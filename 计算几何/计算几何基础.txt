// 计算几何基础：点/向量、有向面积、线段相交、Pick 定理
// 用法：
//   area2(o,a,b)        三角形 OAB 的有向面积的两倍（逆时针为正）
//   polygonArea2(p)     多边形有向面积的两倍（逆时针为正；abs()/2 即面积）
//   segmentIntersect   线段是否相交（含端点接触）
//   pickBoundary(p)    整点多边形的边界整点数 B = sum gcd(|dx|,|dy|)
//   pickInside(p)      整点多边形内部整点数 I = S - B/2 + 1（Pick 定理）
// 复杂度：area2 / 相交 O(1)，Pick 两个函数 O(n)

#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-9;
int sgn(double x) { return (x > eps) - (x < -eps); }

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator+(const Point &b) const { return Point(x + b.x, y + b.y); }
    Point operator-(const Point &b) const { return Point(x - b.x, y - b.y); }
    Point operator*(double k) const { return Point(x * k, y * k); }
    double operator*(const Point &b) const { return x * b.x + y * b.y; }
    double operator^(const Point &b) const { return x * b.y - y * b.x; }
    bool operator<(const Point &b) const {
        return sgn(x - b.x) < 0 || (sgn(x - b.x) == 0 && sgn(y - b.y) < 0);
    }
};

// 三角形 OAB 的有向面积的两倍（逆时针为正）
double area2(const Point &o, const Point &a, const Point &b) {
    return (a - o) ^ (b - o);
}

// 多边形有向面积的两倍（逆时针为正）
double polygonArea2(const vector<Point> &p) {
    double s = 0;
    for (int i = 0; i < (int)p.size(); i++) {
        int j = (i + 1) % p.size();
        s += p[i] ^ p[j];
    }
    return s;
}

// 点 p 是否在线段 ab 上（含端点）
bool onSegment(const Point &a, const Point &b, const Point &p) {
    if (sgn(area2(a, b, p)) != 0) return false;
    return sgn((p - a) * (p - b)) <= 0;
}

// 线段 ab 与 cd 是否相交（含端点接触）
bool segmentIntersect(const Point &a, const Point &b, const Point &c, const Point &d) {
    double c1 = area2(a, b, c), c2 = area2(a, b, d);
    double c3 = area2(c, d, a), c4 = area2(c, d, b);
    if (sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0) return true;
    if (sgn(c1) == 0 && onSegment(a, b, c)) return true;
    if (sgn(c2) == 0 && onSegment(a, b, d)) return true;
    if (sgn(c3) == 0 && onSegment(c, d, a)) return true;
    if (sgn(c4) == 0 && onSegment(c, d, b)) return true;
    return false;
}

// Pick 定理：整点多边形边界整点数 B = sum gcd(|dx|, |dy|)
long long pickBoundary(const vector<Point> &p) {
    long long B = 0;
    for (int i = 0; i < (int)p.size(); i++) {
        int j = (i + 1) % p.size();
        long long dx = llabs((long long)p[i].x - (long long)p[j].x);
        long long dy = llabs((long long)p[i].y - (long long)p[j].y);
        B += __gcd(dx, dy);
    }
    return B;
}

// Pick 定理：内部整点数 I = S - B/2 + 1
long long pickInside(const vector<Point> &p) {
    long long B = pickBoundary(p);
    double S = fabs(polygonArea2(p)) / 2.0;
    return (long long)(S - B / 2.0 + 1 + 0.5);
}