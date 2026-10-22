// 三维凸包（增量法）
// 用法：vector<Point3> p 放入点，调用 convexHull3D(p)；
//       返回凸包三角面片（外法向，即从外部看为逆时针），每个面含三个顶点下标。
// 复杂度：期望时间 O(n^2)（增量 + 随机化），空间 O(n)（面数）。
// 说明：点需为一般位置（不大量共面）时最稳；退化数据（大量共面）会退化。

#include <bits/stdc++.h>
using namespace std;

const double eps = 1e-9;
int sgn(double x) { return (x > eps) - (x < -eps); }

struct Point3 {
    double x, y, z;
    Point3() {}
    Point3(double x, double y, double z) : x(x), y(y), z(z) {}
    Point3 operator-(const Point3 &b) const { return Point3(x - b.x, y - b.y, z - b.z); }
    Point3 operator+(const Point3 &b) const { return Point3(x + b.x, y + b.y, z + b.z); }
    Point3 operator*(double k) const { return Point3(x * k, y * k, z * k); }
    double operator*(const Point3 &b) const { return x * b.x + y * b.y + z * b.z; }
    Point3 operator^(const Point3 &b) const {
        return Point3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    }
    double len2() const { return x * x + y * y + z * z; }
};

double vol6(const Point3 &a, const Point3 &b, const Point3 &c, const Point3 &d) {
    return ((b - a) ^ (c - a)) * (d - a);
}

struct Face {
    int a, b, c;
    bool ok;
    Face() {}
    Face(int a, int b, int c) : a(a), b(b), c(c), ok(true) {}
};

vector<Point3> P3;
vector<Face> FS;

bool visible(int fi, const Point3 &x) {
    const Face &f = FS[fi];
    return sgn(vol6(P3[f.a], P3[f.b], P3[f.c], x)) > 0;
}

vector<Face> convexHull3D(vector<Point3> pts) {
    P3 = pts;
    FS.clear();
    int n = P3.size();
    if (n < 4) return FS;

    vector<int> id(n);
    for (int i = 0; i < n; i++) id[i] = i;
    // 随机化输入顺序（保证期望复杂度）；调试时可注释
    mt19937 rng(20041231);
    for (int i = n - 1; i > 0; i--) {
        int j = rng() % (i + 1);
        swap(P3[i], P3[j]);
        swap(id[i], id[j]);
    }

    int i0 = 0, i1 = 1;
    while (i1 < n && sgn((P3[i1] - P3[i0]).len2()) == 0) i1++;
    if (i1 >= n) return FS;
    int i2 = i1 + 1;
    while (i2 < n && sgn(((P3[i1] - P3[i0]) ^ (P3[i2] - P3[i0])).len2()) == 0) i2++;
    if (i2 >= n) return FS;
    int i3 = i2 + 1;
    while (i3 < n && sgn(vol6(P3[i0], P3[i1], P3[i2], P3[i3])) == 0) i3++;
    if (i3 >= n) return FS;

    int A = i0, B = i1, C = i2, D = i3;
    Point3 G = (P3[A] + P3[B] + P3[C] + P3[D]) * 0.25;
    int fv[4][3] = {{A, B, C}, {A, B, D}, {A, C, D}, {B, C, D}};
    for (int k = 0; k < 4; k++) {
        int a = fv[k][0], b = fv[k][1], c = fv[k][2];
        if (sgn(vol6(P3[a], P3[b], P3[c], G)) > 0) swap(b, c);
        FS.push_back(Face(a, b, c));
    }

    for (int pi = 0; pi < n; pi++) {
        if (pi == i0 || pi == i1 || pi == i2 || pi == i3) continue;
        vector<int> vis;
        for (int fi = 0; fi < (int)FS.size(); fi++)
            if (FS[fi].ok && visible(fi, P3[pi])) vis.push_back(fi);
        if (vis.empty()) continue;
        vector<pair<int, int> > contour;
        for (int k = 0; k < (int)vis.size(); k++) {
            int fi = vis[k];
            int v[3] = {FS[fi].a, FS[fi].b, FS[fi].c};
            for (int e = 0; e < 3; e++) {
                int u = v[e], w = v[(e + 1) % 3];
                bool shared = false;
                for (int k2 = 0; k2 < (int)vis.size(); k2++) {
                    if (k2 == k) continue;
                    int g[3] = {FS[vis[k2]].a, FS[vis[k2]].b, FS[vis[k2]].c};
                    for (int t = 0; t < 3; t++)
                        if (g[t] == w && g[(t + 1) % 3] == u) { shared = true; break; }
                    if (shared) break;
                }
                if (!shared) contour.push_back(make_pair(u, w));
            }
            FS[fi].ok = false;
        }
        for (int k = 0; k < (int)contour.size(); k++)
            FS.push_back(Face(contour[k].first, contour[k].second, pi));
    }

    vector<Face> res;
    for (int i = 0; i < (int)FS.size(); i++)
        if (FS[i].ok) {
            Face f;
            f.a = id[FS[i].a];
            f.b = id[FS[i].b];
            f.c = id[FS[i].c];
            f.ok = true;
            res.push_back(f);
        }
    return res;
}