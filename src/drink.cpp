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


struct Node {
    std::array<Node *, 256> n{};
    int idx = -1;

    void insert(const string &s, int i) {
        auto it = s.begin();
        auto c = this;
        while(it != s.end()) {
            if(c->n[*it] == nullptr)
                c->n[*it] = new Node;

            c = c->n[*it];
            it++;
        }
        c->idx = i;
    }

    int operator[](const string &s) const {
        auto c = this;
        auto it = s.begin();
        while(it != s.end()) {
            if(c->n[*it] == nullptr)
                return -1;

            c = c->n[*it];
            it++;
        }

        return c->idx;
    }

    Node() = default;

    Node(const Node &o) {
        copy_from(o);
    }

    Node &operator=(const Node &o) {
        clear();
        copy_from(o);
        return *this;
    }

    void copy_from(const Node &o) {
        for(int j = 0; j < 256; j++)
            if(o.n[j])
                n[j] = new Node(*o.n[j]);
    }

    void clear() {
        for(auto &t: n) {
            delete t;
            t = nullptr;
        }

    }

    ~Node() {
        clear();
    }
};

typedef vector<string> vs;

auto solve() {
    int N, M;
    cin >> N;

    if(N == 0)
        return vs{};

    vs names(N);
    cin >> names;

    Node nt;
    for(int i = 0; i < N; i++)
        nt.insert(names[i], i);

    vvi G(N);
    vi inc(N);

    cin >> M;
    for(int i = 0; i < M; i++) {
        string as, bs;
        cin >> as >> bs;
        int a = nt[as];
        int b = nt[bs];

        G[a].push_back(b);
        inc[b]++;
    }

    vs res;
    priority_queue<int, vi, greater<int>> q({ greater<int>() });

    for(int i = 0; i < N; i++)
        if(inc[i] == 0)
            q.push(i);

    while(!q.empty()) {
        int c = q.top();
        q.pop();

        res.push_back(names[c]);

        for(const auto &n: G[c]) {
            if(!--inc[n])
                q.push(n);
        }
    }

    return res;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);


    for(int t = 1; true; t++) {
        auto b = solve();

        if(b.empty())
            break;

        cout << "Case #" << t
             << ": Dilbert should drink beverages in this order: " << b
             <<".\n";
    }

    cout.flush();

    return 0;
}
