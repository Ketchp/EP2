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


auto solve() {
    int N;
    cin >> N;

    int Z = 'z' + 1;

    vvi G(Z, vi(Z));

    while(N--) {
        string word;
        cin >> word;

        G[word.front()][word.back()]++;
    }

    vi I(Z), O(Z);

    for(char from = 'a'; from <= 'z'; from++) {
        for(char to = 'a'; to <= 'z'; to++) {
            I[to] += G[from][to];
            O[from] += G[from][to];
        }
    }

    char s, e, p;
    s = e = p = '\0';
    for(char c = 'a'; c <= 'z'; c++) {
        if(abs(I[c] - O[c]) > 1)
            return false;

        if(O[c] > 0) p = c;

        if(I[c] > O[c]) {
            if(e) return false;
            e = c;
        }
        if(O[c] > I[c]) {
            if(s) return false;
            s = c;
        }
    }

    vector<bool> visited(Z);
    queue<char> q;
    if(s)
        q.push(s);
    else
        q.push(p);
    visited[q.front()] = true;

    while(!q.empty()) {
        char c = q.front();
        q.pop();

        for(char ngb = 'a'; ngb <= 'z'; ngb++) {
            if(visited[ngb])
                continue;
            if(G[c][ngb] == 0)
                continue;
            q.push(ngb);
            visited[ngb] = true;
        }
    }

    for(char c = 'a'; c <= 'z'; c++) {
        if(!visited[c] && (I[c] != 0 || O[c] != 0))
            return false;
    }
    return true;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;

    for(int t = 1; t <= tc; t++) {
        cout << (solve() ? "Ordering is possible.\n" : "The door cannot be opened.\n");
    }

    cout.flush();

    return 0;
}
