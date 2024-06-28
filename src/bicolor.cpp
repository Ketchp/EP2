#include <bits/stdc++.h>

using namespace std;

//#define int ll

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
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


auto solve(int N, int M) {
    vvi G(N);

    for(int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        G[a].push_back(b);
        G[b].push_back(a);
    }

    vi D(N, -1);

    for(int s = 0; s < N; s++) {
        if(D[s] != -1) continue;

        queue<int> q;
        q.push(s);
        D[s] = 0;

        while(!q.empty()) {
            int c = q.front();
            q.pop();

            int cd = D[c];
            for(auto n: G[c]) {
                if(D[n] == -1) {
                    D[n] = (cd + 1) % 2;
                    q.push(n);
                }
                else if(D[n] == cd)
                    return false;
            }
        }
    }
    return true;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);


    for(int t = 1; true; t++) {
        int n, l;
        cin >> n >> l;
        if(n == 0)
            break;

        cout << (solve(n, l) ? "YES" : "NO") << '\n';
    }

    cout.flush();

    return 0;
}
