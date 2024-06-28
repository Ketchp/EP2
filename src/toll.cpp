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
typedef vector<vpii> vvpii;
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


auto solve(int m) {
    array<vi, 256> G{};
    array<int, 256> D{};

    while(m--) {
        char a, b;
        cin >> a >> b;

        G[a].emplace_back(b);
        G[b].emplace_back(a);
    }

    int num;
    char from, to;
    cin >> num >> from >> to;

    priority_queue<pii, vector<pii>, greater<>> q;
    q.emplace(num, to);

    while(!q.empty()) {
        auto [dist, curr] = q.top();
        q.pop();

        if(D[curr])
            continue;
        D[curr] = dist;

        int dist_next = (curr >= 'a') ? dist + 1 : dist + (dist + 18) / 19;

        for(auto ngb: G[curr])
            if(!D[ngb])
                q.emplace(dist_next, ngb);
    }

    return D[from];
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

//    int tc = 1;
//    cin >> tc;

    for(int t = 1; true; t++) {
        int m;
        cin >> m;
        if(m == -1)
            break;

        auto res = solve(m);

        cout << "Case " << t << ": " << res << '\n';
    }

    cout.flush();

    return 0;
}
