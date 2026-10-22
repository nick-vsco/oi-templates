// 三角剖分（耳切法 Ear Clipping）
// 用法：vector<Point> poly 为逆时针简单多边形（不自交），调用 triangulate(poly)；
//       返回三角形列表，每个三角形按逆时针给出 3 个顶点，共 n-2 个三角形。
// 复杂度：时间 O(n^2)（每个耳 O(n)，共 n 次），空间 O(n)。
// 说明：简单多边形三角剖分的理论最优是 O(n log n)，但实现复杂；竞赛用耳切法足够。

#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-9;
int sgn(double x) { return (x > eps) - (x < -eps); }

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
};

double cross(const Point &o, const Point &a, const Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

// 判断 p 是否在三角形 abc（逆时针）内部或边界上
bool inTriangle(const Point &a, const Point &b, const Point &c, const Point &p) {
    return sgn(cross(a, b, p)) >= 0 && sgn(cross(b, c, p)) >= 0 && sgn(cross(c, a, p)) >= 0;
}

vector<array<Point, 3>> triangulate(vector<Point> poly) {
    vector<array<Point, 3>> res;
    int n = poly.size();
    if (n < 3) return res;
    vector<int> idx;
    for (int i = 0; i < n; i++) idx.push_back(i);
    while ((int)idx.size() > 3) {
        bool found = false;
        int m = idx.size();
        for (int i = 0; i < m; i++) {
            int prev = (i + m - 1) % m, next = (i + 1) % m;
            if (sgn(cross(poly[idx[prev]], poly[idx[i]], poly[idx[next]])) <= 0) continue;
            bool ear = true;
            for (int j = 0; j < m; j++) {
                if (j == prev || j == i || j == next) continue;
                if (inTriangle(poly[idx[prev]], poly[idx[i]], poly[idx[next]], poly[idx[j]])) {
                    ear = false;
                    break;
                }
            }
            if (!ear) continue;
            array<Point, 3> t;
            t[0] = poly[idx[prev]]; t[1] = poly[idx[i]]; t[2] = poly[idx[next]];
            res.push_back(t);
            idx.erase(idx.begin() + i);
            found = true;
            break;
        }
        if (!found) break;
    }
    if ((int)idx.size() == 3) {
        array<Point, 3> t;
        t[0] = poly[idx[0]]; t[1] = poly[idx[1]]; t[2] = poly[idx[2]];
        res.push_back(t);
    }
    return res;
}