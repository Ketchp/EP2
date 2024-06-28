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
typedef pair<double, double> pdd;
typedef vector<pdd> vpdd;

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


using cap_t = unsigned;
using node_t = int;

void apply_increase(
        std::vector<std::vector<cap_t>> &flow,
        const std::vector<std::pair<node_t, cap_t>> &P,
        node_t start
) {
    auto [from, cap] = P[start];

    for(node_t to = start;
        from != to;
        to = from, from = P[to].first) {
        // if reverse flow is bigger than increase
        if(flow[to][from] >= cap)
            flow[to][from] -= cap;
        else {
            flow[from][to] += cap - flow[to][from];  // flow increase minus reverse flow
            flow[to][from] = 0;  // we know that increase is bigger than reverse flow
        }
    }
}


bool increase_flow(
        const std::vector<node_t> &sources,
        const std::vector<bool> &is_sink,
        const std::vector<std::vector<cap_t>> &capacity,
        std::vector<std::vector<cap_t>> &flow
) {
    const auto N = static_cast<node_t>(capacity.size());
    constexpr auto NO_PRED = std::numeric_limits<node_t>::max();
    constexpr auto MAX_FLOW = std::numeric_limits<cap_t>::max();

    std::queue<node_t> q;
    std::vector<std::pair<node_t, cap_t>> P(N, {NO_PRED, 0});

    for(auto s: sources) {
        q.emplace(s);
        P[s] = {s, MAX_FLOW};
    }

    while(!q.empty()) {
        auto c = q.front();
        q.pop();

        for(node_t ngb = 0; ngb < N; ngb++, ngb += (ngb==c)) {
            if(P[ngb].first != NO_PRED)
                continue;

            // c -> ngb
            // we know that flow[c][ngb] == 0 or flow[ngb][c] == 0

            // flow already at maximum capacity
            if(flow[c][ngb] == capacity[c][ngb] && flow[ngb][c] == 0)
                continue;

            // flow to max capacity
            cap_t increase = capacity[c][ngb] - flow[c][ngb];

            // I am pussy and fear overflow
            if(increase + flow[ngb][c] >= increase)
                increase += flow[ngb][c];  // flow in reverse direction

            P[ngb] = {c, std::min(P[c].second, increase)};

            if(is_sink[ngb]) {
                apply_increase(flow, P, ngb);
                return true;
            }

            q.emplace(ngb);
        }
    }

    return false;
}


template<typename T>
auto ceil_n(T num, int digits) {
    return ceil(num * pow(10, digits)) / pow(10, digits);
}


auto solve() {
    int h, g, k;
    cin >> g >> h >> k;
    auto N = h + g + 2;  // one additional source and sink

    vpii G(g), H(h);
    cin >> G >> H;

    if(g > h + k)  // with enough time all gophers will find right hole
        return -1.;

    node_t SOURCE = g + h;
    node_t SINK = SOURCE + 1;

    // set gophers as sources
    vi sources(1, SOURCE);
    vector<bool> sinks(N, false);
    vector<vector<cap_t>> capacity(N, vector<cap_t>(N, 0));

    sinks[SINK] = true;  // one vertex where all holes link to
    for(auto gopher = 0; gopher < g; gopher++)
        capacity[SOURCE][gopher] = 1;
    for(auto hole = g; hole < g+h; hole++)
        capacity[hole][SINK] = 1;

    set<int> distances_set;

    for(auto [xa, ya]: G) {
        for(auto [xb, yb]: H) {
            auto [x, y] = pdd{xb - xa, yb - ya};
            distances_set.emplace(x*x + y*y);
        }
    }

    vector<int> distances(all(distances_set));
    std::sort(distances.begin(), distances.end());

    map<int, cap_t> cache;
    auto dist_to_live_gophers = [&](int dist) -> cap_t {
        if(cache.count(dist)) return cache[dist];

        vector<vector<cap_t>> flow(N, vector<cap_t>(N, 0));

        for(int gopher = 0; gopher < g; gopher++) {
            for(int hole = g; hole < g+h; hole++) {
                auto [gx, gy] = G[gopher];
                auto [hx, hy] = H[hole - g];
                int c_dist = (gx - hx) * (gx - hx) + (gy - hy) * (gy - hy);
                capacity[gopher][hole] = (c_dist <= dist);
            }
        }

        while(increase_flow(sources,
                            sinks,
                            capacity,
                            flow));

        cap_t res = 0;
        for(node_t gop = 0; gop < g; gop++)
            res += flow[SOURCE][gop];
        return cache[dist] = res;
    };

    size_t low_idx = 0,                      // none alive
    high_idx = distances.size() - 1;  // all alive

    while(low_idx < high_idx) {
        auto mid_idx = (low_idx + high_idx) / 2;
        auto dist = distances[mid_idx];

        auto alive = dist_to_live_gophers(dist);

        if(alive >= g - k)
            high_idx = mid_idx;
        else  // alive < g - k
            low_idx = mid_idx + 1;
    }

    return ceil_n(sqrt(distances[low_idx]), 3);
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;

    for(int t = 1; t <= tc; t++) {
        double res = solve();

        cout << "Case #" << t << ":\n";

        if(res < 0)
            cout << "Too bad.\n\n";
        else
            cout << fixed << setprecision(3) << res << "\n\n";
    }

    cout.flush();

    return 0;
}
