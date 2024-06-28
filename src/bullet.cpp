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

template<typename T = real>
struct Pos : public pair<T, T> {
    using pair<T, T>::pair;

    T &x() { return this->first; }
    T &y() { return this->second; }
    [[nodiscard]] const T &x() const { return this->first; }
    [[nodiscard]] const T &y() const { return this->second; }

    [[nodiscard]] Pos<T> projected_to(const Pos<T> &other) const {
        return other * ((other * *this) / (other * other));
    }

    [[nodiscard]] T norm() const {
        return hypot(x(), y());
    }

    [[nodiscard]] Pos<T> operator-(const Pos<T> &other) const {
        return Pos(this->x() - other.x(), this->y() - other.y());
    }

    [[nodiscard]] T operator*(const Pos<T> &other) const {
        return x() * other.x() + y() * other.y();
    }

    [[nodiscard]] Pos<T> operator*(T k) const {
        return {k * x(), k * y()};
    }

    Pos<T> &operator-=(const Pos<T> &other) {
        return *this = *this - other;
    }
};

template<typename T>
struct Circle;

template<typename T>
istream &operator>>(istream &is, Circle<T> &c) {
    return is >> static_cast<Pos<T> &>(c) >> c.r;
}

template<typename T = real>
struct Circle : public Pos<T> {
    Circle() = default;

    T r{};
    friend istream &operator>><>(istream &is, Circle<T> &c);
};


template<typename T>
istream &operator>>(istream &is, Pos<T> &p) {
    return is >> p.x() >> p.y();
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

pair<real, real> solve(int N) {
    vector<Circle<>> C(N);
    Pos s, e;

    cin >> C;
    cin >> s >> e;

    for(auto &c: C)
        c -= s;
    e -= s;

    real sees_thru = 0;
    real flies = e.norm();

    for(auto &c: C) {
        auto projection = c.projected_to(e);

        auto dir_test = projection * e;
        if(dir_test <= 0 ||    // wrong direction
           dir_test >= e * e)  // good direction but too far
            continue;

        auto rejection = c - projection;
        auto h = rejection.norm();
        if(h >= c.r)  // too far from line
            continue;

        auto chord_len = 2 * sqrt(c.r * c.r - h * h);
        auto circ_len = 2 * acos(h / c.r) * c.r;

        sees_thru += chord_len;
        flies += circ_len - chord_len;
    }


    return make_pair(sees_thru, flies);
}


auto main() -> signed {
    cin.tie(nullptr);
    istream::sync_with_stdio(false);

    cout << fixed << setprecision(2);

    while(true) {
        int N;
        cin >> N;
        if(N == -1)
            break;

        auto [E, F] = solve(N);

        cout << "Superman sees thru " << E << " units, and flies " << F << " units.\n";
    }

    cout.flush();

    return 0;
}
