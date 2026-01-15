#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <set>
#include <list>

#include "types.hpp"

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


#endif //NETSIM_PACKAGE_HPP