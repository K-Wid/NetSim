#include "../include/package.hpp"
#include "../include/storage_types.hpp"

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


