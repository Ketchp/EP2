#include <bits/stdc++.h>

using namespace std;

//#define int ll
#define real long double

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;

template<typename T, typename U>
istream &operator>>(istream &is, pair<T, U> &v) {
    return is >> v.first >> v.second;
}

template<typename T>
istream &operator>>(istream &is, vector<T> &v) {
    for(auto &a: v)
        is >> a;
    return is;
}

template<typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    if(v.empty()) return os;
    for(size_t idx = 0; idx < v.size() - 1; idx++)
        os << v[idx] << ' ';
    return os << v.back();
}


auto solve() {
    int n;
    cin >> n;

    vector<pair<real, real>> points(n);
    vector<bool> closed(n);
    cin >> points;

    real res = 0;

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> q;
    q.emplace(0, 0);

    while(!q.empty()) {
        auto [dist, v] = q.top();
        q.pop();
        if(closed[v])
            continue;
        closed[v] = true;

        res += sqrtl(dist);

        auto [x0, y0] = points[v];
        for(size_t idx = 0; idx < n; idx++) {
            if(closed[idx]) continue;
            auto [x1, y1] = points[idx];
            x1 -= x0;
            y1 -= y0;

            q.emplace(x1 * x1 + y1 * y1, idx);
        }
    }

    return res;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;

    for(int t = 1; t <= tc; t++) {
        cout << std::fixed << setprecision(2) << solve() << '\n';
        if(t != tc) cout << '\n';
    }

    cout.flush();

    return 0;
}
