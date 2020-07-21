#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

template <typename T>
class Polynomial {
private:
    std::map<int64_t, T> pol;

public:
    Polynomial(const std::vector<T>& _pol) {
        pol[-1] = T(0);
        for (int64_t i = 0; i != int64_t(_pol.size()); ++i) {
            if (_pol[i] != T(0))
                pol[i] = _pol[i];
        }
    }
    Polynomial(T d = T(0)) {
        pol[-1] = T(0);
        if (d != T(0))
            pol[0] = d;
    }
    template <typename Iter>
    Polynomial(Iter first, Iter last) {
        pol[-1] = T(0);
        int64_t i = 0;
        while (first != last) {
            if (*first != T(0))
                pol[i] = *first;
            ++i;
            ++first;
        }
    }

    T operator[] (int64_t i) const {
        auto res = pol.find(i);
        if (res != pol.end())
            return res->second;
        return 0;
    }

    bool operator== (const Polynomial<T>&) const;
    bool operator== (T) const;
    bool operator!= (const Polynomial<T>&) const;
    bool operator!= (T) const;

    void delete_zeros();

    Polynomial<T>& operator+= (const Polynomial<T>&);
    Polynomial<T>& operator+= (T);

    Polynomial<T>& operator-= (const Polynomial<T>&);
    Polynomial<T>& operator-= (T);

    Polynomial<T> operator+ (const Polynomial<T>&) const;
    Polynomial<T> operator+ (T) const;
    template <typename U>
    friend Polynomial<U> operator+ (U, const Polynomial<U>&);

    Polynomial<T> operator- (const Polynomial<T>&) const;
    Polynomial<T> operator- (T) const;
    template <typename U>
    friend Polynomial<U> operator- (U, const Polynomial<U>&);

    Polynomial<T>& operator*= (const Polynomial<T>&);
    Polynomial<T>& operator*= (T);

    Polynomial<T> operator* (const Polynomial<T>&) const;
    Polynomial<T> operator* (T) const;
    template <typename U>
    friend Polynomial<U> operator* (U, const Polynomial<U>&);

    int64_t Degree() const;

    typename std::map<int64_t, T>::const_iterator begin() const;
    typename std::map<int64_t, T>::const_iterator end() const;

    T operator() (T) const;

    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const Polynomial<U>&);

    Polynomial<T> operator& (const Polynomial<T>&) const;

    Polynomial<T> operator/ (const Polynomial<T>&) const;

    Polynomial<T> operator% (const Polynomial<T>&) const;

    Polynomial<T> operator, (const Polynomial<T>&) const;
};

template <typename T>
void Polynomial<T>::delete_zeros() {
    std::vector<int> to_delete;
    for (const auto&[p, c] : pol) {
        if (p != -1 && c == 0)
            to_delete.push_back(p);
    }
    for (auto el : to_delete)
        pol.erase(el);
}

template <typename T>
bool Polynomial<T>::operator== (const Polynomial<T>& other) const {
    if (Degree() != other.Degree())
        return false;
    return pol == other.pol;
}

template <typename T>
bool Polynomial<T>::operator== (T d) const {
    Polynomial<T> other(d);
    if (Degree() != other.Degree())
        return false;
    return pol == other.pol;
}

template <typename T>
bool Polynomial<T>::operator!= (const Polynomial<T>& other) const {
    return !(*this == other);
}

template <typename T>
bool Polynomial<T>::operator!= (T other) const {
    return !(*this == other);
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator+= (const Polynomial<T>& other) {
    for (const auto& el : other.pol)
        pol[el.first] += el.second;
    delete_zeros();
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator+= (T d) {
    *this += Polynomial(d);
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator-= (const Polynomial<T>& other) {
    for (const auto& el : other.pol)
        pol[el.first] -= el.second;
    delete_zeros();
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator-= (T d) {
    *this -= Polynomial(d);
    return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator+ (const Polynomial<T>& other) const {
    Polynomial<T> res = *this;
    res += other;
    return res;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator+ (T d) const {
    Polynomial<T> res = *this;
    res += d;
    return res;
}

template <typename T>
Polynomial<T> operator+ (T d, const Polynomial<T>& other) {
    return other + d;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator- (const Polynomial<T>& other) const {
    Polynomial<T> res = *this;
    res -= other;
    return res;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator- (T d) const {
    Polynomial<T> res = *this;
    res -= Polynomial(d);
    return res;
}

template <typename T>
Polynomial<T> operator- (T d, const Polynomial<T>& other) {
    return Polynomial(d) - other;
}

template <typename T>
int64_t Polynomial<T>::Degree() const {
    int64_t deg = (*(*this).pol.rbegin()).first;
    return deg;
}

template <typename T>
typename std::map<int64_t, T>::const_iterator Polynomial<T>::begin() const {
    return ++pol.begin();
}

template <typename T>
typename std::map<int64_t, T>::const_iterator Polynomial<T>::end() const {
    return pol.end();
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator*= (const Polynomial<T>& other) {
    Polynomial<T> res;
    for (auto&[pow, coef] : *this) {
        for (auto&[pow1, coef1] : other)
            res.pol[pow + pow1] += coef * coef1;
    }
    res.delete_zeros();
    *this = res;
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator*= (T d) {
    *this *= Polynomial(d);
    return *this;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator* (const Polynomial<T>& other) const {
    Polynomial<T> res = *this;
    res *= other;
    return res;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator* (T d) const {
    Polynomial<T> res = *this;
    res *= d;
    return res;
}

template <typename T>
Polynomial<T> operator* (T d, const Polynomial<T>& other) {
    Polynomial<T> res = other;
    res *= d;
    return res;
}

template <typename T>
T Polynomial<T>::operator() (T x) const {
    T res = T(0);
    for (const auto[pow, coef] : *this) {
        T p = T(1);
        auto pow1 = pow;
        while (pow1--)
            p *= x;
        res += p * coef;
    }
    return res;
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const Polynomial<T>& other) {
    if (other.Degree() == -1) {
        out << 0;
        return out;
    }
    for (auto it = other.pol.rbegin(); it != other.pol.rend(); ++it) {
        if (it->first == -1)
            continue;
        int64_t i = it->first;
        if (i == 0) {
            if (i == other.Degree()) {
                out << other[i];
            } else {
                if (other[i] > T(0))
                    out << '+' << other[i];
                else
                    out << other[i];
            }
        } else {
            if (i != other.Degree() && other[i] > T(0))
                out << '+';
            if (other[i] == T(-1))
                out << '-';
            if (other[i] != T(-1) && other[i] != T(1))
                out << other[i] << '*';
            out << 'x';
            if (i != 1)
                out << '^' << i;
        }
    }
    return out;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator& (const Polynomial<T>& other) const {
    Polynomial<T> res;

    for (const auto&[pow, coef] : *this) {
        Polynomial<T> cur(T(1));
        auto poww = pow;
        while (poww--)
            cur *= other;
        for (const auto&[pow1, coef1] : cur)
            res.pol[pow1] += (*this)[pow] * cur.pol[pow1];
    }
    res.delete_zeros();
    return res;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator/ (const Polynomial<T>& other) const {
    if (Degree() == -1 || other.Degree() == -1 || Degree() < other.Degree())
        return Polynomial();
    Polynomial<T> dividend = *this;
    Polynomial<T> quotient;
    int i = 0;
    while (i != 10 && dividend.Degree() >= other.Degree()) {
        T coeff = dividend.pol[dividend.Degree()] / other[other.Degree()];
        quotient.pol[dividend.Degree() - other.Degree()] += coeff;
        Polynomial<T> factor;
        factor.pol[dividend.Degree() - other.Degree()] += coeff;
        dividend -= factor * other;
        ++i;
    }
    quotient.delete_zeros();
    return quotient;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator% (const Polynomial<T>& other) const {
    if (Degree() == -1 || other.Degree() == -1 || Degree() < other.Degree())
        return *this;
    Polynomial<T> dividend = *this;
    while (dividend.Degree() >= other.Degree()) {
        T coeff = dividend.pol[dividend.Degree()] / other[other.Degree()];
        Polynomial<T> factor;
        factor.pol[dividend.Degree() - other.Degree()] += coeff;
        dividend -= factor * other;
    }
    dividend.delete_zeros();
    return dividend;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator, (const Polynomial<T>& other) const {
    Polynomial<T> a = *this, b = other;
    while (b != T(0)) {
        Polynomial<T> tmp = a % b;
        a = b;
        b = tmp;
    }
    a = a / Polynomial(a.pol[a.Degree()]);
    return a;
}
