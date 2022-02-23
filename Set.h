#ifndef SET_SET_H
#define SET_SET_H

#include <vector>

template<class ValueType> class Set {
public:
    struct Node {
        Node () {
            height = 0;
            left = nullptr;
            right = nullptr;
            next = nullptr;
            prev = nullptr;
        }

        Node (ValueType val_) {
            height = 0;
            val = val_;
            left = nullptr;
            right = nullptr;
            next = nullptr;
            prev = nullptr;
        }

        ValueType val;
        int height = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* next = nullptr;
        Node* prev = nullptr;
    };

    Node* find(ValueType value, Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->val < value) {
            return find(value, t->right);
        } else if (value < t->val) {
            return find(value, t->left);
        } else {
            return t;
        }
    }

    Node* lower_bound(ValueType value, Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->val < value) {
            return lower_bound(value, t->right);
        } else if (value < t->val) {
            Node* node = lower_bound(value, t->left);
            if (node == nullptr) {
                return t;
            } else {
                return node;
            }
        } else {
            return t;
        }
    }

    Node* find_next(ValueType value, Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->val < value) {
            return find_next(value, t->right);
        } else if (value < t->val) {
            Node* node = find_next(value, t->left);
            if (node == nullptr) {
                return t;
            } else {
                return node;
            }
        } else {
            return nullptr;
        }
    }

    Node* find_prev(ValueType value, Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->val < value) {
            Node* node = find_prev(value, t->right);
            if (node == nullptr) {
                return t;
            } else {
                return node;
            }

        } else if (value < t->val) {
            return find_prev(value, t->left);
        } else {
            return nullptr;
        }
    }

    Node* insert(const ValueType& value, Node* t) {
        if (t == nullptr) {
            t = new Node(value);
            if (cnt == 0) {
                root = t;
            }
            t->next = find_next(value, root);
            t->prev = find_prev(value, root);
            if (t->prev) {
                t->prev->next = t;
            }
            if (t->next) {
                t->next->prev = t;
            }
            ++cnt;
            to_delete.push_back(t);
            return t;
        } else if (t->val < value) {
            t->right = insert(value, t->right);
        } else if (value < t->val) {
            t->left = insert(value, t->left);
        }
        pull(t);
        t = balance(t);
        return t;
    }

    Node* erase(const ValueType& value, Node* t) {
        if (t == nullptr) {
            return t;
        } else if (t->val < value) {
            t->right = erase(value, t->right);
        } else if (value < t->val) {
            t->left = erase(value, t->left);
        } else {
            if (t->right) {
                Node* min = getMin(t->right);
                t->val = min->val;
                t->right = erase(min->val, t->right);
            } else if (t->left) {
                Node* max = getMax(t->left);
                t->val = max->val;
                t->left = erase(max->val, t->left);
            } else {
                if (t->prev) {
                    t->prev->next = t->next;
                }
                if (t->next) {
                    t->next->prev = t->prev;
                }
                --cnt;
                return nullptr;
            }
        }
        pull(t);
        t = balance(t);
        return t;
    }

    int getHeight(Node* t) const {
        if (t == nullptr) {
            return -1;
        } else {
            return t->height;
        }
    }

    Node* getMin(Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->left == nullptr) {
            return t;
        } else {
            return getMin(t->left);
        }
    }

    Node* getMax(Node* t) const {
        if (t == nullptr) {
            return nullptr;
        }
        if (t->right == nullptr) {
            return t;
        } else {
            return getMax(t->right);
        }
    }

    int getDiff(Node* t) const {
        if (t == nullptr) {
            return 0;
        } else {
            return getHeight(t->left) - getHeight(t->right);
        }
    }

    void pull(Node* t) const {
        if (t == nullptr) {
            return;
        }
        t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;
    }

    Node* balance(Node* t) {
        if (getDiff(t) < 2 && getDiff(t) > -2) {
            return t;
        } else if (getDiff(t) == 2) {
            if (getDiff(t->left) == -1) {
                t = bigRotateRight(t);
            } else {
                t = rotateRight(t);
            }
        } else {
            if (getDiff(t->right) == 1) {
                t = bigRotateLeft(t);
            } else {
                t = rotateLeft(t);
            }
        }
        return t;
    }

    Node* rotateLeft(Node* t) {
        if (t == nullptr) {
            return t;
        }
        Node* r = t->right;
        t->right = r->left;
        r->left = t;
        pull(t);
        pull(r);
        return r;
    }

    Node* rotateRight(Node* t) {
        if (t == nullptr) {
            return t;
        }
        Node* l = t->left;
        t->left = l->right;
        l->right = t;
        pull(t);
        pull(l);
        return l;
    }

    Node* bigRotateLeft(Node* t) {
        if (t == nullptr) {
            return t;
        }
        t->right = rotateRight(t->right);
        return rotateLeft(t);
    }

    Node* bigRotateRight(Node* t) {
        if (t == nullptr) {
            return t;
        }
        t->left = rotateLeft(t->left);
        return rotateRight(t);
    }

    void free(Node* t) {
        if (t == nullptr) {
            return;
        }
        free(t->left);
        free(t->right);
        delete t;
    }

    class iterator {
    public:

        iterator() : iter(nullptr) {}

        iterator(Node* node, Node* lst) {
            iter = node;
            last = lst;
        }

        iterator(const iterator& i) {
            iter = i.iter;
            last = i.last;
        }

        ValueType& operator *() const {
            return iter->val;
        }

        ValueType* operator ->() const {
            return (&iter->val);
        }

        iterator& operator ++() {
            if (iter) {
                iter = iter->next;
            }
            return *this;
        }

        iterator& operator --() {
            if (iter) {
                iter = iter->prev;
            } else {
                iter = last;
            }
            return *this;
        }

        iterator operator ++(int) {
            iterator tmp = (*this);
            if (iter) {
                iter = iter->next;
            }
            return tmp;
        }

        iterator operator --(int) {
            iterator tmp = (*this);
            if (iter) {
                iter = iter->prev;
            } else {
                iter = last;
            }
            return tmp;
        }

        bool operator ==(const iterator& other) const {
            return iter == other.iter;
        }

        bool operator !=(const iterator& other) const {
            return iter != other.iter;
        }

    private:
        Node* iter;
        Node* last;
    };

    Set () {
        cnt = 0;
        root = nullptr;
    }

    template<typename Iterator> Set(Iterator it1, Iterator it2) {
        cnt = 0;
        root = nullptr;
        for (auto it = it1; it != it2; ++it) {
            insert(*it);
        }
    }

    Set(std::initializer_list<ValueType> data) {
        cnt = 0;
        root = nullptr;
        for (auto el : data) {
            insert(el);
        }
    }

    Set(const Set& set) {
        if (this == &set) {
            return;
        }
        cnt = 0;
        root = nullptr;
        for (auto el : set) {
            insert(el);
        }
    }

    Set& operator =(const Set& set) {
        if (this == &set) {
            return (*this);
        }
        cnt = 0;
        root = nullptr;
        for (auto el : set) {
            insert(el);
        }
        return (*this);
    }

    void insert(const ValueType& value) {
        root = insert(value, root);
    }

    void erase(const ValueType& value) {
        root = erase(value, root);
    }

    iterator find(const ValueType& value) const {
        return iterator(find(value, root), getMax(root));
    }

    iterator lower_bound(const ValueType& value) const {
        return iterator(lower_bound(value, root), getMax(root));
    }

    iterator begin() const {
        return iterator(getMin(root), getMax(root));
    }

    iterator end() const {
        return iterator(nullptr, getMax(root));
    }

    int size() const {
        return cnt;
    }

    bool empty() const {
        return cnt == 0;
    }

    ~Set() {
        for (auto el : to_delete) {
            delete el;
        }
    };

private:
    Node* root = nullptr;
    int cnt = 0;
    std::vector<Node*> to_delete;

};

#endif