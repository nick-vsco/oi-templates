// 平面最近点对（分治 + 按 y 归并，O(n log n)）
// 返回最近点对的距离平方（开根号即距离）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll INF = (ll)4e18;

struct Point {
    ll x, y;
    Point() {}
    Point(ll x, ll y) : x(x), y(y) {}
    bool operator<(const Point &b) const {
        return x < b.x || (x == b.x && y < b.y);
    }
};

ll dis2(const Point &a, const Point &b) {
    ll dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

vector<Point> pts, tmpPts;

ll solveCP(int l, int r) {
    if (l >= r) return INF;
    int mid = (l + r) >> 1;
    ll midx = pts[mid].x;
    ll d = min(solveCP(l, mid), solveCP(mid + 1, r));
    int i = l, j = mid + 1, k = l;
    while (i <= mid || j <= r) {
        if (j > r || (i <= mid && pts[i].y <= pts[j].y)) tmpPts[k++] = pts[i++];
        else tmpPts[k++] = pts[j++];
    }
    for (int t = l; t <= r; t++) pts[t] = tmpPts[t];
    k = 0;
    for (int t = l; t <= r; t++) {
        ll dx = pts[t].x - midx;
        if (dx * dx < d) tmpPts[k++] = pts[t];
    }
    for (int a = 0; a < k; a++)
        for (int b = a + 1; b < k; b++) {
            ll dy = tmpPts[b].y - tmpPts[a].y;
            if (dy * dy >= d) break;
            d = min(d, dis2(tmpPts[a], tmpPts[b]));
        }
    return d;
}

ll closestPair(vector<Point> p) {
    int n = p.size();
    if (n < 2) return -1;
    pts = p;
    sort(pts.begin(), pts.end());
    tmpPts.resize(n);
    return solveCP(0, n - 1);
}