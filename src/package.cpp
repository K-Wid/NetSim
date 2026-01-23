#include "../include/package.hxx"

std::set<ElementID> Package::_assigned_IDs{};
std::set<ElementID> Package::_freed_IDs{};

ElementID Package::find_new_id() {
    if (_freed_IDs.empty()) {
        if (_assigned_IDs.empty()) {
            return 1;
        }
        return *_assigned_IDs.rbegin() + 1;
    }
    return *_freed_IDs.rbegin();
}

Package::Package(ElementID id) {
    bool id_in_assigned_IDs = _assigned_IDs.count(id) > 0;
    if (id_in_assigned_IDs) {
        id = Package::find_new_id();
    }
    _id = id;
    _assigned_IDs.insert(_id);
}

Package::Package() {
    _id = Package::find_new_id();
    _assigned_IDs.insert(_id);
    _freed_IDs.erase(_id);
}

Package::Package(Package &&package) {
    _id = package.get_id();
    _assigned_IDs.insert(_id);
    _freed_IDs.erase(_id);
}

Package& Package::operator=(Package&& package) {
    _id = package.get_id();
    _assigned_IDs.insert(_id);
    _freed_IDs.erase(_id);
    return *this;
}

Package::~Package() {
    _assigned_IDs.erase(_id);
    _freed_IDs.insert(_id);
}

Package PackageQueue::pop() {
    switch (_queue_type) {
        case QueueType::Fifo: {
            Package package = std::move(_queue.front());
            _queue.pop_front();
            return package;
        }
        case QueueType::Lifo: {
            Package package = std::move(_queue.back());
            _queue.pop_back();
            return package;
        }
        default: {
            return Package{0};
        }
    }
}

void PackageQueue::push(Package&& package) {
    _queue.push_back(std::move(package));
}

