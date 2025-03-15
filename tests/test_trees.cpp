#include <gtest/gtest.h>

#include <random>
#include <vector>
#include <set>

#include "AVLTree.hpp"
#include "RedBlackTree.hpp"

template <typename TreeType>
class SearchTreeTest : public ::testing::Test {
protected:
    TreeType tree;
};

using TreeImplementations = ::testing::Types<RedBlackTree<int, int>>;
TYPED_TEST_SUITE(SearchTreeTest, TreeImplementations);

TYPED_TEST(SearchTreeTest, IsEmptyContainerAfterInitializing) {
    EXPECT_TRUE(this->tree.empty());
}

TYPED_TEST(SearchTreeTest, CanInsertOneElement) {
    EXPECT_NO_THROW(this->tree.insert(0, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());
}

TYPED_TEST(SearchTreeTest, CanInsertSomeElements1) {
    EXPECT_NO_THROW(this->tree.insert(0, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(1, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(2, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());
}

TYPED_TEST(SearchTreeTest, CanInsertSomeElements2) {
    EXPECT_NO_THROW(this->tree.insert(2, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(1, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(0, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());
}

TYPED_TEST(SearchTreeTest, CanInsertSomeElements3) {
    EXPECT_NO_THROW(this->tree.insert(1, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(2, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(0, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());
}

TYPED_TEST(SearchTreeTest, CanInsertSomeElements4) {
    EXPECT_NO_THROW(this->tree.insert(1, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(0, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());

    EXPECT_NO_THROW(this->tree.insert(2, 0));
    EXPECT_TRUE(this->tree.isCorrectTree());
}

TYPED_TEST(SearchTreeTest, CanInsertALotOfElements) {

    const size_t cnt = 1000;

    std::vector<int> keys;
    std::set<int> was;

    while (keys.size() < cnt) {
        int key = std::rand();

        if (!was.contains(key)) {
            was.insert(key);
            keys.push_back(key);
        }
    }

    for (int key: keys) {
        EXPECT_NO_THROW(this->tree.insert(key, 0));
        EXPECT_TRUE(this->tree.isCorrectTree());
    }
}

TYPED_TEST(SearchTreeTest, CanFindOneElement) {
    this->tree.insert(0, 0);
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 0));
}

TYPED_TEST(SearchTreeTest, CanFindSomeElements1) {
    this->tree.insert(0, 0);
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 0));

    this->tree.insert(1, 0);
    EXPECT_EQ(*this->tree.find(1), std::make_pair(1, 0));

    this->tree.insert(2, 0);
    EXPECT_EQ(*this->tree.find(2), std::make_pair(2, 0));
}

TYPED_TEST(SearchTreeTest, CanFindSomeElements2) {
    this->tree.insert(2, 0);
    EXPECT_EQ(*this->tree.find(2), std::make_pair(2, 0));

    this->tree.insert(1, 0);
    EXPECT_EQ(*this->tree.find(1), std::make_pair(1, 0));

    this->tree.insert(0, 0);
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 0));
}

TYPED_TEST(SearchTreeTest, CanFindSomeElements3) {
    this->tree.insert(1, 0);
    EXPECT_EQ(*this->tree.find(1), std::make_pair(1, 0));

    this->tree.insert(2, 0);
    EXPECT_EQ(*this->tree.find(2), std::make_pair(2, 0));

    this->tree.insert(0, 0);
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 0));
}

TYPED_TEST(SearchTreeTest, CanFindSomeElements4) {
    this->tree.insert(1, 0);
    EXPECT_EQ(*this->tree.find(1), std::make_pair(1, 0));

    this->tree.insert(0, 0);
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 0));

    this->tree.insert(2, 0);
    EXPECT_EQ(*this->tree.find(2), std::make_pair(2, 0));
}

TYPED_TEST(SearchTreeTest, CanFindALotOfElements) {

    const size_t cnt = 1000;

    std::vector<int> keys;
    std::set<int> was;

    while (keys.size() < cnt) {
        int key = std::rand();

        if (!was.contains(key)) {
            was.insert(key);
            keys.push_back(key);
        }
    }

    for (int key : keys) {
        this->tree.insert(key, 0);
    }

    for (int key : keys) {
        EXPECT_EQ(*this->tree.find(key), std::make_pair(key, 0));
    }
}

TYPED_TEST(SearchTreeTest, CanChangeSomeElements1) {
    this->tree.insert(0, 0);
    (*this->tree.find(0)).second = 1;
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 1));

    this->tree.insert(1, 0);
    (*this->tree.find(1)).second = 1;
    EXPECT_EQ(*this->tree.find(1), std::make_pair(1, 1));

    this->tree.insert(2, 0);
    (*this->tree.find(2)).second = 1;
    EXPECT_EQ(*this->tree.find(2), std::make_pair(2, 1));
}

TYPED_TEST(SearchTreeTest, CanChangeSomeElements2) {
    this->tree.insert(2, 0);
    (*this->tree.find(2)).second = 1;
    EXPECT_EQ(*this->tree.find(2), std::make_pair(2, 1));

    this->tree.insert(1, 0);
    (*this->tree.find(1)).second = 1;
    EXPECT_EQ(*this->tree.find(1), std::make_pair(1, 1));

    this->tree.insert(0, 0);
    (*this->tree.find(0)).second = 1;
    EXPECT_EQ(*this->tree.find(0), std::make_pair(0, 1));
}

TYPED_TEST(SearchTreeTest, CanChangeALotOfElements) {

    const size_t cnt = 1000;

    std::vector<int> keys;
    std::set<int> was;

    while (keys.size() < cnt) {
        int key = std::rand();

        if (!was.contains(key)) {
            was.insert(key);
            keys.push_back(key);
        }
    }

    for (int key : keys) {
        this->tree.insert(key, 0);
    }

    for (int key : keys) {
        (*this->tree.find(key)).second = 1;
    }

    for (int key : keys) {
        EXPECT_EQ(*this->tree.find(key), std::make_pair(key, 1));
    }
}



