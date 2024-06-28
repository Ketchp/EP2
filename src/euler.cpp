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


using cap_t = size_t;
using node_t = size_t;


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


auto solve() {
    size_t V, E;
    cin >> V >> E;

    vvi G(V);
    vector<vector<vector<size_t>>> Edge_id(V, vector<vector<size_t>>(V));
    vector<tuple<node_t, node_t, bool>> Edges; Edges.reserve(E);  // true for directed
    vi Deg(V);

    for(size_t e = 0; e < E; e++) {
        int a, b;
        char type;

        cin >> a >> b >> type;
        a--;
        b--;

        G[a].emplace_back(b);
        Deg[a]++; Deg[b]++;
        Edge_id[a][b].emplace_back(e);

        if(type == 'U') {
            G[b].emplace_back(a);
            Edge_id[b][a].emplace_back(e);
        }

        Edges.emplace_back(a, b, type == 'D');
    }

    size_t SINK = E + V;
    size_t SOURCE = SINK + 1;
    size_t CAP_SIZE = SOURCE + 1;

    vector<vector<cap_t>> capacity(CAP_SIZE, vector<cap_t>(CAP_SIZE, 0));
    // SOURCE --> edges  -->  vertices  -->  SINK

    // 0 .. E-1       : edges
    // E .. E + V - 1 : vertices
    // E + V          : SINK
    // E + V + 1      : SOURCE


    // add capacity from edge to vertex,
    // we want to pair edge to vertex it will go to
    for(size_t e = 0; e < E; e++) {
        auto [from, to, directed] = Edges[e];

        capacity[SOURCE][e]++;  // source -> edge; each edge can be oriented

        capacity[e][E + to]++;  // edge -> vertex pairing
        if(!directed)
            capacity[e][E + from]++;
    }

    for(size_t v = 0; v < V; v++) {
        auto deg = Deg[v];

        if(deg % 2 == 1)
            return false;

        capacity[E + v][SINK] = deg / 2;
    }

    vector<size_t> sources(1, SOURCE);
    vector<bool> sinks(CAP_SIZE, false); sinks[SINK] = true;
    vector<vector<cap_t>> flow(CAP_SIZE, vector<cap_t>(CAP_SIZE, 0));

    while(increase_flow(sources,
                        sinks,
                        capacity,
                        flow));

    cap_t max_flow = 0;
    for(size_t e = 0; e < E; e++)
        max_flow += flow[SOURCE][e];

    if(max_flow != E)
        return false;

    vector<node_t> loop;
    auto dfs_euler = [&](
            auto f,
            node_t curr,
            size_t left
        ) -> bool {

        loop.emplace_back(curr);
        if(left == 0)
            return true;

        for(size_t ngb = 0; ngb < V; ngb++) {
            for(auto e : Edge_id[curr][ngb]) {
                if(flow[e][E + ngb]) {
                    flow[e][E + ngb]--;
                    if(f(f, ngb, left - 1))
                        return true;
                    flow[e][E + ngb]++;
                }
            }
        }

        loop.pop_back();
        return false;
    };

    dfs_euler(dfs_euler, 0, E);

    auto last = loop.back();
    loop.pop_back();

    for(auto v: loop)
        cout << v + 1 << ' ';

    cout << last + 1 << '\n';

    return true;
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;

    for(int t = 1; t <= tc; t++) {
        if(!solve())
            cout << "No euler circuit exist\n";
        cout << '\n';
    }

    cout.flush();

    return 0;
}
