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

TYPED_TEST(SearchTreeTest, RangeBasedIterators) {

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
        this->tree.insert(key, key);
    }
    
    std::sort(keys.begin(), keys.end());

    int p = 0;
    for (auto [key, value] : this->tree) {
        EXPECT_EQ(key, keys[p]);
        EXPECT_EQ(value, keys[p++]);
    }
}

TYPED_TEST(SearchTreeTest, DereferencingEndPointer) {
    EXPECT_ANY_THROW(*this->tree.end());
}

TYPED_TEST(SearchTreeTest, LowerBound) {
    this->tree.insert(1, 10);
    this->tree.insert(3, 30);
    this->tree.insert(5, 50);

    auto it = this->tree.lowerBound(3);
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);

    it = this->tree.lowerBound(2);
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);

    it = this->tree.lowerBound(6);
    EXPECT_EQ(it, this->tree.end());
}

TYPED_TEST(SearchTreeTest, UpperBound) {
    this->tree.insert(1, 10);
    this->tree.insert(3, 30);
    this->tree.insert(5, 50);

    auto it = this->tree.upperBound(3);
    EXPECT_EQ(it->first, 5);
    EXPECT_EQ(it->second, 50);

    it = this->tree.upperBound(2);
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);

    it = this->tree.upperBound(5);
    EXPECT_EQ(it, this->tree.end());
}

TYPED_TEST(SearchTreeTest, LowerAndUpperBoundOnLargeTree) {
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
        this->tree.insert(key, key);
    }

    std::sort(keys.begin(), keys.end());

    for (size_t i = 0; i < cnt; ++i) {
        int key = keys[i];

        auto it_lower = this->tree.lowerBound(key);
        auto it_upper = this->tree.upperBound(key);

        if (i == cnt - 1) {
            EXPECT_EQ(it_lower->first, key);
            EXPECT_EQ(it_upper, this->tree.end());
        }
        else {
            EXPECT_EQ(it_lower->first, key);
            EXPECT_EQ(it_upper->first, keys[i + 1]);
        }
    }
}

TYPED_TEST(SearchTreeTest, LowerAndUpperBoundOnEmptyTree) {
    auto it = this->tree.lowerBound(10);
    EXPECT_EQ(it, this->tree.end());

    it = this->tree.upperBound(10);
    EXPECT_EQ(it, this->tree.end());
}






