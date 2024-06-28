#include <bits/stdc++.h>

using namespace std;

#define int ll

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

struct State {
    State() = default;
    State(int a, int b, uint8_t k)
        : position{a, b}, keys{k}
    {};

    pii position{0, 0};
    uint8_t keys = 0;

    [[nodiscard]] int y() const {return position.first;}
    [[nodiscard]] int x() const {return position.second;}

    [[nodiscard]] vector<State> ngb() const {
        return {
                {y(), x()+1, keys},
                {y(), x()-1, keys},
                {y()+1, x(), keys},
                {y()-1, x(), keys},
        };
    }
};


#define tile(H, p) (H[p.y()][p.x()])

void enqueue(queue<State> &q, const State &s, vvvi &D, int d) {
    if(tile(D, s)[s.keys] != -1) return;
    tile(D, s)[s.keys] = d;

    for(uint8_t km = 0; km < 16; km++)
        tile(D, s)[s.keys & km] = d;

    q.push(s);
}


auto solve(int H, int W) {
    vector<string> M(H + 2);

    M[0] = M[H+1] = std::string(W+2, '#');

    State initial;
    for(int row = 1; row <= H; row++) {
        string in;
        cin >> in;

        if(initial.position == pii{0, 0})
            for(int col = 0; col <= W; col++) {
                if(in[col] == '*') {
                    initial.position = {row, col + 1};
                    break;
                }
            }
        M[row] = "#" + std::move(in) + "#";
    }

    cin.getline(nullptr, 0);

    vvvi D(H+2, vvi(W+2, vi(16, -1)));

    queue<State> q;
    enqueue(q, initial, D, 0);

    while(!q.empty()) {
        auto c = q.front();
        q.pop();

        if(tile(M, c) == 'X')
            return tile(D, c)[c.keys];

        int cD = tile(D, c)[c.keys] + 1;

        for(auto &n: c.ngb()) {
            if(tile(M, n) == '#')
                continue;

            if(tile(M, n) == 'b')
                n.keys |= 1;
            if(tile(M, n) == 'y')
                n.keys |= 2;
            if(tile(M, n) == 'r')
                n.keys |= 4;
            if(tile(M, n) == 'g')
                n.keys |= 8;

            if(tile(M, n) == 'B' && (n.keys & 1) == 0)
                continue;
            if(tile(M, n) == 'Y' && (n.keys & 2) == 0)
                continue;
            if(tile(M, n) == 'R' && (n.keys & 4) == 0)
                continue;
            if(tile(M, n) == 'G' && (n.keys & 8) == 0)
                continue;

            enqueue(q, n, D, cD);
        }
    }

    return 0ll;
}




auto main() -> signed {
//    cin.tie(nullptr);
//    istream::sync_with_stdio(false);

    while(true) {
        int X, Y;
        cin >> X >> Y;
        if(X == 0 && Y == 0)
            break;

        auto r = solve(X, Y);
        if(r == 0)
            cout << "The poor student is trapped!\n";
        else
            cout << "Escape possible in " << r << " steps.\n";
    }
    cout.flush();
    return 0;
}
