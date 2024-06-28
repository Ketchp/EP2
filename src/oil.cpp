#include <bits/stdc++.h>

using namespace std;

//#define int ll
#define real long double

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) std::begin(x), std::end(x)
#define sz(x) (int)(x).size()
using ll = long long;
using pii = pair<int, int>;
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


using TR = uint16_t;
using RN = uint16_t;
RN ROOM_COUNT = 10;
TR ALL_ROOMS = (1 << ROOM_COUNT) - 1;


TR get_ngb(TR rooms, RN n) {
    rooms &= ~(1 << n);
    return ((rooms << 1) | (rooms >> 1)) & ALL_ROOMS;
}

template<typename T>
uint8_t set_bits(T x) {
    uint8_t cnt = x&1;
    while(x) cnt += (x >>= 1) & 1;
    return cnt;
}


auto main() -> signed {
    priority_queue<pair<uint8_t, TR>> q;
    vector<bool> visited(1 << ROOM_COUNT, false);
    map<TR, pair<TR, RN>> P;

    q.emplace(set_bits(ALL_ROOMS), ALL_ROOMS);

    while(!q.empty()) {
        auto [_, c] = q.top();
        q.pop();

        for(RN n = 0; n < ROOM_COUNT; n++) {
            auto ngb = get_ngb(c, n);

            if(visited[ngb]) continue;

            visited[ngb] = true;
            q.emplace(set_bits(ngb), ngb);
            P[ngb] = {c, n};
        }
    }

    cout << visited[0] << endl;

    auto c = 0;
    while(c != ALL_ROOMS) {
        auto [r, n] = P[c];

        cout << "(" << bitset<10>(r) << ", " << n << ")" << " -> " << bitset<10>(c) << endl;
        c = r;
    }

    return 0;
}
