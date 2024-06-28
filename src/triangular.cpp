#include <bits/stdc++.h>

using namespace std;

#define int ll

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

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

string bad_fig(const string &i) {return i + " are not the vertices of an acceptable figure";}
string is_triangle(const string &i) {return i + " are the vertices of a triangle";}
string is_parallelogram(const string &i) {return i + " are the vertices of a parallelogram";}
string is_hexagon(const string &i) {return i + " are the vertices of a hexagon";}

struct P {
    explicit P(int p)
        : p(p),
          line((int)((1+sqrt(8 * (double)p - 7)) / 2)),
          start(1 + line * (line-1) / 2),
          offset(p-start)
    {};

    int p, line, start, offset;
};

int dist(const P &a, const P &b) {
    if(a.line == b.line)
        return abs(a.offset - b.offset);
    if(a.offset == b.offset)
        return abs(a.line - b.line);
    if(a.line - a.offset == b.line - b.offset)
        return abs(a.line - b.line);
    return 0;
}

bool all_dist_same(const std::vector<P> &pts) {
    int d = dist(pts.front(), pts.back());
    for(int i = 1; i < pts.size(); i++)
        if(dist(pts[i-1], pts[i]) != d)
            return false;
    return d > 0;
}

bool on_line(const P &a, const P &b, const P &c) {
    return ((a.line == b.line && b.line == c.line) ||
            (a.offset == b.offset && b.offset == c.offset) ||
            (a.line - a.offset == b.line - b.offset && b.line - b.offset == c.line - c.offset));
}

auto solve() {
    constexpr int BS = 100'000;
    char buff[BS];
    cin.getline(buff, BS);
    if(!buff[0])
        return string{""};

    string input(buff);

    stringstream ss{buff};

    vi points;
    while(ss) {
        int temp;
        ss >> temp;
        points.push_back(temp);
    }
    points.pop_back();

    if(points.size() != 3 &&
       points.size() != 4 &&
       points.size() != 6)
        return bad_fig(input);

    vector<P> pd;
    for(auto p: points)
        pd.emplace_back(p);

    std::sort(pd.begin(), pd.end(), [](const P &a, const P &b){
        if(a.line != b.line) return a.line < b.line;
        return a.offset < b.offset;
    });

    if(pd.size() == 3) {
        if(all_dist_same(pd))
            return is_triangle(input);
    }
    else if(pd.size() == 4) {
        swap(pd[2], pd[3]);
        if(all_dist_same(pd))
            return is_parallelogram(input);
    }
    else {
        swap(pd[2], pd[3]);
        swap(pd[3], pd[5]);
        if(all_dist_same(pd) &&
            dist(pd[0], pd[3]) == dist(pd[1], pd[4]) &&
            dist(pd[1], pd[4]) == dist(pd[2], pd[5])
            )
            return is_hexagon(input);
    }

    return bad_fig(input);
}


auto main() -> signed {
//    cin.tie(nullptr);
//    istream::sync_with_stdio(false);

    while(true) {
        auto r = solve();
        if(r.empty()) break;
        cout << r << '\n';
    }
    cout.flush();
    return 0;
}
