#include <iostream>
#include <new>

template <typename T>
class Vector {
private:
    std::size_t sz;
    std::size_t cap;
    T* data;

public:
    Vector()
            : sz(0)
            , cap(0) {
    }
    Vector(size_t _sz)
            : sz(_sz)
            , cap(_sz)
            , data(reinterpret_cast<T*>(::operator new(_sz * sizeof(T)))) {
        T* end = data + sz, ptr = data;
        while (ptr != end)
            new(ptr++) T();
    }
    Vector(const Vector<T>& other)
            : sz(0)
            , cap(other.sz)
            , data(reinterpret_cast<T*>(::operator new(other.sz * sizeof(T)))) {
        for (const T& el : other)
            push_back(el);
    }

    void push_back(const T& val) {
        if (sz == cap) {
            size_t _cap = cap;
            cap = (cap == 0 ? 1 : 2 * cap);
            T* newdata = reinterpret_cast<T*>(::operator new(cap * sizeof(T)));
            new(newdata + sz) T(val);
            T* end = newdata + sz;
            T* ptr = newdata;
            T* ptr1 = data;
            while (ptr != end)
                new(ptr++) T(*(ptr1++));
            if (_cap != 0) {
                end = data + sz, ptr = data;
                while (ptr != end)
                    (ptr++)->~T();
                ::operator delete(data);
            }
            ++sz;
            data = newdata;
        } else {
            new(data + sz++) T(val);
        }
    }
    void push_back(T&& val) {
        if (sz == cap) {
            size_t _cap = cap;
            cap = (cap == 0 ? 1 : 2 * cap);
            T* newdata = reinterpret_cast<T*>(::operator new(cap * sizeof(T)));
            new(newdata + sz) T(std::move(val));
            T* end = newdata + sz;
            T* ptr = newdata;
            T* ptr1 = data;
            while (ptr != end)
                new(ptr++) T(*(ptr1++));
            if (_cap != 0) {
                end = data + sz, ptr = data;
                while (ptr != end)
                    (ptr++)->~T();
                ::operator delete(data);
            }
            ++sz;
            data = newdata;
        } else {
            new(data + sz++) T(std::move(val));
        }
    }
    void pop_back() {
        (data + --sz)->~T();
    }
    const T& operator[] (size_t i) const {
        return data[i];
    }
    T& operator[] (size_t i) {
        return data[i];
    }
    void resize(size_t _sz) {
        for (size_t i = _sz; i < sz; ++i)
            (data + i)->~T();
        if (cap < _sz) {
            T* newdata = reinterpret_cast<T*>(::operator new(_sz * sizeof(T)));
            T* end = newdata + std::min(sz, _sz);
            T* ptr = newdata;
            T* ptr1 = data;
            while (ptr != end)
                new(ptr++) T(*(ptr1++));
            for (std::size_t i = sz; i != _sz; ++i)
                new(newdata + i) T();
            if (cap != 0) {
                end = data + sz, ptr = data;
                while (ptr != end)
                    (ptr++)->~T();
                ::operator delete(data);
            }
            sz = cap = _sz;
            data = newdata;
        } else if (sz < _sz) {
            for (std::size_t i = sz; i != _sz; ++i)
                new(data + i) T();
            sz = _sz;
        }
        sz = _sz;
    }
    void reserve(size_t _cap) {
        if (cap < _cap) {
            T* newdata = reinterpret_cast<T*>(::operator new(_cap * sizeof(T)));
            T* end = newdata + sz;
            T* ptr = newdata;
            T* ptr1 = data;
            while (ptr != end)
                new(ptr++) T(*(ptr1++));
            if (cap != 0) {
                end = data + sz, ptr = data;
                while (ptr != end)
                    (ptr++)->~T();
                ::operator delete(data);
            }
            data = newdata;
            cap = _cap;
        }
        for (std::size_t i = _cap; i < sz; ++i)
            (data + i)->~T();
        if (sz > cap)
            sz = cap;
    }
    void swap(Vector<T>& other) {
        std::swap(sz, other.sz);
        std::swap(cap, other.cap);
        std::swap(data, other.data);
    }
    Vector<T>& operator= (const Vector<T>& other) {
        Vector<T> tmp(other);
        swap(tmp);
        return *this;
    }
    class Iterator {
    private:
        std::size_t i;
        T* ptr;

    public:
        Iterator(std::size_t _i, T* _ptr)
                : i(_i)
                , ptr(_ptr) {
        }
        const T& operator* () const {
            return ptr[i];
        }
        T& operator* () {
            return ptr[i];
        }
        Iterator& operator++ () {
            ++i;
            return *this;
        }
        Iterator operator-- () {
            --i;
            return *this;
        }
        bool operator!= (const Iterator& other) const {
            return !(ptr == other.ptr && i == other.i);
        }
    };
    Iterator begin() const {
        return Iterator(0, data);
    }
    Iterator end() const {
        return Iterator(sz, data);
    }
    size_t size() const {
        return sz;
    }
    size_t capacity() const {
        return cap;
    }
    void clear() {
        if (cap != 0) {
            T* end = data + sz;
            T* ptr = data;
            while (ptr != end)
                (ptr++)->~T();
            ::operator delete(data);
        }
        cap = 0;
        sz = 0;
    }
    ~Vector() {
        clear();
    }
};

