#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "package.hpp"

using ::testing::Property;

/*
 * Tests have to be run individually, because static lists in class Package
 * are carried onto next tests and cause incorrect results.
 */

TEST(Package_test, default_constructor_IDs) {
    Package p1;
    Package p2;
    Package p3;
    Package p4;
    ASSERT_EQ(p1.get_id(), 1);
    ASSERT_EQ(p2.get_id(), 2);
    ASSERT_EQ(p3.get_id(), 3);
    ASSERT_EQ(p4.get_id(), 4);
}

TEST(Package_test, default_constructor_IDs_from_freed) {
    Package p1;
    Package p2;
    Package p3;
    p2.~Package();
    Package p4;
    ASSERT_EQ(p1.get_id(), 1);
    ASSERT_EQ(p4.get_id(), 2);
    ASSERT_EQ(p3.get_id(), 3);
}

TEST(PackageQueue_test, pop_FIFO) {
    Package p1;
    Package p2;
    Package p3;
    Package p4;
    PackageQueue p_q{QueueType::Fifo};
    p_q.push(std::move(p1));
    p_q.push(std::move(p2));
    p_q.push(std::move(p3));
    p_q.push(std::move(p4));
    ASSERT_EQ(p_q.pop().get_id(), 1);
    ASSERT_EQ(p_q.pop().get_id(), 2);
    ASSERT_EQ(p_q.pop().get_id(), 3);
    ASSERT_EQ(p_q.pop().get_id(), 4);
    ASSERT_TRUE(p_q.empty());
}

TEST(PackageQueue_test, pop_LIFO) {
    Package p1;
    Package p2;
    Package p3;
    Package p4;
    PackageQueue p_q{QueueType::Lifo};
    p_q.push(std::move(p1));
    p_q.push(std::move(p2));
    p_q.push(std::move(p3));
    p_q.push(std::move(p4));
    ASSERT_EQ(p_q.pop().get_id(), 4);
    ASSERT_EQ(p_q.pop().get_id(), 3);
    ASSERT_EQ(p_q.pop().get_id(), 2);
    ASSERT_EQ(p_q.pop().get_id(), 1);
    ASSERT_TRUE(p_q.empty());
}
