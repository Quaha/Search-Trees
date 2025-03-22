#pragma once

#include "AVLTree.hpp"

template <typename TKey, typename TValue>
class TestableAVLTree : public AVLTree<TKey, TValue> {

    using typename AVLTree<TKey, TValue>::node_ptr;

    using AVLTree<TKey, TValue>::NULL_PTR;

protected:

    size_t getHeights(bool& is_correct_heights, node_ptr x) const {
        if (x == NULL_PTR) {
            return 0U;
        }

        size_t left_bh = getHeights(is_correct_heights, this->getLeftSon(x));
        size_t right_bh = getHeights(is_correct_heights, this->getRightSon(x));

        if (std::abs((int)left_bh - (int)right_bh) > 1) {
            is_correct_heights = false;
        }

        size_t curr_height = std::max(left_bh, right_bh);

        if (this->getHeight(x) != curr_height) {
            is_correct_heights = false;
        }

        return curr_height + 1U;
    }

    size_t getCountOfCorrectNode(node_ptr x) const {
        if (x == NULL_PTR) {
            return 0U;
        }

        size_t count = 0;

        count += getCountOfCorrectNode(this->getLeftSon(x));
        count += getCountOfCorrectNode(this->getRightSon(x));

        if (!this->isFictitious(x)) {
            ++count;
        }

        return count;
    }

    bool isSearchTree(node_ptr x, node_ptr prev_x) const {
        if (this->isFictitious(x)) {
            return true;
        }
        if (x != this->root && this->getParent(x) != prev_x) {
            return false;
        }
        if (!this->isFictitious(this->getLeftSon(x))) {
            if (this->getKey(x) <= this->getKey(this->getLeftSon(x))) {
                return false;
            }
            if (!isSearchTree(this->getLeftSon(x), x)) {
                return false;
            }
        }
        if (!this->isFictitious(this->getRightSon(x))) {
            if (this->getKey(x) >= this->getKey(this->getRightSon(x))) {
                return false;
            }
            if (!isSearchTree(this->getRightSon(x), x)) {
                return false;
            }
        }
        return true;
    }

public:

    bool isTreeCorrect() {

        bool is_correct_heights = true;
        getHeights(is_correct_heights, this->root);

        bool is_search_tree = isSearchTree(this->root, this->root);
        bool is_correct_size = (this->size() == getCountOfCorrectNode(this->root));

        return is_correct_heights && is_search_tree && is_correct_size;
    }
};