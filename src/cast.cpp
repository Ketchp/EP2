#include <bits/stdc++.h>

using namespace std;

//#define int ll

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


auto grid_ngb = {pii{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void visit_dot(const vector<vector<char>> &image,
               vector<vector<bool>> &visited,
               queue<pii> &q_out,
               int sh, int sw,
               int H, int W) {
    queue<pii> q;
    q.emplace(sh, sw);

    while(!q.empty()) {
        auto [ch, cw] = q.front();
        q.pop();

        for(auto [dh, dw]: grid_ngb) {
            auto nh = ch + dh,
                 nw = cw + dw;
            if(nh == -1 || nh == H || nw == -1 || nw == W) continue;
            if(visited[nh][nw]) continue;
            visited[nh][nw] = true;

            if(image[nh][nw] == '*')
                q_out.emplace(nh, nw);
            else if(image[nh][nw] == 'X')
                q.emplace(nh, nw);
        }
    }
}


auto solve() {
    int H, W;
    cin >> H >> W;

    if(H == 0 && W == 0)
        return vi{};

    vector<vector<char>> image(H, vector<char>(W));
    vector<vector<bool>> visited(H, vector<bool>(W));

    for(auto &row: image)
        cin >> row;

    vi res;

    for(int h = 0; h < H; h++)
        for(int w = 0; w < W; w++)
    {
        if(visited[h][w] || image[h][w] == '.') continue;
        int count = 0;

        queue<pii> q;
        visited[h][w] = true;
        if(image[h][w] == 'X') {
            count++;
            visit_dot(image, visited, q, h, w, H, W);
        }
        else
            q.emplace(h, w);

        while(!q.empty()) {
            auto [ch, cw] = q.front();
            q.pop();

            for(auto [dh, dw]: grid_ngb) {
                auto nh = ch + dh,
                     nw = cw + dw;
                if(nh == -1 || nh == H || nw == -1 || nw == W) continue;
                if(visited[nh][nw]) continue;

                visited[nh][nw] = true;
                if(image[nh][nw] == 'X') {
                    count++;
                    visit_dot(image, visited, q, nh, nw, H, W);
                }
                else if(image[nh][nw] == '*')
                    q.emplace(nh, nw);
            }
        }
        res.emplace_back(count);
    }

    std::sort(res.begin(), res.end());
    return res;
}

auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    while(true) {
        auto sol = solve();
        if(sol.empty())
            break;
        cout << "Throw: " << sol << '\n';
    }
    cout.flush();
    return 0;
}

