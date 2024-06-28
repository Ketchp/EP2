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


template<typename T>
inline T safe_mod(T v, T m) {
    return (v % m + m) % m;
}

struct PogStr: public string {
    using string::string;

    string::value_type operator[](ssize_t idx) const noexcept {
        return string::operator[](safe_mod(idx, size()));
    }

    ssize_t size() const noexcept {
        return static_cast<ssize_t>(string::size());
    }
};

template<typename ...args>
struct vector_with_normal_indexing_like_in_python_and_even_better: public vector<args...> {
    typename vector<args...>::value_type operator[](ssize_t idx) const {
        return vector<args...>::operator[](safe_mod(idx, size()));
    }

    ssize_t size() const noexcept {
        return static_cast<ssize_t>(vector<args...>::size());
    }
};

struct ssize_tm {
    ssize_tm(ssize_t value, ssize_t mod)
        : value(value), mod(mod) {};
    ssize_t value;
    ssize_t mod;

    ssize_tm operator+(ssize_t other) const noexcept {
        return ssize_tm{safe_mod(value + other, mod), mod};
    }

    operator ssize_t() {
        return value;
    }
};


ssize_t solve() {
    PogStr s;
    cin >> s;
    auto N = static_cast<ssize_t>(s.size());


    using vwnilipaeb = vector_with_normal_indexing_like_in_python_and_even_better<ssize_tm>;
    vwnilipaeb starts;
    ssize_t b_len = 1;

    {
        char best_c = 'z';
        for(ssize_t idx = 0; idx < N; idx++) {
            char c = s[idx];
            if(best_c > c) {
                starts.clear();
                best_c = c;
            }
            else if(c > best_c)
                continue;
            starts.emplace_back(idx, N);
        }
    }

    // find the longest runs of the best character
    // collapse into the single character of original weight - 1
    // look at next character and find the best continuation
    // repeat
    while(true) {
        if(starts.size() == 1)
            return starts[0];

        ssize_t si = 0;
        while(starts[si-1] + b_len == starts[si] && si <= starts.size())
            si++;

        // this means that all starts can be chained up together
        // meaning best start is first start
        if(si > starts.size())
            return starts[0];

        vwnilipaeb new_starts;
        ssize_t new_len = b_len;


        for(ssize_t si0 = si; si - si0 < starts.size(); si++) {
            ssize_t sif = si;
            while(starts[si] + b_len == starts[si+1])  // we know this ends
                si++;

            if((si - sif + 1) * b_len > new_len) {
                new_starts.clear();
                new_len = (si - sif + 1) * b_len;
            }
            else if((si - sif + 1) * b_len < new_len)
                continue;

            new_starts.emplace_back(starts[sif]);
        }


        // now we know that all starts are followed by non-start
        starts.clear();
        b_len = new_len;
        {
            char best_follow = 'z';
            for(auto &start: new_starts) {
                char c = s[start + b_len];
                if(best_follow > c) {
                    best_follow = c;
                    starts.clear();
                }
                else if(best_follow < c)
                    continue;
                starts.emplace_back(start);
            }
        }

        b_len++;
    }
}




auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    int tc = 1;
    cin >> tc;

    for(int t = 1; t <= tc; t++) {
        cout << solve() + 1 << '\n';
        cout.flush();
    }


    return 0;
}
