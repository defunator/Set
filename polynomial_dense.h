#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

template <typename T>
class Polynomial {
private:
    std::vector<T> pol;

public:
    Polynomial(const std::vector<T>& _pol) : pol(_pol) {
        pol.push_back(T(0));
    }
    Polynomial(T d = T(0)) {
        pol.push_back(d);
    }
    template <typename Iter>
    Polynomial(Iter first, Iter last) : pol(std::vector<T>(first, last)) {
        pol.push_back(T(0));
    }

    size_t size() const {
        return pol.size();
    }

    T operator[] (size_t i) const {
        if (i >= pol.size())
            return T(0);
        return pol[i];
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

    int Degree();
    int Degree() const;

    typename std::vector<T>::const_iterator begin() const;
    typename std::vector<T>::const_iterator end() const;

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
    while (pol.size() != 1 && pol.back() == T(0))
        pol.pop_back();
}

template <typename T>
bool Polynomial<T>::operator== (const Polynomial<T>& other) const {
    if (Degree() != other.Degree())
        return false;
    for (int i = 0; i <= Degree(); ++i) {
        if (pol[i] != other.pol[i])
            return false;
    }
    return true;
}

template <typename T>
bool Polynomial<T>::operator== (T d) const {
    Polynomial<T> other(d);
    if (Degree() != other.Degree())
        return false;
    for (int i = 0; i <= Degree(); ++i) {
        if (pol[i] != other.pol[i])
            return false;
    }
    return true;
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
    if (size() < other.size())
        pol.resize(other.size());
    for (size_t i = 0; i != size() && i != other.size(); ++i)
        pol[i] += other[i];
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator+= (T d) {
    *this += Polynomial(d);
    return *this;
}

template <typename T>
Polynomial<T>& Polynomial<T>::operator-= (const Polynomial<T>& other) {
    if (size() < other.size())
        pol.resize(other.size());
    for (size_t i = 0; i != size() && i != other.size(); ++i)
        pol[i] -= other[i];
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
Polynomial<T>& Polynomial<T>::operator*= (const Polynomial<T>& other) {
    delete_zeros();
    Polynomial<T> res(std::vector<T>(size() + other.size(), T(0)));
    for (size_t i = 0; i != size(); ++i)
        for (size_t j = 0; j != other.size(); ++j)
            res.pol[i + j] += pol[i] * other.pol[j];
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
int Polynomial<T>::Degree() {
    delete_zeros();
    if (size() == 1 && pol[0] == T(0))
        return -1;
    return size() - 1;
}

template <typename T>
int Polynomial<T>::Degree() const {
    int deg = size() - 1;
    for (auto it = pol.rbegin(); it != pol.rend() && *it == T(0); ++it, --deg) {}
    return deg;
}


template <typename T>
typename std::vector<T>::const_iterator Polynomial<T>::begin() const {
    return pol.begin();
}

template <typename T>
typename std::vector<T>::const_iterator Polynomial<T>::end() const {
    return pol.begin() + Degree() + 1;
}

template <typename T>
T Polynomial<T>::operator() (T x) const {
    T res = pol[0], p = x;
    for (size_t i = 1; i != size(); ++i) {
        res += pol[i] * p;
        p *= x;
    }
    return res;
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const Polynomial<T>& other) {
    if (other.Degree() == -1) {
        out << 0;
        return out;
    }
    for (int i = other.Degree(); i >= 0; --i) {
        if (other.pol[i] == T(0))
            continue;
        if (i == 0) {
            if (i == other.Degree()) {
                out << other.pol[i];
            } else {
                if (other.pol[i] > T(0))
                    out << '+' << other.pol[i];
                else
                    out << other.pol[i];
            }
        } else {
            if (i != other.Degree() && other.pol[i] > T(0))
                out << '+';
            if (other.pol[i] == T(-1))
                out << '-';
            if (other.pol[i] != T(-1) && other.pol[i] != T(1))
                out << other.pol[i] << '*';
            out << 'x';
            if (i != 1)
                out << '^' << i;
        }
    }
    return out;
}


template <typename T>
Polynomial<T> Polynomial<T>::operator& (const Polynomial<T>& other) const {
    Polynomial<T> res(std::vector<T>(this->size() * other.size(), T(0)));
    Polynomial<T> cur(T(1));
    for (int i = 0; i != Degree() + 1; ++i, cur *= other) {
        for (int j = 0; j != cur.Degree() + 1; ++j)
            res.pol[j] += pol[i] * cur.pol[j];
    }
    return res;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator/ (const Polynomial<T>& other) const {
    if (Degree() == -1 || other.Degree() == -1 || Degree() < other.Degree())
        return Polynomial();
    Polynomial<T> dividend = *this;
    Polynomial<T> quotient(std::vector(Degree() - other.Degree() + 1, T(0)));
    while (dividend.Degree() >= other.Degree()) {
        T coeff = dividend.pol[dividend.Degree()] / other.pol[other.Degree()];
        quotient.pol[dividend.Degree() - other.Degree()] += coeff;
        Polynomial<T> factor(std::vector<T>(dividend.Degree() - other.Degree() + 1, T(0)));
        factor.pol[dividend.Degree() - other.Degree()] += coeff;
        dividend -= factor * other;
    }
    return quotient;
}

template <typename T>
Polynomial<T> Polynomial<T>::operator% (const Polynomial<T>& other) const {
    if (Degree() == -1 || other.Degree() == -1 || Degree() < other.Degree())
        return *this;
    Polynomial<T> dividend = *this;
    while (dividend.Degree() >= other.Degree()) {
        T coeff = dividend.pol[dividend.Degree()] / other.pol[other.Degree()];
        Polynomial<T> factor(std::vector<T>(dividend.Degree() - other.Degree() + 1, T(0)));
        factor.pol[dividend.Degree() - other.Degree()] += coeff;
        dividend -= factor * other;
    }
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
