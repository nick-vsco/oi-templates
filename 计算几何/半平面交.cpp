// 半平面交（排序增量 + 双端队列，O(n log n)）
// 用法：每条有向直线 Line(p, v)，v 方向左侧为半平面内部；
//       调用 halfPlaneIntersection(L) 返回交多边形顶点（逆时针），无解返回空 vector
// 复杂度：时间 O(n log n)（排序主导），空间 O(n)

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
};

double cross(const Point &a, const Point &b) { return a.x * b.y - a.y * b.x; }
double cross(const Point &o, const Point &a, const Point &b) { return cross(a - o, b - o); }

struct Line {
    Point p, v;
    double ang;
    Line() {}
    Line(const Point &p, const Point &v) : p(p), v(v) { ang = atan2(v.y, v.x); }
    bool operator<(const Line &b) const {
        if (sgn(ang - b.ang) != 0) return ang < b.ang;
        // 同角度时更靠左（内部更小）的排前面，去重保留它
        return cross(b.p - p, v) > 0;
    }
};

bool onLeft(const Line &l, const Point &p) {
    return sgn(cross(l.v, p - l.p)) >= 0;
}

Point lineIntersection(const Line &a, const Line &b) {
    Point u = a.p - b.p;
    double t = cross(b.v, u) / cross(a.v, b.v);
    return a.p + a.v * t;
}

vector<Point> halfPlaneIntersection(vector<Line> L) {
    sort(L.begin(), L.end());
    vector<Line> lines;
    for (int i = 0; i < (int)L.size(); i++) {
        if (i > 0 && sgn(L[i].ang - L[i - 1].ang) == 0) continue;
        lines.push_back(L[i]);
    }
    int n = lines.size();
    deque<Line> q;
    deque<Point> p;
    for (int i = 0; i < n; i++) {
        while (!p.empty() && !onLeft(lines[i], p.back())) { p.pop_back(); q.pop_back(); }
        while (!p.empty() && !onLeft(lines[i], p.front())) { p.pop_front(); q.pop_front(); }
        q.push_back(lines[i]);
        if (q.size() >= 2) p.push_back(lineIntersection(q[q.size() - 2], q[q.size() - 1]));
    }
    while (!p.empty() && !onLeft(q.front(), p.back())) { p.pop_back(); q.pop_back(); }
    while (!p.empty() && !onLeft(q.back(), p.front())) { p.pop_front(); q.pop_front(); }
    if (p.empty()) return vector<Point>();
    p.push_back(lineIntersection(q.front(), q.back()));
    vector<Point> res;
    while (!p.empty()) { res.push_back(p.front()); p.pop_front(); }
    return res;
}

// 多边形有向面积的两倍（逆时针为正）
double polygonArea2(const vector<Point> &p) {
    double s = 0;
    for (int i = 0; i < (int)p.size(); i++) {
        int j = (i + 1) % p.size();
        s += p[i].x * p[j].y - p[i].y * p[j].x;
    }
    return s;
}

// n 个凸多边形（逆时针给出顶点）的面积交
// 做法：把每个多边形的每条边变成半平面（左侧为内部），做一次半平面交，求交多边形面积
// 复杂度：O(N log N)，N 为所有多边形边数之和
double convexPolygonsIntersectArea(const vector<vector<Point> > &polys) {
    vector<Line> L;
    for (int k = 0; k < (int)polys.size(); k++) {
        const vector<Point> &poly = polys[k];
        int m = poly.size();
        if (m < 3) continue;
        for (int i = 0; i < m; i++) {
            int j = (i + 1) % m;
            L.push_back(Line(poly[i], poly[j] - poly[i]));
        }
    }
    vector<Point> res = halfPlaneIntersection(L);
    if ((int)res.size() < 3) return 0;
    return fabs(polygonArea2(res)) / 2.0;
}

// ===== 完整用法示例：n 个凸多边形的面积交 =====
// 输入格式：
//   第一行一个整数 n，表示凸多边形的个数；
//   接下来 n 组，每组：
//     第一行一个整数 m_i，表示第 i 个多边形的顶点数（边数）；
//     接下来 m_i 行，每行两个整数 x y，逆时针给出该多边形各个顶点的坐标。
// 输出格式：
//   一行一个实数，表示 n 个凸多边形交的面积，保留三位小数。
// 说明：题目给定的多边形外输出按逆时针；若不确定方向，可在读入后用
//       if (polygonArea2(poly) < 0) reverse(poly.begin(), poly.end());
//       反转一次。空交会输出 0.000。
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<vector<Point> > polys(n);
    for (int k = 0; k < n; k++) {
        int m;
        scanf("%d", &m);
        polys[k].resize(m);
        for (int i = 0; i < m; i++)
            scanf("%lf %lf", &polys[k][i].x, &polys[k][i].y);
        // 若题目不保证逆时针，打开下面这行
        // if (polygonArea2(polys[k]) < 0) reverse(polys[k].begin(), polys[k].end());
    }
    printf("%.3f\n", convexPolygonsIntersectArea(polys));
    return 0;
}