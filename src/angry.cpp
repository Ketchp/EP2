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

using cap_t = unsigned;
using node_t = size_t;

void apply_increase(
        std::vector<std::vector<cap_t>> &flow,
        const std::vector<std::pair<node_t, cap_t>> &P,
        node_t start
) {
    auto [from, cap] = P[start];

    for(node_t to = start;
        from != to;
        to = from, from = P[to].first)
    {
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
    const cap_t N = capacity.size();
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



auto solve(int n, int m) {
    vector<node_t> sources(1, 1);
    vector<bool> sinks(2 * n, false);
    sinks[2*n-2] = true;

    // Machine 1(0):  0 -> 1
    // Machine 2(1):  2 -> 3
    // ..
    // Machine n(n-1):  2*n - 2 -> 2*n - 1

    vector<vector<cap_t>> capacity(2*n, vector<cap_t>(2*n, 0));

    for(size_t i = 2; i < n; i++) {
        node_t m_id;
        cap_t cap;
        cin >> m_id >> cap;
        m_id--;  // indexing from 0

        capacity[2 * m_id][2 * m_id + 1] = cap;
    }

    while(m--) {
        node_t j, k;
        cap_t d;
        cin >> j >> k >> d;
        j--; k--;

        capacity[2 * j + 1][2 * k] = d;
        capacity[2 * k + 1][2 * j] = d;
    }


    vector<vector<cap_t>> flow(2*n, vector<cap_t>(2*n, 0));
    while(increase_flow(sources,
                        sinks,
                        capacity,
                        flow));

    cap_t res = 0;
    for(node_t ngb = 0; ngb < 2 * n; ngb++)
        res += flow[1][ngb];

    return res;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

//    int tc = 1;
//    cin >> tc;

    for(int t = 1; true; t++) {
        int m, w;
        cin >> m >> w;
        if(m == 0)
            break;

        cout << solve(m, w) << '\n';
    }

    cout.flush();

    return 0;
}
