#include <bits/stdc++.h>

using namespace std;

//#define int ll
#define real long double

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
using ll = long long;
using pii = pair<int, int>;
using prr = pair<real, real>;
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

using Tt = vector<array<int, 4>>;

real hall(const vector<prr> &trees) {
    if(trees.size() < 2)
        return 0;

    real y_min = trees[0].second;
    prr start = trees[0];
    for(auto [x, y]: trees)
        if(y < y_min)
            y_min = y,
            start = {x, y};

    prr last_dir{-1, 0};
    real len = 0;
    prr curr = start;
    do {
        real dir_max = -2;
        auto dist_min = numeric_limits<real>::infinity();
        prr next;

        for(auto [nx, ny]: trees) {
            if(curr == prr{nx, ny})
                continue;

            // o luigi spaghetti
            auto [cx, cy] = curr;
            prr curr_dir{nx - cx, ny - cy};
            real curr_d = hypot(curr_dir.first, curr_dir.second);

            auto dir =
                    (last_dir.first * curr_dir.first +
                    last_dir.second * curr_dir.second) / curr_d;

            if(dir > dir_max || (dir == dir_max && dist_min >= curr_d)) {
                dir_max = dir;
                dist_min = curr_d;
                next = {nx, ny};
            }
        }

        len += dist_min;
        last_dir = {next.first - curr.first, next.second - curr.second};
        {
            auto d = hypot(last_dir.first, last_dir.second);
            last_dir = {last_dir.first / d, last_dir.second / d};
        }
        curr = next;
    } while(curr != start);

    return len;
}

auto solve(size_t N) {
    Tt trees(N);
    for(auto &[X, Y, V, L]: trees)
        cin >> X >> Y >> V >> L;

    auto res = numeric_limits<size_t>::max();
    for(size_t i = 1; i < (1 << N); i++) {
        vector<prr> subset;
        size_t lost_v = 0;
        real max_l = 0;
        for(size_t j = 0; j < N; j++)
            if((i >> j) & 1)
                subset.emplace_back(trees[j][0], trees[j][1]);
            else
                lost_v += trees[j][2],
                max_l += trees[j][3];

        if(hall(subset) <= max_l)
            res = min(res, lost_v);
    }

    return res;
}


auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    while(true) {
        size_t N;
        cin >> N;
        if(N == 0)
            break;

        cout << "The lost value is " << solve(N) << "." << endl;
    }

    return 0;
}
