#ifndef SET_SET_H
#define SET_SET_H

#include <vector>
#include <cstddef>

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

    Node* find(ValueType value, Node* tree) const {
        if (tree == nullptr) {
            return nullptr;
        }
        if (tree->val < value) {
            return find(value, tree->right);
        } else if (value < tree->val) {
            return find(value, tree->left);
        } else {
            return tree;
        }
    }

    Node* lower_bound(ValueType value, Node* tree) const {
        if (tree == nullptr) {
            return nullptr;
        }
        if (tree->val < value) {
            return lower_bound(value, tree->right);
        } else if (value < tree->val) {
            Node* node = lower_bound(value, tree->left);
            if (node == nullptr) {
                return tree;
            } else {
                return node;
            }
        } else {
            return tree;
        }
    }

    Node* find_next(ValueType value, Node* tree) const {
        if (tree == nullptr) {
            return nullptr;
        }
        if (tree->val < value) {
            return find_next(value, tree->right);
        } else if (value < tree->val) {
            Node* node = find_next(value, tree->left);
            if (node == nullptr) {
                return tree;
            } else {
                return node;
            }
        } else {
            return nullptr;
        }
    }

    Node* find_prev(ValueType value, Node* tree) const {
        if (tree == nullptr) {
            return nullptr;
        }
        if (tree->val < value) {
            Node* node = find_prev(value, tree->right);
            if (node == nullptr) {
                return tree;
            } else {
                return node;
            }

        } else if (value < tree->val) {
            return find_prev(value, tree->left);
        } else {
            return nullptr;
        }
    }

    Node* insert(const ValueType& value, Node* tree) {
        if (tree == nullptr) {
            tree = new Node(value);
            if (cnt == 0) {
                root = tree;
            }
            tree->next = find_next(value, root);
            tree->prev = find_prev(value, root);
            if (tree->prev) {
                tree->prev->next = tree;
            }
            if (tree->next) {
                tree->next->prev = tree;
            }
            ++cnt;
            to_delete.push_back(tree);
            return tree;
        } else if (tree->val < value) {
            tree->right = insert(value, tree->right);
        } else if (value < tree->val) {
            tree->left = insert(value, tree->left);
        }
        pull(tree);
        tree = balance(tree);
        return tree;
    }

    Node* erase(const ValueType& value, Node* tree) {
        if (tree == nullptr) {
            return tree;
        } else if (tree->val < value) {
            tree->right = erase(value, tree->right);
        } else if (value < tree->val) {
            tree->left = erase(value, tree->left);
        } else {
            if (tree->right) {
                Node* min = getMin(tree->right);
                tree->val = min->val;
                tree->right = erase(min->val, tree->right);
            } else if (tree->left) {
                Node* max = getMax(tree->left);
                tree->val = max->val;
                tree->left = erase(max->val, tree->left);
            } else {
                if (tree->prev) {
                    tree->prev->next = tree->next;
                }
                if (tree->next) {
                    tree->next->prev = tree->prev;
                }
                --cnt;
                return nullptr;
            }
        }
        pull(tree);
        tree = balance(tree);
        return tree;
    }

    int getHeight(Node* tree) const {
        if (tree == nullptr) {
            return -1;
        } else {
            return tree->height;
        }
    }

    Node* getMin(Node* tree) const {
        if (tree == nullptr) {
            return nullptr;
        }
        if (tree->left == nullptr) {
            return tree;
        } else {
            return getMin(tree->left);
        }
    }

    Node* getMax(Node* tree) const {
        if (tree == nullptr) {
            return nullptr;
        }
        if (tree->right == nullptr) {
            return tree;
        } else {
            return getMax(tree->right);
        }
    }

    int getDiff(Node* tree) const {
        if (tree == nullptr) {
            return 0;
        } else {
            return getHeight(tree->left) - getHeight(tree->right);
        }
    }

    void pull(Node* tree) const {
        if (tree == nullptr) {
            return;
        }
        tree->height = std::max(getHeight(tree->left), getHeight(tree->right)) + 1;
    }

    Node* balance(Node* tree) {
        if (getDiff(tree) < RIGHT_BOUND && getDiff(tree) > LEFT_BOUND) {
            return tree;
        } else if (getDiff(tree) == RIGHT_BOUND) {
            if (getDiff(tree->left) == -1) {
                tree = bigRotateRight(tree);
            } else {
                tree = rotateRight(tree);
            }
        } else {
            if (getDiff(tree->right) == 1) {
                tree = bigRotateLeft(tree);
            } else {
                tree = rotateLeft(tree);
            }
        }
        return tree;
    }

    Node* rotateLeft(Node* tree) {
        if (tree == nullptr) {
            return tree;
        }
        Node* r = tree->right;
        tree->right = r->left;
        r->left = tree;
        pull(tree);
        pull(r);
        return r;
    }

    Node* rotateRight(Node* tree) {
        if (tree == nullptr) {
            return tree;
        }
        Node* l = tree->left;
        tree->left = l->right;
        l->right = tree;
        pull(tree);
        pull(l);
        return l;
    }

    Node* bigRotateLeft(Node* tree) {
        if (tree == nullptr) {
            return tree;
        }
        tree->right = rotateRight(tree->right);
        return rotateLeft(tree);
    }

    Node* bigRotateRight(Node* tree) {
        if (tree == nullptr) {
            return tree;
        }
        tree->left = rotateLeft(tree->left);
        return rotateRight(tree);
    }

    void free(Node* tree) {
        if (tree == nullptr) {
            return;
        }
        free(tree->left);
        free(tree->right);
        delete tree;
    }

    class iterator {
    public:

        iterator() : iter(nullptr) {}

        iterator(Node* node, Node* lst) {
            iter = node;
            last = lst;
        }

        iterator(const iterator& it) {
            iter = it.iter;
            last = it.last;
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

    size_t size() const {
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
    const int RIGHT_BOUND = 2;
    const int LEFT_BOUND = -2;
    Node* root = nullptr;
    size_t cnt = 0;
    std::vector<Node*> to_delete;

};

#endif
