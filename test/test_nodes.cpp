#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "nodes.hxx"

double b() { return 0.9; }

TEST(ReceiverTest, ScalingProbability) {
    ReceiverPreferences rp(b);
    Storehouse s1(1, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Storehouse s2(2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    rp.add_receiver(&s1);
    ASSERT_EQ(rp.get_preferences().at(&s1), 1.0);

    rp.add_receiver(&s2);
    ASSERT_EQ(rp.get_preferences().at(&s1), 0.5);
    ASSERT_EQ(rp.get_preferences().at(&s2), 0.5);

    ASSERT_EQ(rp.choose_receiver(), &s2);
}

TEST(RampTest, Timing) {
    Ramp r(1, 2);
    Storehouse s(2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    r.receiver_preferences_ = ReceiverPreferences(b);
    r.receiver_preferences_.add_receiver(&s);
    Time t = 1;
    r.deliver_goods(t);
    t++;
    r.deliver_goods(t);
    t++;

    ASSERT_EQ(r.get_sending_buffer().has_value(), true);
}

TEST(WorkerTest, HasBuffer) {
    // Test scenariusza opisanego na stronie:
    // http://home.agh.edu.pl/~mdig/dokuwiki/doku.php?id=teaching:programming:soft-dev:topics:net-simulation:part_nodes#bufor_aktualnie_przetwarzanego_polproduktu

    Worker w(1, 2, std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    Time t = 1;

    w.receive_package(Package(1));
    w.do_work(t);
    ++t;
    w.receive_package(Package(2));
    w.do_work(t);
    auto& buffer = w.get_sending_buffer();

    ASSERT_TRUE(buffer.has_value());
    EXPECT_EQ(buffer.value().get_id(), 1);
}

TEST(PackageTest, IsAssignedIdLowest) {
    // przydzielanie ID o jeden większych -- utworzenie dwóch obiektów pod rząd

    Package p1;
    Package p2;

    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
}

TEST(PackageTest, IsIdReused) {
    // przydzielanie ID po zwolnionym obiekcie

    {
        Package p1;
    }
    Package p2;

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsMoveConstructorCorrect) {
    Package p1;
    Package p2(std::move(p1));

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageTest, IsAssignmentOperatorCorrect) {
    Package p1;
    Package p2 = std::move(p1);

    EXPECT_EQ(p2.get_id(), 1);
}

TEST(PackageQueueTest, IsFifoCorrect) {
    PackageQueue q(PackageQueueType::FIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 1);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 2);
}

TEST(PackageQueueTest, IsLifoCorrect) {
    PackageQueue q(PackageQueueType::LIFO);
    q.push(Package(1));
    q.push(Package(2));

    Package p(std::move(q.pop()));
    EXPECT_EQ(p.get_id(), 2);

    p = q.pop();
    EXPECT_EQ(p.get_id(), 1);
}