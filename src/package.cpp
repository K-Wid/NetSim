#include "../include/package.hxx"
#include "../include/storage_types.hxx"

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

Package::~Package() {
    _assigned_IDs.erase(_id);
    _freed_IDs.insert(_id);
    if (_assigned_IDs.empty()) {
        _freed_IDs.clear();
    }
}