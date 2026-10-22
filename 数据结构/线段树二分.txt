struct Segment{
    int var[maxn << 2];

    void build(int p, int l, int r)
    {
        if (l == r)
        {
            var[p] = a[l];
            return ;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid);
        build(p << 1 | 1, mid + 1, r);
        var[p] = max(var[p << 1], var[p << 1 | 1]);
    }

    int query(int p, int l, int r, int L, int R, int k)
    {
        if (var[p] <= k) return -1;
        // 一定要优先判断无解
        int mid = (l + r) >> 1;
        if (l == r)    return l;
        else if(L <= l && r <= R)
        {
            if(var[p << 1] > k) return query(p << 1, l, mid, L, R, k);
            else return query(p << 1 | 1, mid + 1, r, L, R, k);
        }
        else
        {
            int res = -1;
            if (L <= mid) res = query(p << 1, l, mid, L, R, k);
            if (res == -1 && mid < R) res = query(p << 1 | 1, mid + 1, r, L, R, k);
            return res;
        }
    }
}T;