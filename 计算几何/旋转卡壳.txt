// 旋转卡壳：求凸包直径（最远点对），返回两个端点在【原始输入数组】中的下标，时间 O(n log n)
// 用法（推荐一个函数搞定）：
//   vector<Point> p 是原始点；调用 farthestPair(p)，返回 FarthestPair：
//   dist2 为最远点对距离的平方，ia、ib 为这两个点在原始数组 p 中的下标。
//   坐标直接 p[ia]、p[ib]；距离 sqrt(dist2)。
// 分步用法：
//   int id[] 形式：convexHullWithId(p, ids) 得到凸包点 h 和每个凸包点的原始下标 ids，
//   再调用 rotatingCalipers(h, ids)（返回原始下标）。
// 复杂度：凸包 O(n log n)（排序主导），卡壳 O(n)，空间 O(n)

#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-9;
int sgn(double x) { return (x > eps) - (x < -eps); }

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}
    Point operator-(const Point &b) const { return Point(x - b.x, y - b.y); }
    bool operator<(const Point &b) const {
        return sgn(x - b.x) < 0 || (sgn(x - b.x) == 0 && sgn(y - b.y) < 0);
    }
};

double cross(const Point &o, const Point &a, const Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
double dis2(const Point &a, const Point &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

bool cmpPointId(const pair<Point, int> &a, const pair<Point, int> &b) {
    if (a.first < b.first) return true;
    if (b.first < a.first) return false;
    return a.second < b.second;
}

// 带原始下标的二维凸包：h 返回凸包点（逆时针），ids 返回每个凸包点的原始下标
vector<Point> convexHullWithId(const vector<Point> &pts, vector<int> &ids) {
    int n = pts.size();
    vector<pair<Point, int> > a(n);
    for (int i = 0; i < n; i++) a[i] = make_pair(pts[i], i);
    sort(a.begin(), a.end(), cmpPointId);
    vector<pair<Point, int> > uq;
    for (int i = 0; i < n; i++) {
        if (uq.empty() || sgn(uq.back().first.x - a[i].first.x) != 0 || sgn(uq.back().first.y - a[i].first.y) != 0)
            uq.push_back(a[i]);
    }
    vector<Point> h;
    ids.clear();
    int m = uq.size();
    if (m <= 1) {
        for (int i = 0; i < m; i++) { h.push_back(uq[i].first); ids.push_back(uq[i].second); }
        return h;
    }
    vector<int> pos;
    for (int i = 0; i < m; i++) {
        while (h.size() >= 2 && sgn(cross(h[h.size() - 2], h.back(), uq[i].first)) <= 0) {
            h.pop_back();
            pos.pop_back();
        }
        h.push_back(uq[i].first);
        pos.push_back(uq[i].second);
    }
    int lower = h.size();
    for (int i = m - 2; i >= 0; i--) {
        while ((int)h.size() > lower && sgn(cross(h[h.size() - 2], h.back(), uq[i].first)) <= 0) {
            h.pop_back();
            pos.pop_back();
        }
        h.push_back(uq[i].first);
        pos.push_back(uq[i].second);
    }
    h.pop_back();
    pos.pop_back();
    ids = pos;
    return h;
}

struct FarthestPair {
    double dist2;  // 最远点对距离的平方
    int ia, ib;    // 两个端点在【原始输入数组】中的下标
};

// 卡壳：输入凸包 h 与对应原始下标 ids，返回原始下标
FarthestPair rotatingCalipers(const vector<Point> &h, const vector<int> &ids) {
    FarthestPair res;
    res.dist2 = 0;
    res.ia = res.ib = -1;
    int n = h.size();
    if (n == 0) return res;
    res.ia = res.ib = ids[0];
    if (n == 1) return res;
    if (n == 2) {
        res.dist2 = dis2(h[0], h[1]);
        res.ia = ids[0];
        res.ib = ids[1];
        return res;
    }
    int j = 1;
    for (int i = 0; i < n; i++) {
        int ni = (i + 1) % n;
        while (true) {
            int nj = (j + 1) % n;
            double d1 = fabs(cross(h[i], h[ni], h[j]));
            double d2 = fabs(cross(h[i], h[ni], h[nj]));
            if (d2 > d1) j = nj;
            else break;
        }
        double d = dis2(h[i], h[j]);
        if (d > res.dist2) { res.dist2 = d; res.ia = ids[i]; res.ib = ids[j]; }
        d = dis2(h[ni], h[j]);
        if (d > res.dist2) { res.dist2 = d; res.ia = ids[ni]; res.ib = ids[j]; }
    }
    return res;
}

// 封装：直接在原始点数组上求最远点对，返回原始下标
FarthestPair farthestPair(const vector<Point> &pts) {
    vector<int> ids;
    vector<Point> h = convexHullWithId(pts, ids);
    return rotatingCalipers(h, ids);
}