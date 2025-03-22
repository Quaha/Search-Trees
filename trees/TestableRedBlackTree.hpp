#pragma once

#include "RedBlackTree.hpp"

template <typename TKey, typename TValue>
class TestableRedBlackTree : public RedBlackTree<TKey, TValue> {

    using typename RedBlackTree<TKey, TValue>::node_ptr;
    using typename RedBlackTree<TKey, TValue>::Color;

    using RedBlackTree<TKey, TValue>::NULL_PTR;

protected:
    size_t getBlackHeight(bool& is_correct_bh, node_ptr x) const {
        if (x == NULL_PTR) {
            return 0U;
        }

        size_t left_bh = getBlackHeight(is_correct_bh, this->getLeftSon(x));
        size_t right_bh = getBlackHeight(is_correct_bh, this->getRightSon(x));

        if (left_bh != right_bh) {
            is_correct_bh = false;
        }

        size_t result = left_bh;
        if (this->getColor(x) == Color::Black) {
            ++result;
        }

        return result;
    }

    bool isCorrectRedVertices(node_ptr x) const {
        if (this->getLeftSon(x) != NULL_PTR) {
            if (this->getColor(x) == Color::Red && this->getColor(this->getLeftSon(x)) == Color::Red) {
                return false;
            }
            if (!isCorrectRedVertices(this->getLeftSon(x))) {
                return false;
            }
        }

        if (this->getRightSon(x) != NULL_PTR) {
            if (this->getColor(x) == Color::Red && this->getColor(this->getRightSon(x)) == Color::Red) {
                return false;
            }
            if (!isCorrectRedVertices(this->getRightSon(x))) {
                return false;
            }
        }

        return true;
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
        bool is_correct_bh = true;
        getBlackHeight(is_correct_bh, this->root);

        bool is_correct_red_vertices = isCorrectRedVertices(this->root);
        bool is_search_tree = isSearchTree(this->root, this->root);
        bool is_correct_size = (this->size() == getCountOfCorrectNode(this->root));

        return is_correct_bh && is_correct_red_vertices && is_search_tree && is_correct_size;
    }
};