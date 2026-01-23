#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <set>
#include <list>

#include "types.hxx"

enum class QueueType {
    Fifo, Lifo
};

class Package {
private:
    static std::set<ElementID> _assigned_IDs;
    static std::set<ElementID> _freed_IDs;
    static ElementID find_new_id();
    ElementID _id;
public:
    Package(ElementID id);
    Package();
    Package(Package&& package);

    Package& operator=(Package&&);
    ElementID get_id() const { return _id; }

    ~Package();
};

class IPackageStockpile {
public:
    using iterator = std::list<Package>::iterator;
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&&) = 0;
    virtual bool empty() = 0;
    virtual std::size_t size() = 0;

    virtual iterator begin() = 0;
    virtual const_iterator cbegin() const = 0;
    virtual iterator end() = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue: public IPackageStockpile {
public:
    virtual Package pop() = 0;
    virtual QueueType get_queue_type() const = 0;
};

class PackageQueue: public IPackageQueue {
private:
    std::list<Package> _queue;
    QueueType _queue_type;
public:
    PackageQueue(QueueType queue_type) : _queue_type{queue_type} {}

    Package pop() override;
    QueueType get_queue_type() const override { return _queue_type; }
    void push(Package&&) override;
    bool empty() override { return _queue.empty(); }
    std::size_t size() override { return _queue.size(); }

    IPackageStockpile::iterator begin() override { return _queue.begin(); }
    IPackageStockpile::const_iterator cbegin() const override { return _queue.cbegin(); }
    IPackageStockpile::iterator end() override { return _queue.end(); }
    IPackageStockpile::const_iterator cend() const override { return _queue.cend(); }

    ~PackageQueue() override = default;
};

#endif //NETSIM_PACKAGE_HPP