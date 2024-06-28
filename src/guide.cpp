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

void dfs(const vvi &G, vi &in, vi &low, set<int> &art, int current, int parent = -1) {
    if(parent == -1)
        in[current] = 0;
    else
        in[current] = in[parent] + 1;

    for(auto ngb: G[current]) {
        if(in[ngb] == -1) {
            dfs(G, in, low, art, ngb, current);

            if(low[ngb] >= in[current] && parent >= 0)
                art.insert(current);

            if(parent == -1)
                parent = -2;
            else if(parent == -2)
                art.insert(current);

            low[current] = min(low[current], low[ngb]);
        }
        else if(ngb != parent && in[ngb] < in[current]){
            low[current] = min(low[current], in[ngb]);
        }
    }
}

auto solve(int n) {
    vector<string> cities(n);
    cin >> cities;

    vvi G(n);

    int m;
    cin >> m;
    while(m--) {
        string a, b;
        cin >> a >> b;
        auto idx_a = find(all(cities), a) - cities.begin();
        auto idx_b = find(all(cities), b) - cities.begin();

        G[idx_a].emplace_back(idx_b);
        G[idx_b].emplace_back(idx_a);
    }

    vi in(n, -1);
    vi low(n, INT32_MAX);
    set<int> art;

    for(int idx = 0; idx < n; idx++)
        if(in[idx] == -1)
            dfs(G, in, low, art, idx);

    vector<string> res;
    res.reserve(cities.size());

    for(auto idx: art)
        res.emplace_back(std::move(cities[idx]));

    std::sort(res.begin(), res.end());
    return res;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

//    int tc = 1;
//    cin >> tc;

    for(int t = 1; true; t++) {
        int n;
        cin >> n;
        if(n == 0)
            break;

        auto res = solve(n);

        cout << "City map #" << t << ": " << res.size() << " camera(s) found\n";
        for(const auto &v: res)
            cout << v << '\n';
        cout << '\n';
    }

    cout.flush();

    return 0;
}
