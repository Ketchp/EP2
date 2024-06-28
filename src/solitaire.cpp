#include <bits/stdc++.h>

using namespace std;

//#define int ll

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int8_t, int8_t> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;

template<typename T>
istream &operator>>(istream &is, vector<T> &v) {
    for(auto &a: v)
        is >> a;
    return is;
}

template<typename T, typename U>
istream &operator>>(istream &is, pair<T, U> &v) {
    return is >> v.first >> v.second;
}

template<typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    if(v.empty()) return os;
    for(size_t idx = 0; idx < v.size() - 1; idx++)
        os << v[idx] << ' ';
    return os << v.back();
}

template <typename T>
int8_t sgn(T val) {
    return (T(0) < val) - (val < T(0));
}



/*
 *     1  2  3  4  5  6  7  8
 * 1   0  1  2  3  4  5  6  7
 * 2   8  9 10 11 12 13 14 15
 * 3  16 17 18 19 20 21 22 23
 * 4  24 25 26 27 28 29 30 31
 * 5  32 33 34 35 36 37 38 39
 * 6  40 41 42 43 44 45 46 47
 * 7  48 49 50 51 52 53 54 55
 * 8  56 57 58 59 60 61 62 63
 */

struct Pos : array<pii, 4> {
    using array<pii, 4>::array;

    explicit Pos(const vi &s)
        : array<pii, 4> {pii
            {s[0], s[1]},
            {s[2], s[3]},
            {s[4], s[5]},
            {s[6], s[7]},
        }
    {
        sort(this->begin(), this->end());
    }

    [[nodiscard]] vector<Pos> ngb() const {
        vector<Pos> res;
        auto tc = *this;

        for(auto idx: idc) {
            for(auto [dy, dx]: dd) {
                for(auto mult: mults) {
                    auto tp = tc[idx];
                    tp.first += dy * mult;
                    tp.second += dx * mult;

                    if(tp.first < 1 || tp.first > 8) break;
                    if(tp.second < 1 || tp.second > 8) break;

                    if(!tc.contains(tp)) {
                        res.emplace_back(tc);
                        res.back()[idx] = tp;
                        res.back().bubble(idx);
                        break;
                    }
                }
            }
        }

        return res;
    }

    [[nodiscard]] bool contains(pii v) const {
        return (*this)[0] == v ||
               (*this)[1] == v ||
               (*this)[2] == v ||
               (*this)[3] == v;
    }

    void bubble(int from) {
        while(from > 1 && (*this)[from] < (*this)[from-1]) {
            std::swap((*this)[from], (*this)[from-1]);
            from--;
        }
        while(from < 2 && (*this)[from] > (*this)[from+1]) {
            std::swap((*this)[from], (*this)[from+1]);
            from++;
        }
    }

    static constexpr array<int, 4> idc{0, 1, 2, 3};
    static constexpr array<pii, 4> dd{pii
        {-1,  0},
        { 1,  0},
        { 0, -1},
        { 0,  1},
    };
    static constexpr array<int, 2> mults{1, 2};
};


auto solve(vi &_s, vi &_e) {
    Pos s{_s};
    Pos e{_e};

    if(s == e)
        return true;

    std::map<Pos, int> D;

    queue<Pos> q;
    q.push(s);
    q.push(e);
    D[s] = 1;
    D[e] = -1;

    /*
     * s                              e
     * 0  1   2   3   4   5   6   7   8
     * 1  2   3   4  x5  -4  -3  -2  -1
     *
     * s                                  e
     * 0  1   2   3   4   5   6   7   8   9
     * 1  2   3   4   5  -5  -4  -3  -2  -1
     */

    while(!q.empty()) {
        auto c = q.front();
        q.pop();

        int cd = D[c];
        for(auto n: c.ngb()) {
            if(D.count(n)) {
                if(D[n] * cd < 0 && abs(D[n] + cd) <= 9)
                    return true;
                continue;
            }

            D[n] = cd + sgn(cd);
            if(abs(D[n]) < 5)
                q.push(n);
        }
    }

    return false;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);


    for(int t = 1; true; t++) {
        vi n(8), l(8);
        cin >> n >> l;
        if(cin.eof())
            break;

        cout << (solve(n, l) ? "YES" : "NO") << '\n';
    }

    cout.flush();

    return 0;
}
