#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "package.hxx"
#include "storage_types.hxx"

using ::testing::Property;

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

TEST(Package, K) {
    {
        Package p1;
    }
    Package p2;
    ASSERT_EQ(p2.get_id(), 1);
}

TEST(PackageQueue_test, pop_FIFO) {
    Package p1;
    Package p2;
    Package p3;
    Package p4;
    PackageQueue p_q{PackageQueueType::FIFO};
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
    PackageQueue p_q{PackageQueueType::LIFO};
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

// Testy UPeL

TEST(UPeL_PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd

    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(UPeL_PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(UPeL_PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(UPeL_PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(UPeL_PackageQueueTest, IsFifoCorrect) {
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 1);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 2);
}

TEST(UPeL_PackageQueueTest, IsLifoCorrect) {
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 2);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 1);
}
