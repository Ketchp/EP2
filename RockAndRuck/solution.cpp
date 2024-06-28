#include <iostream>
#include <vector>
#include <array>
#include <limits>


using namespace std;


int main() {
    size_t n, m;
    cin >> n >> m;

    vector<vector<size_t>> G(n);
    vector<size_t> degree(n);

    // ((n - 1) + 1) / 2 for right rounding
    size_t degree_thr = n / 2;

    std::vector<bool> D_bool(n, false);  // O(n)

    // O(m)
    while(m--) {
        size_t u, v;
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);

        if(++degree[u] == degree_thr) D_bool[u] = true;
        if(++degree[v] == degree_thr) D_bool[v] = true;
    }

    // O(n)
    std::vector<size_t> D_list; D_list.reserve(n);
    std::vector<size_t> S_list; S_list.reserve(n);
    vector<size_t> G_to_H(n);

    for(size_t i = 0; i < n; i++) {
        array{&S_list, &D_list}[D_bool[i]]->emplace_back(i);
        G_to_H[i] = D_bool[i] ? D_list.size() : 0 ;
    }

    // D_size is O(m/n)
    const size_t D_size = D_list.size();
    const size_t S_size = S_list.size();

    // number of edges from/to set S
    // we will care only about deg_from_S for points in D,
    // but it is easier to compute it for all
    vector<size_t> deg_from_S(n);

    // O(n + m)... n for outer for loop, m for inner loop over at most all edges
    for(auto s: S_list)
        for(auto ngb: G[s])
            deg_from_S[ngb]++;

    // we can join all points from S_list to one point (+1)
    // S set is represented by 0 in H
    size_t H_size = D_size + 1;

    // this matrix takes at most O(m^2/n^2) <= O(m)
    vector<vector<bool>> H_matrix(H_size, vector<bool>(H_size, true));

    // O(m/n) * O(m/n) <= O(m)
    for(auto d1: D_list) {
        for(auto d2: G[d1]) {
            if(D_bool[d2]) {
                H_matrix[G_to_H[d1]][G_to_H[d2]] = false;
                H_matrix[G_to_H[d2]][G_to_H[d1]] = false;
            }
        }

        if(deg_from_S[d1] == S_size) {
            H_matrix[0][G_to_H[d1]] = false;
            // H_matrix[G_to_H[d1]][0] = false;  // not needed
        }
    }

    vector<vector<size_t>> H(H_size);

    // O(m^2/n^2) <= O(m)
    for(size_t i = 0; i < H_size; i++) {
        for(size_t j = i+1; j < H_size; j++) {
            if(H_matrix[i][j]) {
                H[i].emplace_back(j);
                H[j].emplace_back(i);
            }
        }
    }

    size_t curr_comp = 0;
    constexpr auto NO_COMP = numeric_limits<size_t>::max();

    vector<size_t> components(H_size, NO_COMP);

    for(size_t i = 0; i < H_size; i++) {
        if(components[i] != NO_COMP)
            continue;

        vector<size_t> q;
        q.push_back(i);

        while(!q.empty()) {
            auto c = q.back();
            q.pop_back();

            for(auto ngb: H[c]) {
                if(components[ngb] != NO_COMP)
                    continue;
                components[ngb] = curr_comp;
                q.push_back(ngb);
            }
        }
        curr_comp++;
    }

    size_t queries;
    cin >> queries;
    while(queries--) {
        size_t a0, b0, ak, bk;
        cin >> a0 >> b0 >> ak >> bk;

        // we might assume:
        // components[G_to_H[a0]] == components[G_to_H[b0]] &&
        // components[G_to_H[ak]] == components[G_to_H[bk]]

        cout << array{"NO\n", "YES\n"}[components[G_to_H[a0]] == components[G_to_H[ak]]];
    }
}
