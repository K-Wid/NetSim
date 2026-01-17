//
// Created by stasz on 17.01.2026.
//

#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <types.hpp>

template<class Node>
class NodeCollection
{
    public:
    void add(Node&& node);
    void remove_by_id(ElementID id);
    NodeCollection<Node>::iterator find_by_id(ElementID id);
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const;

    using iterator = std::list<Node>::iterator;
    using const_iterator = std::list<Node>::const_iterator;
    
    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;

};

#endif //FACTORY_HPP
