struct SafeHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    static uint64_t seed() {
        static const uint64_t s =
            std::chrono::steady_clock::now().time_since_epoch().count();
        return s;
    }
    template <class T>
    typename std::enable_if<std::is_integral<T>::value, size_t>::type
    operator()(T x) const {
        return splitmix64(static_cast<uint64_t>(x) + seed());
    }
    size_t operator()(const std::string& s) const {
        return splitmix64(std::hash<std::string>{}(s) + seed());
    }
    size_t operator()(const std::pair<int,int>& p) const {
        uint64_t h = splitmix64(static_cast<uint64_t>(p.first) + seed());
        h ^= splitmix64(static_cast<uint64_t>(p.second) + 0x9e3779b9ULL
                        + (h << 6) + (h >> 2));
        return h;
    }
};