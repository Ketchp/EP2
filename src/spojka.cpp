#include <bits/stdc++.h>

using namespace std;

//#define int ll
#define real long double

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
using ll = long long;
using pii = pair<int, int>;
using vpii = vector<pii>;
using vvpii = vector<vpii>;
using vi = vector<int>;
using vvi = vector<vi>;
using vvvi = vector<vvi>;

template<typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
    if(v.empty()) return os;
    for(size_t idx = 0; idx < v.size() - 1; idx++)
        os << v[idx] << ' ';
    return os << v.back();
}

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

auto solve() {
    int n, m;
    cin >> n >> m;
    n++;

    vvi D(n, vi(n, INT32_MAX / 2));

    while(m--) {
        int a, b, d;
        cin >> a >> b >> d;

        D[a][b] = d;
        D[b][a] = d;
    }

    for(int k = 1; k < n; k++)
        for(int i = 1; i < n; i++)
            for(int j = 1; j < n; j++)
                D[i][j] = min(D[i][j], D[i][k] + D[k][j]);

    int T = 0;
    for(int i = 1; i < n; i++)
        for(int j = i+1; j < n; j++)
            T = max(T, D[i][j]);

    return T;
}


auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;
    cout << fixed << setprecision(2);

    for(int t = 1; t <= tc; t++) {
        auto res = solve();

        if(res == INT32_MAX / 2)
            cout << "Bez spojeni neni veleni!\n";
        else
            cout << "Nejvetsi vzdalenost je " << res << ".\n";
    }

    cout.flush();

    return 0;
}
