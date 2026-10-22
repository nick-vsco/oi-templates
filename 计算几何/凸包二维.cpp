// 二维凸包（Andrew 单调链，最优 O(n log n)）
// 用法：vector<Point> p 放入所有点，调用 convexHull(p)；
//       返回逆时针顺序的凸包顶点（不含共线点）。若要去掉共线点则保持 <= 0，保留共线点改成 < 0。
// 复杂度：时间 O(n log n)（排序主导，扫描 O(n)），空间 O(n)

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
    bool operator<(const Point &b) const {
        return sgn(x - b.x) < 0 || (sgn(x - b.x) == 0 && sgn(y - b.y) < 0);
    }
};

double cross(const Point &o, const Point &a, const Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

vector<Point> convexHull(vector<Point> p) {
    sort(p.begin(), p.end());
    vector<Point> q;
    for (int i = 0; i < (int)p.size(); i++) {
        if (q.empty() || sgn(q.back().x - p[i].x) != 0 || sgn(q.back().y - p[i].y) != 0)
            q.push_back(p[i]);
    }
    p = q;
    int n = p.size();
    if (n <= 1) return p;
    vector<Point> h;
    for (int i = 0; i < n; i++) {
        while (h.size() >= 2 && sgn(cross(h[h.size() - 2], h.back(), p[i])) <= 0)
            h.pop_back();
        h.push_back(p[i]);
    }
    int lower = h.size();
    for (int i = n - 2; i >= 0; i--) {
        while ((int)h.size() > lower && sgn(cross(h[h.size() - 2], h.back(), p[i])) <= 0)
            h.pop_back();
        h.push_back(p[i]);
    }
    h.pop_back();
    return h;
}