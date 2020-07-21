#pragma once
#include <algorithm>
#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>
#include <vector>

namespace HashMap {
// let's make min amount of buckets 32
    constexpr const size_t MinBuckAmount = 32;

    template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
    class HashMap {
    private:
        size_t map_size;
        Hash hasher;
        std::vector<std::list<std::pair<KeyType, ValueType>>> map;

        // rebuilds current map so that 1 / 8 <= elements / buckets <= 1 / 2
        // so there is O(n) buckets in every time
        void rebuild(size_t);

    public:
        explicit
        HashMap(const Hash &_hasher = Hash())
                : map_size(0), hasher(_hasher), map(MinBuckAmount) {
        }

        template<typename InputIterator>
        HashMap(InputIterator begin,
                InputIterator end,
                const Hash &_hasher = Hash())
                : HashMap(_hasher) {
            while (begin != end)
                insert(*(begin++));
        }

        HashMap(std::initializer_list<std::pair<KeyType, ValueType>> init_list,
                const Hash &_hasher = Hash())
                : HashMap(init_list.begin(), init_list.end(), _hasher) {}

        size_t size() const {
            return map_size;
        }

        bool empty() const {
            return map_size == 0;
        }

        Hash hash_function() const {
            return hasher;
        }

        void insert(std::pair<KeyType, ValueType> &&);

        void insert(const std::pair<KeyType, ValueType> &);

        void erase(const KeyType &);

        ValueType &operator[](const KeyType &);

        void clear();

    private:
        template <bool is_const>
        class base_iterator {
            private:
                typedef typename std::conditional<is_const, const HashMap*, HashMap*>::type HashMapPtr;
                typedef typename std::conditional<is_const,
                        const std::pair<const KeyType, ValueType>, std::pair<const KeyType, ValueType>>::type PairType;
                typedef typename std::conditional<is_const,
                        typename std::list<std::pair<KeyType, ValueType>>::const_iterator,
                        typename std::list<std::pair<KeyType, ValueType>>::iterator>::type IterType;
                HashMapPtr ptr;
                std::size_t veciter;
                IterType it;

                base_iterator& find_next() {
                    while (it == ptr->map[veciter].end()) {
                        if (veciter + 1 == ptr->map.size())
                            return *this;
                        it = ptr->map[++veciter].begin();
                    }
                    return *this;
                }
            public:
                base_iterator()
                        : ptr(nullptr), veciter(0) {}

                base_iterator(HashMapPtr _ptr, std::size_t _veciter, IterType _it)
                : ptr(_ptr), veciter(_veciter), it(_it) {
                    find_next();
                }

                bool operator==(const base_iterator &other) const {
                    return ptr == other.ptr
                           && veciter == other.veciter
                           && it == other.it;
                }

                bool operator!=(const base_iterator &other) const {
                    return !(*this == other);
                }

                base_iterator& operator++() {
                    if (it == ptr->map[veciter].end() && veciter + 1 == ptr->map.size())
                        return *this;
                    ++it;
                    return find_next();
                }

                base_iterator operator++(int) {
                    base_iterator tmp(*this);
                    ++(*this);
                    return tmp;
                }

                PairType& operator*() const {
                    auto& ret = reinterpret_cast<PairType&>(*it);
                    return ret;
                }

                PairType*
                operator->() const {
                    auto* ret = reinterpret_cast<PairType*>(&*it);
                    return ret;
                }
        };

    public:
        typedef base_iterator<false> iterator;
        typedef base_iterator<true> const_iterator;

        iterator begin() {
            return iterator(this, 0, map[0].begin());
        }

        const_iterator begin() const {
            return const_iterator(this, 0, map[0].cbegin());
        }

        iterator end() {
            return iterator(this, map.size() - 1, map[map.size() - 1].end());
        }

        const_iterator end() const {
            return const_iterator(this, map.size() - 1, map[map.size() - 1].cend());
        }

        iterator find(const KeyType &);

        const_iterator find(const KeyType &) const;

        const ValueType &at(const KeyType &) const;
    };

    template<class KeyType, class ValueType, class Hash>
    void HashMap<KeyType, ValueType, Hash>::rebuild(size_t newmap_size) {
        std::vector<std::list<std::pair<KeyType, ValueType>>> newmap(newmap_size);
        for (auto &element : *this) {
            newmap[hasher(element.first) % newmap.size()].push_back(element);
        }
        std::swap(map, newmap);
    }

    template<class KeyType, class ValueType, class Hash>
    void HashMap<KeyType, ValueType, Hash>::insert(
            std::pair<KeyType, ValueType> &&element) {
        auto hash_value = hasher(element.first) % map.size();
        for (const auto &el : map[hash_value]) {
            auto key = el.first;
            if (key == element.first)
                return;
        }
        map[hash_value].push_back(element);
        ++map_size;
        if (2 * map_size >= map.size())
            rebuild(2 * map.size());
    }

    template<class KeyType, class ValueType, class Hash>
    void HashMap<KeyType, ValueType, Hash>::insert(
            const std::pair<KeyType, ValueType> &element) {
        insert(std::pair<KeyType, ValueType>(element));
    }

    template<class KeyType, class ValueType, class Hash>
    void HashMap<KeyType, ValueType, Hash>::erase(const KeyType &key) {
        auto hash_value = hasher(key) % map.size();
        for (auto it = map[hash_value].begin(); it != map[hash_value].end(); ++it) {
            if (it->first == key) {
                map[hash_value].erase(it);
                --map_size;
                break;
            }
        }
        if (8 * map_size <= map.size())
            // but map.size() needs to be at least MinBuckAmount
            rebuild(std::max(MinBuckAmount, map.size() / 2));
    }

    template<class KeyType, class ValueType, class Hash>
    ValueType &HashMap<KeyType, ValueType, Hash>::operator[](
            const KeyType &_key) {
        for (auto &element : map[hasher(_key) % map.size()]) {
            auto key = element.first;
            auto &val = element.second;
            if (key == _key)
                return val;
        }
        insert({_key, ValueType{}});
        return map[hasher(_key) % map.size()].rbegin()->second;
    }

    template<class KeyType, class ValueType, class Hash>
    void HashMap<KeyType, ValueType, Hash>::clear() {
        for (auto &chain : map)
            chain.clear();
        map.resize(MinBuckAmount);
        map_size = 0;
    }

    template<class KeyType, class ValueType, class Hash>
    typename HashMap<KeyType, ValueType, Hash>::iterator
    HashMap<KeyType, ValueType, Hash>::find(const KeyType &key) {
        auto hash_value = hasher(key) % map.size();
        for (auto it = map[hash_value].begin(); it != map[hash_value].end(); ++it) {
            if (it->first == key)
                return HashMap<KeyType, ValueType, Hash>::iterator(
                        this, hash_value, it);
        }
        return end();
    }

    template<class KeyType, class ValueType, class Hash>
    typename HashMap<KeyType, ValueType, Hash>::const_iterator
    HashMap<KeyType, ValueType, Hash>::find(const KeyType &key) const {
        auto hash_value = hasher(key) % map.size();
        for (auto it = map[hash_value].begin(); it != map[hash_value].end(); ++it) {
            if (it->first == key)
                return HashMap<KeyType, ValueType, Hash>::const_iterator(
                        this, hash_value, it);
        }
        return end();
    }

    template<class KeyType, class ValueType, class Hash>
    const ValueType &
    HashMap<KeyType, ValueType, Hash>::at(const KeyType &key) const {
        const_iterator it = find(key);
        if (it == end())
            throw std::out_of_range("");
        return it->second;
    }
}


// using namespace HashMap;
