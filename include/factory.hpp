//
// Created by stasz on 17.01.2026.
//

#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <types.hpp>

#include "nodes.hpp"

template<class Node>
class NodeCollection
{
    public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node);
    void remove_by_id(ElementID id);
    NodeCollection<Node>::iterator find_by_id(ElementID id);
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const;


    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;

};

using Workers = NodeCollection<Worker>;
using Storehouses = NodeCollection<Storehouse>;
using Ramps = NodeCollection<Ramp>;


class Factory
{
    private:
    Workers _workers;
    Storehouses _storehouses;
    Ramps _ramps;

    public:
    void add_ramp(Ramp&&);
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const;
    NodeCollection<Ramp>::const_iterator ramp_cend() const;

    void add_worker(Worker&&);
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;
    NodeCollection<Worker>::const_iterator worker_cbegin() const;
    NodeCollection<Worker>::const_iterator worker_cend() const;

    void add_storehouse(Storehouse&&);
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const;
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const;

    private:
    void remove_reciever(NodeCollection<Node>& collection, ElementID id);


    public:
    bool is_consistent(void) const;
    void do_deliveries(Time);
    void do_package_passing(void);
    void do_work(Time);


};
#endif //FACTORY_HPP
