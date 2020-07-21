#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>
#include <cmath>

template <typename T>
class Matrix;

template <typename T>
class MatrixIter {
    friend class Matrix<T>;

private:
    Matrix<T> * matr_ptr;
    const Matrix<T>* matr_const_ptr;
    size_t i, j;

public:
    MatrixIter(const Matrix<T>& data, size_t _i, size_t _j) : matr_ptr(nullptr)
                                                , matr_const_ptr(&data)
                                                , i(_i)
                                                , j(_j)
                                                {}
    MatrixIter(Matrix<T>& data, size_t _i, size_t _j) : matr_ptr(&data)
                                                , matr_const_ptr(&data)
                                                , i(_i)
                                                , j(_j)
                                                {}

    bool operator== (const MatrixIter<T>& other) const {
        return i == other.i && j == other.j
               && matr_const_ptr == other.matr_const_ptr;
    }
    bool operator!= (const MatrixIter<T>& other) const {
        return !(*this == other);
    }
    MatrixIter<T>& operator++ () {
        ++j;
        if (j == matr_const_ptr->cols) {
            j = 0;
            ++i;
        }
        return *this;
    }
    T& operator* () {
        if (matr_ptr == nullptr) {
            T& t = const_cast<T&>(matr_const_ptr->matr[i][j]);
            return t;
        }
        return matr_ptr->matr[i][j];
    }
    T operator* () const {
        return matr_const_ptr->matr[i][j];
    }
};

template <typename T>
class Matrix {
    friend class MatrixIter<T>;

private:
    size_t rows, cols;
    std::vector<std::vector<T>> matr;

public:
    Matrix(const std::vector<std::vector<T>>& data) :
            rows(data.size()), cols(data[0].size()), matr(data) {}
    Matrix(size_t _rows = 0, size_t _cols = 0) : rows(_rows), cols(_cols) {
        matr.resize(rows);
        for (auto& line : matr)
            line.resize(cols);
    }

    T& operator() (size_t i, size_t j = 0) {
        return this->matr[i][j];
    }
    T operator() (size_t i = 0, size_t j = 0) const {
        return this->matr[i][j];
    }
    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const Matrix<U>&);

    std::pair<size_t, size_t> size() const;

    Matrix<T>& operator+= (const Matrix<T>& other);
    Matrix<T> operator+ (const Matrix<T>&) const;

    Matrix<T>& operator*= (const T&);
    Matrix<T> operator* (const T&) const;
    template <typename U>
    friend Matrix<U> operator* (const U&, const Matrix<U>&);

    Matrix<T>& transpose();
    Matrix<T> transposed() const;

    Matrix<T>& operator*= (const Matrix<T>&);
    Matrix<T> operator* (const Matrix<T>&) const;

    MatrixIter<T> begin();
    MatrixIter<T> end();
    MatrixIter<T> begin() const;
    MatrixIter<T> end() const;

    template <typename U>
    std::vector<U> solve(const std::vector<U>&) const;
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i != m.rows; ++i) {
        for (size_t j = 0; j != m.cols; ++j) {
            if (j != 0)
                out << '\t';
            out << m.matr[i][j];
        }
        if (i != m.rows - 1)
            out << '\n';
    }
    return out;
}

template <typename T>
std::pair<size_t, size_t> Matrix<T>::size() const {
    return std::pair<size_t, size_t>{rows, cols};
}

template <typename T>
Matrix<T>& Matrix<T>::operator+= (const Matrix<T>& other) {
    for (size_t i = 0; i != rows; ++i)
        for (size_t j = 0; j != cols; ++j)
            matr[i][j] += other.matr[i][j];
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator+ (const Matrix<T>& other) const {
    Matrix<T> res = *this;
    res += other;
    return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*= (const T& d) {
    for (auto& lines : matr)
        for (auto& el : lines)
            el *= d;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator* (const T& d) const {
    Matrix<T> res = *this;
    res *= d;
    return res;
}

template <typename T>
Matrix<T> operator* (const T& d, const Matrix<T>& other) {
    return other * d;
}

template <typename T>
Matrix<T>& Matrix<T>::transpose() {
    Matrix<T> res(cols, rows);
    for (size_t i = 0; i != rows; ++i)
        for (size_t j = 0; j != cols; ++j)
            res.matr[j][i] = matr[i][j];
    *this = res;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::transposed() const {
    Matrix<T> res(cols, rows);
    for (size_t i = 0; i != rows; ++i)
        for (size_t j = 0; j != cols; ++j)
            res.matr[j][i] = matr[i][j];
    return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*= (const Matrix<T>& other) {
    Matrix<T> res(rows, other.cols);
    for (size_t i = 0; i != rows; ++i) {
        for (size_t j = 0; j != other.cols; ++j)
            for (size_t k = 0; k != cols; ++k)
                res.matr[i][j] += matr[i][k] * other.matr[k][j];
    }
    *this = res;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator* (const Matrix<T>& other) const {
    Matrix<T> res(rows, other.cols);
    for (size_t i = 0; i != rows; ++i) {
        for (size_t j = 0; j != other.cols; ++j)
            for (size_t k = 0; k != cols; ++k)
                res.matr[i][j] += matr[i][k] * other.matr[k][j];
    }
    return res;
}

template <typename T>
MatrixIter<T> Matrix<T>::begin() {
    return MatrixIter(*this, 0, 0);
}

template <typename T>
MatrixIter<T> Matrix<T>::end() {
    return MatrixIter(*this, rows, 0);
}

template <typename T>
MatrixIter<T> Matrix<T>::begin() const {
    return MatrixIter(*this, 0, 0);
}

template <typename T>
MatrixIter<T> Matrix<T>::end() const {
    return MatrixIter(*this, rows, 0);
}

template <typename T>
template <typename U>
std::vector<U> Matrix<T>::solve(const std::vector<U>& b) const {
    std::vector<std::vector<double>> sle(rows, std::vector<double>(rows));
    std::vector<double> max_in_line(sle.size());
    for (size_t i = 0; i != rows; ++i) {
        for (size_t j = 0; j != cols; ++j) {
            sle[i][j] = matr[i][j];
            max_in_line[i] = std::max(max_in_line[i], std::abs(sle[i][j]));
        }
        sle[i].push_back(b[i]);
        max_in_line[i] = std::max(max_in_line[i], std::abs(sle[i].back()));
    }
    for (size_t col = 0, row = 0; col != sle.size(); ++col, ++row) {
        size_t best_line = row;
        for (size_t i = row; i != sle.size(); ++i) {
            if (std::abs(sle[i][col]) * max_in_line[best_line]
                > std::abs(sle[best_line][col]) * max_in_line[i])
                best_line = i;
        }
        if (best_line != row) {
            std::swap(max_in_line[row], max_in_line[best_line]);
            std::swap(sle[row], sle[best_line]);
        }
        for (size_t i = row + 1; i != sle.size(); ++i) {
            if (sle[i][col] == 0)
                continue;
            max_in_line[i] = 0;
            double t = sle[i][col] / sle[row][col];
            for (size_t j = col; j != sle[i].size(); ++j) {
                sle[i][j] -= sle[row][j] * t;
                max_in_line[i] = std::max(max_in_line[i], std::abs(sle[i][j]));
            }
        }
    }
    std::vector<U> res(sle.size());
    for (int i = sle.size() - 1; i != -1; --i) {
        double x = sle[i].back();
        for (size_t j = i + 1; j != sle.size(); ++j)
            x -= sle[i][j] * res[j];
        x /= sle[i][i];
        res[i] = x;
    }
    return res;
}
