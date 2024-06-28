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


auto grid_ngb = {tuple{-1, 0, 0}, {1, 0, 0},
                      {0, -1, 0}, {0, 1, 0},
                      {0, 0, -1}, {0, 0, 1}};

int bfs(vector<vector<vector<bool>>> &cave,
        tuple<int, int, int> start,
        tuple<int, int, int> size
) {
    auto [x, y, z] = start;
    auto [X, Y, Z] = size;

    if(!cave[z][y][x])
        return 0;
    cave[z][y][x] = false;

    int count = 1;
    queue<tuple<int, int, int>> q;
    q.push(start);

    while(!q.empty()) {
        auto [cx, cy, cz] = q.front();
        q.pop();

        for(auto [dx, dy, dz]: grid_ngb) {
            auto nx = cx + dx,
                 ny = cy + dy,
                 nz = cz + dz;
            if (nx == -1 || nx == X) continue;
            if (ny == -1 || ny == Y) continue;
            if (nz == -1 || nz == Z) continue;
            if (!cave[nz][ny][nx]) continue;
            cave[nz][ny][nx] = false;
            count++;
            q.emplace(nx, ny, nz);
        }
    }
    return count;
}

auto solve() {
    int X, Y, Z;
    cin >> X >> Y >> Z;

    vector<vector<vector<bool>>> cave(Z, vector<vector<bool>>(Y, vector<bool>(X)));

    for(int z = 0; z < Z; z++) {
        int P = 0;
        cin >> P;
        for(int i = 0; i < P; i++) {
            int R, S;
            cin >> R >> S;
            cave[z][S-1][R-1] = true;
        }
    }

    int result = 0;
    for(int y = 0; y < Y; y++)
        for(int x = 0; x < X; x++)
            result += bfs(cave, {x, y, 0}, {X, Y, Z});
    return result;
}

auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;

    for(int t = 1; t <= tc; t++) {
//        solve();
//        solve(); cout << '\n';
        cout << "Je nutne vycerpat " << 1000 * solve() << " litru vody.\n";
//        cout << (solve() ? "YES\n" : "NO\n");
    }
    cout.flush();
    return 0;
}
