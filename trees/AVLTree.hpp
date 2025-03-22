#pragma once

#include <utility>
#include <cmath>
#include <vector>

template <typename TKey, typename TValue>
class AVLTree {
protected:

    using node_ptr = int;
    const static node_ptr NULL_PTR = -1;

    struct Node {

        node_ptr parent;
        node_ptr left_node, right_node;

        size_t height;

        std::pair<TKey, TValue> data;

        bool is_fictitious;
    };

public:

    class Iterator {
    protected:

        node_ptr ptr = 0;

        AVLTree<TKey, TValue>* container_ptr;

        Iterator(node_ptr ptr, AVLTree<TKey, TValue>* container_ptr) :
            ptr(ptr),
            container_ptr(container_ptr)
        {}

    public:

        std::pair<const TKey&, TValue&> operator*() {
            if (ptr == NULL_PTR) {
                throw std::out_of_range("It is forbidden to dereference .end() iterator.");
            }
            return { container_ptr->tree[ptr].data.first, container_ptr->tree[ptr].data.second };
        }

        const std::pair<const TKey&, const TValue&> operator*() const {
            if (ptr == NULL_PTR) {
                throw std::out_of_range("It is forbidden to dereference .end() iterator.");
            }
            return { container_ptr->tree[ptr].data.first, container_ptr->tree[ptr].data.second };
        }

        std::pair<TKey, TValue>* operator->() const {
            return &container_ptr->tree[ptr].data;
        }

        Iterator& operator++() {
            node_ptr curr_node_ptr = ptr;
            node_ptr right_son_ptr = container_ptr->tree[curr_node_ptr].right_node;

            if (!container_ptr->tree[right_son_ptr].is_fictitious) {
                ptr = container_ptr->getLowestPos(right_son_ptr);
                return *this;
            }

            node_ptr prev_node_ptr = container_ptr->tree[curr_node_ptr].parent;
            while (prev_node_ptr != NULL_PTR && container_ptr->tree[prev_node_ptr].right_node == curr_node_ptr) {
                curr_node_ptr = prev_node_ptr;
                prev_node_ptr = container_ptr->tree[curr_node_ptr].parent;
            }
            ptr = prev_node_ptr;

            return *this;
        }

        bool operator==(const Iterator& other) const {
            return this->ptr == other.ptr;
        }

        bool operator!=(const Iterator& other) const {
            return this->ptr != other.ptr;
        }

        friend class AVLTree;
    };


protected:

    std::vector<Node> tree;
    size_t count_of_elements = 0;

    node_ptr root;

    std::vector<node_ptr> free_poses;

protected:

    Iterator makeIterator(node_ptr position) const {
        return Iterator(position, const_cast<AVLTree<TKey, TValue>*>(this));
    }

    node_ptr createNode(node_ptr parent) {
        if (free_poses.empty()) {
            free_poses.push_back(static_cast<node_ptr>(tree.size()));
            tree.push_back(Node());
        }
        node_ptr ptr = free_poses.back();
        free_poses.pop_back();

        tree[ptr].parent = parent;
        tree[ptr].left_node = NULL_PTR;
        tree[ptr].right_node = NULL_PTR;

        tree[ptr].height = 0U;

        tree[ptr].is_fictitious = true;

        return ptr;
    }

    void deleteNode(node_ptr ptr) {
        free_poses.push_back(ptr);
    }

protected:
    void smallLeftRotation(node_ptr x) {
        node_ptr y = getRightSon(x);

        node_ptr subtree_root = getParent(x);

        tree[x].right_node = tree[y].left_node;
        if (getRightSon(x) != NULL_PTR) {
            tree[getRightSon(x)].parent = x;
        }

        tree[x].parent = y;

        tree[y].parent = subtree_root;
        tree[y].left_node = x;

        updateHeight(x);
        updateHeight(y);

        changeParent(subtree_root, x, y);
    }

    void smallRightRotation(node_ptr x) {
        node_ptr y = getLeftSon(x);

        node_ptr subtree_root = getParent(x);

        tree[x].left_node = tree[y].right_node;
        if (getLeftSon(x) != NULL_PTR) {
            tree[getLeftSon(x)].parent = x;
        }

        tree[x].parent = y;

        tree[y].parent = subtree_root;
        tree[y].right_node = x;

        updateHeight(x);
        updateHeight(y);

        changeParent(subtree_root, x, y);
    }

    void bigLeftRotation(node_ptr x) {
        smallRightRotation(getRightSon(x));
        smallLeftRotation(x);
    }

    void bigRightRotation(node_ptr x) {
        smallLeftRotation(getLeftSon(x));
        smallRightRotation(x);
    }

protected:
    node_ptr getLeftSon(node_ptr x) const {
        return tree[x].left_node;
    }

    node_ptr getRightSon(node_ptr x) const {
        return tree[x].right_node;
    }

    node_ptr getParent(node_ptr x) const {
        return tree[x].parent;
    }

    const TKey& getKey(node_ptr x) const {
        return tree[x].data.first;
    }

    const TValue& getValue(node_ptr x) const {
        return tree[x].data.second;
    }

    bool isFictitious(node_ptr x) const {
        return tree[x].is_fictitious;
    }

    void changeParent(node_ptr parent, node_ptr old_son, node_ptr new_son) {
        if (parent == NULL_PTR) {
            root = new_son;
        }
        else {
            if (getLeftSon(parent) == old_son) {
                tree[parent].left_node = new_son;
            }
            else {
                tree[parent].right_node = new_son;
            }
        }
        tree[new_son].parent = parent;

        updateHeight(parent);
    }

protected:
    size_t getHeight(node_ptr x) const {
        return tree[x].height;
    }

    void setHeight(node_ptr x, size_t height) {
        tree[x].height = height;
    }

    void updateHeight(node_ptr x) {
        if (x != NULL_PTR) {
            setHeight(x, 0U);
            if (getLeftSon(x) != NULL_PTR) {
                setHeight(x, std::max(getHeight(x), getHeight(getLeftSon(x)) + 1U));
            }
            if (getRightSon(x) != NULL_PTR) {
                setHeight(x, std::max(getHeight(x), getHeight(getRightSon(x)) + 1U));
            }
        }
    }

protected:

    node_ptr getLowestPos(node_ptr x) const {
        node_ptr lowest_pos = x;
        while (!isFictitious(getLeftSon(lowest_pos))) {
            lowest_pos = getLeftSon(lowest_pos);
        }
        return lowest_pos;
    }

    node_ptr findPosition(const TKey& key) const {
        node_ptr current_ptr = root;
        while (!isFictitious(current_ptr) && getKey(current_ptr) != key) {
            if (getKey(current_ptr) > key) {
                current_ptr = tree[current_ptr].left_node;
            }
            else {
                current_ptr = tree[current_ptr].right_node;
            }
        }
        return current_ptr;
    }

    void insertPosition(node_ptr ptr, const TKey& key, const TValue& value) {
        tree[ptr].data.first = key;
        tree[ptr].data.second = value;

        tree[ptr].is_fictitious = false;

        tree[ptr].left_node = createNode(ptr);
        tree[ptr].right_node = createNode(ptr);

        updateHeight(ptr);

        fixTree(getParent(ptr));
    }

    void erasePosition(node_ptr x) {
        if (!isFictitious(getLeftSon(x)) && !isFictitious(getRightSon(x))) {
            node_ptr min_right = getLowestPos(getRightSon(x));
            std::swap(tree[x].data, tree[min_right].data);
            erasePosition(min_right);
        }
        else if (isFictitious(getLeftSon(x))) {
            node_ptr parent = getParent(x);
            changeParent(parent, x, getRightSon(x));

            deleteNode(getLeftSon(x));
            deleteNode(x);

            fixTree(parent);
        }
        else {
            node_ptr parent = getParent(x);
            changeParent(parent, x, getLeftSon(x));

            deleteNode(getRightSon(x));
            deleteNode(x);

            fixTree(parent);
        }
    }

    void fixTree(node_ptr x) {
        for (x; x != NULL_PTR; x = getParent(x)) {

            node_ptr y = getLeftSon(x);
            node_ptr z = getRightSon(x);

            if (getHeight(y) + 1 < getHeight(z)) {
                if (getHeight(getRightSon(z)) >= getHeight(getLeftSon(z))) {
                    smallLeftRotation(x);
                }
                else {
                    bigLeftRotation(x);
                }
                x = getParent(x);
            }
            else if (getHeight(y) > getHeight(z) + 1) {
                if (getHeight(getRightSon(y)) >= getHeight(getLeftSon(y))) {
                    bigRightRotation(x);
                }
                else {
                    smallRightRotation(x);
                }
                x = getParent(x);
            }
            else {
                updateHeight(x);
            }
        }
    }

    void lowerBound(const TKey& key, node_ptr x, node_ptr& nearest_pos) const {
        if (isFictitious(x)) {
            return;
        }
        if (getKey(x) >= key) {
            nearest_pos = x;
            lowerBound(key, getLeftSon(x), nearest_pos);
        }
        else {
            lowerBound(key, getRightSon(x), nearest_pos);
        }
    }

    void upperBound(const TKey& key, node_ptr x, node_ptr& nearest_pos) const {
        if (isFictitious(x)) {
            return;
        }
        if (getKey(x) > key) {
            nearest_pos = x;
            upperBound(key, getLeftSon(x), nearest_pos);
        }
        else {
            upperBound(key, getRightSon(x), nearest_pos);
        }
    }

public:

    AVLTree() {
        root = createNode(-1);
    }

    Iterator begin() const {
        if (empty()) {
            return end();
        }
        node_ptr lowest_pos = getLowestPos(root);
        return makeIterator(lowest_pos);
    }

    Iterator end() const {
        return makeIterator(NULL_PTR);
    }

    Iterator lowerBound(const TKey& key) {
        node_ptr nearest_pos = NULL_PTR;
        lowerBound(key, root, nearest_pos);
        return makeIterator(nearest_pos);
    }

    Iterator upperBound(const TKey& key) {
        node_ptr nearest_pos = NULL_PTR;
        upperBound(key, root, nearest_pos);
        return makeIterator(nearest_pos);
    }

    Iterator insert(const TKey& key, const TValue& value) {
        node_ptr ptr = findPosition(key);
        if (isFictitious(ptr)) {
            ++count_of_elements;
            insertPosition(ptr, key, value);
        }
        return makeIterator(ptr);
    }

    Iterator erase(const TKey& key) {
        return erase(find(key));
    }

    Iterator erase(Iterator it) {
        if (it == end()) {
            throw std::out_of_range("No such key in the tree");
        }
        --count_of_elements;
        auto result = it;
        erasePosition(it.ptr);
        return ++result;
    }

    Iterator find(const TKey& key) const {
        node_ptr ptr = findPosition(key);
        if (isFictitious(ptr)) {
            return end();
        }
        return makeIterator(ptr);
    }

    bool isExist(const TKey& key) const {
        return find(key) != end();
    }

    TValue& operator[](const TKey& key) {
        if (!isExist(key)) {
            throw std::runtime_error("No such key in table");
        }
        return (*find(key)).second;
    }

    const TValue& operator[](const TKey& key) const {
        if (!isExist(key)) {
            throw std::runtime_error("No such key in table");
        }
        return (*find(key)).second;
    }

    size_t size() const {
        return count_of_elements;
    }

    bool empty() const {
        return count_of_elements == 0U;
    }

    void clear() {
        tree.clear();
        this->count_of_elements = 0U;

        free_poses.clear();
        root = createNode(-1);
    }
};