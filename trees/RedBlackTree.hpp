#include <iostream>
#include <vector>

template <typename TKey, typename TValue>
class RedBlackTree {
protected:

    enum Color {
        Red,
        Black
    };

    struct Node {

        int parent;
        int left_node, right_node;

        Color color;

        std::pair<TKey, TValue> data;

        bool is_fict;
    };

public:

    class Iterator {
    protected:

        int tree_position = 0;

        RedBlackTree<TKey, TValue>* container;

        Iterator(int start_position, RedBlackTree<TKey, TValue>* container) :
            tree_position(start_position),
            container(container)
        {}

    public:

        std::pair<const TKey&, TValue&> operator*() {
            if (tree_position == -1) {
                throw std::out_of_range("It is forbidden to dereference .end() iterator.");
            }
            return { container->tree[tree_position].data.first, container->tree[tree_position].data.second };
        }

        const std::pair<const TKey&, const TValue&> operator*() const {
            if (tree_position == -1) {
                throw std::out_of_range("It is forbidden to dereference .end() iterator.");
            }
            return { container->tree[tree_position].data.first, container->tree[tree_position].data.second };
        }

        std::pair<TKey, TValue>* operator->() const {
            return &container->tree[tree_position].data;
        }

        Iterator operator++() {
            const TKey& key = container->tree[tree_position].data.first;
            auto result_it = container->upperBound(key);

            this->tree_position = result_it.tree_position;

            return result_it;
        }

        bool operator==(const Iterator& other) const {
            return this->tree_position == other.tree_position;
        }

        bool operator!=(const Iterator& other) const {
            return this->tree_position != other.tree_position;
        }

        friend class RedBlackTree;
    };

protected:

    std::vector<Node> tree;
    size_t sz = 0;

    std::vector<int> free_poses;

    int root;

    Iterator makeIterator(int position) const {
        return Iterator(position, const_cast<RedBlackTree<TKey, TValue>*>(this));
    }

    int createNode(int parent) {
        if (free_poses.empty()) {
            free_poses.push_back((int)tree.size());
            tree.push_back(Node());
        }
        int id = free_poses.back();
        free_poses.pop_back();

        tree[id].parent = parent;
        tree[id].left_node = -1;
        tree[id].right_node = -1;

        tree[id].color = Color::Black;
        tree[id].is_fict = true;

        return id;
    }

    void deleteNode(int x) {
        free_poses.push_back(x);
    }

    int findPosition(const TKey& key) const {
        int current_pos = root;
        while (!tree[current_pos].is_fict && tree[current_pos].data.first != key) {
            if (tree[current_pos].data.first > key) {
                current_pos = tree[current_pos].left_node;
            }
            else {
                current_pos = tree[current_pos].right_node;
            }
        }
        return current_pos;
    }

    void smallLeftRotation(int x) {
        int y = getRightSon(x);

        int subtree_root = getParent(x);

        tree[x].right_node = tree[y].left_node;
        if (getRightSon(x) != -1) {
            tree[getRightSon(x)].parent = x;
        }

        tree[x].parent = y;

        tree[y].parent = subtree_root;
        tree[y].left_node = x;

        changeParentsSon(subtree_root, x, y);
    }

    void smallRightRotation(int x) {
        int y = getLeftSon(x);

        int subtree_root = getParent(x);

        tree[x].left_node = tree[y].right_node;
        if (getLeftSon(x) != -1) {
            tree[getLeftSon(x)].parent = x;
        }

        tree[x].parent = y;

        tree[y].parent = subtree_root;
        tree[y].right_node = x;

        changeParentsSon(subtree_root, x, y);
    }

    void bigLeftRotation(int x) {
        smallRightRotation(getLeftSon(x));
        smallLeftRotation(x);
    }

    void bigRightRotation(int x) {
        smallLeftRotation(getRightSon(x));
        smallRightRotation(x);
    }

    int getLeftSon(int x) const {
        return tree[x].left_node;
    }

    int getRightSon(int x) const {
        return tree[x].right_node;
    }

    int getParent(int x) const {
        return tree[x].parent;
    }

    int getGrandParent(int x) const {
        return getParent(getParent(x));
    }

    int getUncle(int x) const {
        int g = getGrandParent(x);
        if (getLeftSon(g) == getParent(x)) {
            return getRightSon(g);
        }
        else {
            return getLeftSon(g);
        }
    }

    int getCousin(int x) const {
        if (tree[getParent(x)].left_node == x) {
            return tree[getParent(x)].right_node;
        }
        return tree[getParent(x)].left_node;
    }

    int getColor(int x) const {
        return tree[x].color;
    }

    void setColor(int x, Color color) {
        tree[x].color = color;
    }

    void fixTreeAfterInsert(int x) {
        if (getParent(x) == -1) { // => x is root
            tree[x].color = Color::Black;
            return;
        }

        int y = getParent(x);

        if (getColor(y) == Color::Red) { // x is Red

            int g = getGrandParent(x);
            int u = getUncle(x);

            if (getColor(u) == Color::Red) {
                setColor(y, Color::Black);
                setColor(u, Color::Black);
                setColor(g, Color::Red);

                fixTreeAfterInsert(g);
            }
            else { // u is Black

                if (getLeftSon(g) == y && getLeftSon(y) == x) {
                    setColor(y, Color::Black);
                    setColor(g, Color::Red);
                    smallRightRotation(g);
                }
                else if (getRightSon(g) == y && getRightSon(y) == x) {
                    setColor(y, Color::Black);
                    setColor(g, Color::Red);
                    smallLeftRotation(g);
                }
                else if (getLeftSon(g) == y && getRightSon(y) == x) {
                    smallLeftRotation(y);
                    fixTreeAfterInsert(y);
                }
                else if (getRightSon(g) == y && getLeftSon(y) == x) {
                    smallRightRotation(y);
                    fixTreeAfterInsert(y);
                }
            }
        }
    }

    void changeParentsSon(int p, int old_s, int new_s) {
        if (p == -1) {
            root = new_s;
        }
        else {
            if (getLeftSon(p) == old_s) {
                tree[p].left_node = new_s;
            }
            else {
                tree[p].right_node = new_s;
            }
        }
        tree[new_s].parent = p;
    }

    int getLowestPos(int x) const {
        int lowest_pos = x;
        while (!tree[getLeftSon(lowest_pos)].is_fict) {
            lowest_pos = getLeftSon(lowest_pos);
        }
        return lowest_pos;
    }

    void fixThisTrashAfterErase(int x) {
        if (x == root) {
            return;
        }

        int A = getParent(x);
        int B = getCousin(x);

        if (getColor(A) == Color::Red) {
            if (getLeftSon(B) != -1 && getColor(getLeftSon(B)) == Color::Red) {
                if (getLeftSon(A) == B) {
                    /*
                            A
                           /
                          B
                         /
                        C
                    */

                    //tree[A].color = Color::Black;

                    //smallRightRotation(B);

                    if (getColor(getRightSon(B)) == Color::Black) {
                        smallRightRotation(A);
                    }
                    else {
                        tree[B].color = Color::Red;
                        tree[getLeftSon(B)].color = Color::Black;
                        tree[getRightSon(B)].color = Color::Black;
                        smallRightRotation(A);
                        smallRightRotation(A);
                    }
                }
                else {
                    /*
                        A
                         \
                          B
                         /
                        C
                    */
                    tree[A].color = Color::Black;

                    smallRightRotation(B);
                    smallLeftRotation(A);
                }
            }
            else if (getRightSon(B) != -1 && getColor(getRightSon(B)) == Color::Red) {
                if (getLeftSon(A) == B) {
                    /*
                       A
                      /
                     B
                      \
                       C
                    */
                    tree[A].color = Color::Black;

                    smallLeftRotation(B);
                    smallRightRotation(A);
                }
                else {
                    /*
                       A
                        \
                         B
                          \
                           Ñ
                    */

                    //tree[A].color = Color::Black;

                    //smallLeftRotation(B);
                    //smallLeftRotation(A);

                    if (getColor(getLeftSon(B)) == Color::Black) {
                        smallLeftRotation(A);
                    }
                    else {
                        tree[B].color = Color::Red;
                        tree[getLeftSon(B)].color = Color::Black;
                        tree[getRightSon(B)].color = Color::Black;
                        smallLeftRotation(A);
                        smallLeftRotation(A);
                    }
                }
            }
            else {
                tree[A].color = Color::Black;
                tree[B].color = Color::Red;
            }
        }
        else if (getColor(A) == Color::Black) {
            if (getColor(B) == Color::Red) {
                if (getLeftSon(A) == B) {
                    int C = getRightSon(B);

                    /*
                       A
                      /
                     B
                      \
                       C
                    */

                    if (getLeftSon(C) != -1 && getColor(getLeftSon(C)) == Color::Red) {
                        int D = getLeftSon(C);

                        tree[D].color = Color::Black;

                        smallLeftRotation(B);
                        smallRightRotation(A);
                    }
                    else if (getRightSon(C) != -1 && getColor(getRightSon(C)) == Color::Red) {
                        int D = getRightSon(C);

                        smallLeftRotation(B);
                        smallRightRotation(A);
                        smallRightRotation(B);
                    }
                    else {
                        tree[C].color = Color::Red;
                        tree[B].color = Color::Black;

                        smallRightRotation(A);
                    }
                }
                else {
                    int C = getLeftSon(B);

                    /*
                        A
                         \
                          B
                         /
                        C
                    */

                    if (getRightSon(C) != -1 && getColor(getRightSon(C)) == Color::Red) {
                        int D = getRightSon(C);

                        tree[D].color = Color::Black;

                        smallRightRotation(B);
                        smallLeftRotation(A);
                    }
                    else if (getLeftSon(C) != -1 && getColor(getLeftSon(C)) == Color::Red) {
                        int D = getLeftSon(C);

                        smallRightRotation(B);
                        smallLeftRotation(A);
                        smallLeftRotation(B);
                    }
                    else {
                        tree[C].color = Color::Red;
                        tree[B].color = Color::Black;

                        smallLeftRotation(A);
                    }
                }
            }
            else if (getColor(B) == Color::Black){
                if (getLeftSon(B) != -1 && getColor(getLeftSon(B)) == Color::Red) {
                    int C = getLeftSon(B);
                    if (getLeftSon(A) == B) {
                        /*
                                A
                               /
                              B
                             /
                            C
                        */

                        tree[C].color = Color::Black;

                        //smallRightRotation(B);
                        smallRightRotation(A);
                    }
                    else {
                        /*
                            A
                             \
                              B
                             /
                            C
                        */
                        tree[C].color = Color::Black;

                        smallRightRotation(B);
                        smallLeftRotation(A);
                    }
                }
                else if (getRightSon(B) != -1 && getColor(getRightSon(B)) == Color::Red) {
                    int C = getRightSon(B);
                    if (getLeftSon(A) == B) {
                        /*
                           A
                          /
                         B
                          \
                           C
                        */
                        tree[C].color = Color::Black;

                        smallLeftRotation(B);
                        smallRightRotation(A);
                    }
                    else {
                        /*
                           A
                            \
                             B
                              \
                               Ñ
                        */

                        tree[C].color = Color::Black;

                        //smallLeftRotation(B);
                        smallLeftRotation(A);
                    }
                }
                else {
                    tree[B].color = Color::Red;
                    fixThisTrashAfterErase(A);
                }
            }
        }
    }

    void eraseNode(int x) {
        if (!tree[getLeftSon(x)].is_fict && !tree[getRightSon(x)].is_fict) {
            int min_right = getLowestPos(getRightSon(x));
            std::swap(tree[x].data, tree[min_right].data);
            eraseNode(min_right);
        }
        else if (tree[getLeftSon(x)].is_fict && !tree[getRightSon(x)].is_fict) {
            changeParentsSon(getParent(x), x, getRightSon(x));
            tree[getRightSon(x)].color = Color::Black;

            deleteNode(getLeftSon(x));
            deleteNode(x);
        }
        else if (!tree[getLeftSon(x)].is_fict && tree[getRightSon(x)].is_fict) {
            changeParentsSon(getParent(x), x, getLeftSon(x));
            tree[getLeftSon(x)].color = Color::Black;

            deleteNode(getRightSon(x));
            deleteNode(x);
        }
        else if (tree[getLeftSon(x)].is_fict && tree[getRightSon(x)].is_fict) {
            if (getColor(x) == Color::Red) {
                changeParentsSon(getParent(x), x, getLeftSon(x));

                deleteNode(getRightSon(x));
                deleteNode(x);
            }
            else {

                int subtree_root = getParent(x);
                int bad_subtree = getLeftSon(x);

                changeParentsSon(subtree_root, x, bad_subtree);

                deleteNode(getRightSon(x));
                deleteNode(x);

                fixThisTrashAfterErase(bad_subtree);
            }
        }
    }

    int getBlackHeight(bool& flag, int x) {
        if (x == -1) {
            return 0;
        }

        int left_bh = getBlackHeight(flag, getLeftSon(x));
        int right_bh = getBlackHeight(flag, getRightSon(x));

        if (left_bh != right_bh) {
            flag = false;
        }
        return left_bh + (tree[x].color == Color::Black);
    }

    bool isCorrectRedVertices(int x) {

        if (getLeftSon(x) != -1) {
            if (tree[x].color == Color::Red && tree[getLeftSon(x)].color == Color::Red) {
                return false;
            }
            if (!isCorrectRedVertices(getLeftSon(x))) {
                return false;
            }
        }

        if (getRightSon(x) != -1) {
            if (tree[x].color == Color::Red && tree[getRightSon(x)].color == Color::Red) {
                return false;
            }
            if (!isCorrectRedVertices(getRightSon(x))) {
                return false;
            }
        }
        return true;
    }

    bool isSearchTree(int x) {
        if (tree[x].is_fict) {
            return true;
        }
        if (!tree[getLeftSon(x)].is_fict) {
            if (tree[x].data.first <= tree[getLeftSon(x)].data.first) {
                return false;
            }
            if (!isSearchTree(getLeftSon(x))) {
                return false;
            }
        }
        if (!tree[getRightSon(x)].is_fict) {
            if (tree[x].data.first >= tree[getRightSon(x)].data.first) {
                return false;
            }
            if (!isSearchTree(getRightSon(x))) {
                return false;
            }
        }
        return true;
    }

    int getCountOfCorrectNode(int x) {
        if (x == -1) {
            return 0;
        }
        return getCountOfCorrectNode(getLeftSon(x)) + getCountOfCorrectNode(getRightSon(x)) + (!tree[x].is_fict);
    }

    void lowerBound(const TKey& key, int x, int &best_pos) {
        if (tree[x].is_fict) {
            return;
        }
        if (tree[x].data.first >= key) {
            best_pos = x;
            lowerBound(key, getLeftSon(x), best_pos);
        }
        else{
            lowerBound(key, getRightSon(x), best_pos);
        }
    }

    void upperBound(const TKey& key, int x, int &best_pos) {
        if (tree[x].is_fict) {
            return;
        }
        if (tree[x].data.first > key) {
            best_pos = x;
            upperBound(key, getLeftSon(x), best_pos);
        }
        else {
            upperBound(key, getRightSon(x), best_pos);
        }
    }

public:
    bool isCorrectTree() {
        bool is_correct_bh = true;
        getBlackHeight(is_correct_bh, root);

        bool is_correct_red_vertices = isCorrectRedVertices(root);

        bool is_search_tree = isSearchTree(root);

        bool is_correct_size = (size() == getCountOfCorrectNode(root));

        return is_correct_bh && is_correct_red_vertices && is_search_tree && is_correct_size;
    }

    RedBlackTree() {
        root = createNode(-1);
    }

    Iterator begin() const {
        int lowest_pos = getLowestPos(root);
        return makeIterator(lowest_pos);
    }

    Iterator end() const {
        return makeIterator(-1);
    }

    Iterator lowerBound(const TKey& key) {
        int best_pos = -1;
        lowerBound(key, root, best_pos);
        return makeIterator(best_pos);
    }

    Iterator upperBound(const TKey& key) {
        int best_pos = -1;
        upperBound(key, root, best_pos);
        return makeIterator(best_pos);
    }

    Iterator insert(const TKey& key, const TValue& value) {
        int pos = findPosition(key);
        if (tree[pos].is_fict) {
            ++sz;

            tree[pos].data.first = key;
            tree[pos].data.second = value;

            tree[pos].color = Color::Red;
            tree[pos].is_fict = false;

            tree[pos].left_node = createNode(pos);
            tree[pos].right_node = createNode(pos);

            fixTreeAfterInsert(pos);
        }
        return makeIterator(pos);
    }

    Iterator erase(const TKey& key) {
        return erase(find(key));
    }

    Iterator erase(Iterator it) {
        if (it == end()) {
            throw std::out_of_range("No such key in the tree");
        }
        --sz;
        auto result = it;
        ++result;
        eraseNode(it.tree_position);
        return result;
    }

    Iterator find(const TKey& key) const {
        int pos = findPosition(key);
        if (tree[pos].is_fict) {
            return end();
        }
        return makeIterator(pos);
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
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void clear() {
        tree.clear();
        this->sz = 0;

        free_poses.clear();

        root = createNode();
    }
};