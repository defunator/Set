#pragma once
#include <initializer_list>
#include <iostream>
#include <list>

namespace NSet {
    template <typename ValueType>
    class Set {
    private:
        struct Node {
            typename std::list<ValueType>::iterator keys[4];
            Node* sons[4];
            size_t sons_number;
            Node* parent;

            explicit
            Node();

            Node(typename std::list<ValueType>::iterator key);

            Node(const Node& other);

            void sort_sons();

            void upd_key();

            void upd_keys();
            void remove_node();
        };
        Node* root_;
        std::list<ValueType> elements_;
        size_t size_;

        static Node* lower_bound(const ValueType& key, Node* root);
        void split_parent(Node* v);

        void dfs(Node* v);

    public:
        explicit
        Set();

        template <typename InputIterator>
        Set(InputIterator begin, InputIterator end);
        Set(std::initializer_list<ValueType> init_list);
        Set(const Set& other);

        void swap(Set& other);
        Set& operator= (const Set& other);

        size_t size() const;
        bool empty() const;

        void insert(const ValueType& x);
        void erase(const ValueType& x);

        typedef typename std::list<ValueType>::const_iterator iterator;

        iterator begin() const;
        iterator end() const;

        iterator find(const ValueType& key) const;

        iterator lower_bound(const ValueType& key) const;

        ~Set();
    };
}

template <typename ValueType>
NSet::Set<ValueType>::Node::Node()
    : sons_number(0)
    , parent(nullptr) {
}

template <typename ValueType>
NSet::Set<ValueType>::Node::Node(typename std::list<ValueType>::iterator key)
    : Node() {
    sons_number = 1;
    keys[0] = key;
}

template <typename ValueType>
NSet::Set<ValueType>::Node::Node(const Node& other)
    : Node() {
    sons_number = other.sons_number;
    parent = other.parent;
    for (size_t i = 0; i != sons_number; ++i) {
        keys[i] = other.keys[i];
        sons[i] = other.sons[i];
    }
}

template <typename ValueType>
void NSet::Set<ValueType>::Node::sort_sons() {
    for (size_t i = 0; i != sons_number; ++i) {
        for (size_t j = 1; j != sons_number; ++j) {
            if (sons[j - 1]->sons_number == 0 || (sons[j]->sons_number != 0 && *keys[j] < *keys[j - 1])) {
                std::swap(keys[j - 1], keys[j]);
                std::swap(sons[j - 1], sons[j]);
            }
        }
    }
}

template <typename ValueType>
void NSet::Set<ValueType>::Node::upd_key() {
    for (size_t i = 0; i != sons_number; ++i) {
        if (sons[i]->sons_number != 0) {
            size_t j = sons[i]->sons_number - 1;
            if (j > 0) {
                while (sons[i]->sons[j]->sons_number == 0) {
                    --j;
                }
            }
            keys[i] = sons[i]->keys[j];
        }
    }
}

template <typename ValueType>
void NSet::Set<ValueType>::Node::upd_keys() {
    Node* v = parent;
    while (v != nullptr) {
        v->upd_key();
        v = v->parent;
    }
}

template <typename ValueType>
void NSet::Set<ValueType>::split_parent(Node* v) {
    if (v->sons_number != 4) {
        return;
    }
    Node* split_v = new Node();
    split_v->sons[0] = v->sons[2];
    split_v->sons[1] = v->sons[3];
    split_v->keys[0] = v->keys[2];
    split_v->keys[1] = v->keys[3];
    split_v->parent = v->parent;
    v->sons[2]->parent = v->sons[3]->parent = split_v;
    v->sons_number = split_v->sons_number = 2;

    if (v->parent == nullptr) {
        Node* new_root = new Node();
        new_root->sons[0] = v;
        new_root->sons[1] = split_v;
        v->parent = split_v->parent = new_root;
        new_root->sons_number = 2;
        new_root->upd_key();
        new_root->sort_sons();
        root_ = new_root;
    } else {
        v->parent->sons[v->parent->sons_number] = split_v;
        ++v->parent->sons_number;
        v->parent->upd_key();
        v->parent->sort_sons();
        split_parent(v->parent);
    }
}

template <typename ValueType>
void NSet::Set<ValueType>::Node::remove_node() {
    for (size_t i = 0; i != parent->sons_number; ++i) {
        if (parent->sons[i] == this) {
            for (size_t j = i + 1; j != parent->sons_number; ++j) {
                std::swap(parent->sons[j - 1], parent->sons[j]);
                std::swap(parent->keys[j - 1], parent->keys[j]);
            }
            --parent->sons_number;
            upd_keys();
            delete parent->sons[parent->sons_number];
            break;
        }
    }
}

template <typename ValueType>
NSet::Set<ValueType>::Set()
    : root_(nullptr)
    , size_(0) {
}

template <typename ValueType>
template <typename InputIterator>
NSet::Set<ValueType>::Set(InputIterator begin, InputIterator end)
    : root_(nullptr)
    , size_(0) {
    while (begin != end) {
        insert(*(begin)++);
    }
}

template <typename ValueType>
NSet::Set<ValueType>::Set(std::initializer_list<ValueType> init_list)
    : Set(init_list.begin(), init_list.end()) {
}

template <typename ValueType>
void NSet::Set<ValueType>::dfs(Node* v) {
    if (v->sons_number == 2 || v->sons_number == 3) {
        for (size_t i = 0; i != v->sons_number; ++i) {
            dfs(v->sons[i]);
        }
    }
    delete v;
}

template <typename ValueType>
NSet::Set<ValueType>::Set(const Set& other)
    : Set(other.begin(), other.end()) {
}

template <typename ValueType>
void NSet::Set<ValueType>::swap(Set& other) {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(elements_, other.elements_);
}

template <typename ValueType>
NSet::Set<ValueType>& NSet::Set<ValueType>::operator= (const NSet::Set<ValueType>& other) {
    Set<ValueType> tmp(other);
    swap(tmp);
    return *this;
}

template <typename ValueType>
size_t NSet::Set<ValueType>::size() const {
    return size_;
}

template <typename ValueType>
bool NSet::Set<ValueType>::empty() const {
    return size_ == 0;
}

template <typename ValueType>
void NSet::Set<ValueType>::insert(const ValueType& key) {
    if (size_ == 0) {
        elements_.push_back(key);
        root_ = new Node(elements_.begin());
        ++size_;
        return;
    }

    Node* neighbour_key = lower_bound(key, root_);
    if (!(*neighbour_key->keys[0] < key) && !(key < *neighbour_key->keys[0])) {
        return;
    }

    ++size_;
    typename std::list<ValueType>::iterator key_it;

    if (key < *neighbour_key->keys[0]) {
        key_it = elements_.insert(neighbour_key->keys[0], key);
    } else {
        key_it = neighbour_key->keys[0];
        ++key_it;
        key_it = elements_.insert(key_it, key);
    }

    Node* new_node = new Node(key_it);

    if (neighbour_key->parent == nullptr) {
        Node* tmp = root_;
        root_ = new Node();
        root_->sons[0] = tmp;
        root_->sons[1] = new_node;
        root_->sons_number = 2;
        tmp->parent = new_node->parent = root_;
        root_->upd_key();
        root_->sort_sons();
    } else {
        Node* par = neighbour_key->parent;
        par->sons[par->sons_number] = new_node;
        ++par->sons_number;
        new_node->parent = par;
        par->upd_key();
        par->sort_sons();
        split_parent(par);
    }
    new_node->upd_keys();
}

template <typename ValueType>
void NSet::Set<ValueType>::erase(const ValueType& key) {
    if (size_ == 0) {
        return;
    }

    Node* key_node = lower_bound(key, root_);

    if (*key_node->keys[0] < key || key < *key_node->keys[0]) {
        return;
    }

    --size_;
    elements_.erase(key_node->keys[0]);
    --key_node->sons_number;

    while (true) {
        if (key_node->parent == nullptr) {
            delete root_;
            root_ = nullptr;
            break;
        } else if (key_node->parent->sons_number == 3) {
            key_node->remove_node();
            break;
        } else {
            Node* par = key_node->parent;

            Node* bro;
            if (par->sons[0] == key_node) {
                bro = par->sons[1];
            } else {
                bro = par->sons[0];
            }
            delete key_node;
            key_node = par->sons[0] = par->sons[1] = nullptr;
            par->sons_number = 0;

            Node* gpar = par->parent;
            if (gpar == nullptr) {
                delete par;
                par = nullptr;
                root_ = bro;
                root_->parent = nullptr;
                break;
            }

            Node* dpar;
            for (size_t i = 0; i != gpar->sons_number; ++i) {
                if (gpar->sons[i] == par) {
                    if (i == 0) {
                        dpar = gpar->sons[1];
                    } else {
                        dpar = gpar->sons[i - 1];
                    }
                    dpar->sons[dpar->sons_number] = bro;
                    ++dpar->sons_number;
                    bro->parent = dpar;
                    dpar->upd_key();
                    dpar->sort_sons();
                    split_parent(dpar);
                    break;
                }
            }

            key_node = par;
        }
    }
}

template <typename ValueType>
typename NSet::Set<ValueType>::iterator
NSet::Set<ValueType>::begin() const {
    return elements_.cbegin();
}

template <typename ValueType>
typename NSet::Set<ValueType>::iterator
NSet::Set<ValueType>::end() const {
    return elements_.cend();
}

template <typename ValueType>
typename NSet::Set<ValueType>::iterator
NSet::Set<ValueType>::find(const ValueType& key) const {
    typename NSet::Set<ValueType>::iterator it = lower_bound(key);
    if (it != end() && !(key < *it) && !(*it < key)) {
        return it;
    }
    return end();
}

template <typename ValueType>
typename NSet::Set<ValueType>::Node*
NSet::Set<ValueType>::lower_bound(const ValueType& key, Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    Node* t = root;
    while (t->sons_number != 1) {
        if (t->sons_number == 2) {
            if (*t->keys[0] < key) {
                t = t->sons[1];
            } else {
                t = t->sons[0];
            }
        } else {
            if (*t->keys[1] < key) {
                t = t->sons[2];
            } else if (*t->keys[0] < key) {
                t = t->sons[1];
            } else {
                t = t->sons[0];
            }
        }
    }
    return t;
}

template <typename ValueType>
typename NSet::Set<ValueType>::iterator
NSet::Set<ValueType>::lower_bound(const ValueType& key) const {
    Node* t = lower_bound(key, root_);
    if (t == nullptr) {
        return end();
    } else if (!(*(t->keys[0]) < key)) {
        return t->keys[0];
    }
    return end();
}

template <typename ValueType>
NSet::Set<ValueType>::~Set() {
    elements_.clear();
    size_ = 0;
    if (root_ != nullptr) {
        dfs(root_);
    }
    root_ = nullptr;
}

using namespace NSet;


