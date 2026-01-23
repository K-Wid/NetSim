//
// Created by stasz on 17.01.2026.
//

#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <stdexcept>
#include "types.hxx"

#include "nodes.hxx"

template<class Node>
class NodeCollection
{
    private:
    std::list<Node> _nodes;

    public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node)
    {
        _nodes.push_back(std::move(node));
    };
    void remove_by_id(ElementID id)
    {
        auto iter = std::find_if(_nodes.begin(), _nodes.end(), id);
        if (iter != _nodes.end() || *iter == id)
        {
            _nodes.erase(iter);
        }

    };
    NodeCollection<Node>::iterator find_by_id(ElementID id)
    {
        auto iter = std::find_if(_nodes.begin(), _nodes.end(), id);
        if (iter != _nodes.end() || *iter == id)
        {
            return iter;
        }
        else
        {
            throw std::logic_error("Didn't find such node");
        }
    };
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const
    {
        auto iter = std::find_if(_nodes.begin(), _nodes.end(), id);
        if (iter != _nodes.end() || *iter == id)
        {
            return iter;
        }
        {
            throw std::logic_error("Didn't find such node");
        }
    };


    iterator begin(){return _nodes.begin();};
    const_iterator cbegin() const {return _nodes.cbegin();};
    iterator end() {return _nodes.end();};
    const_iterator cend() const {return _nodes.cend();};

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
    void add_ramp(Ramp&& ramp) {_ramps.add(std::move(ramp));};
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const{return _ramps.cbegin();};
    NodeCollection<Ramp>::const_iterator ramp_cend() const{return _ramps.cend();};

    void add_worker(Worker&& worker) {_workers.add(std::move(worker));};
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return _workers.cbegin();};
    NodeCollection<Worker>::const_iterator worker_cend() const {return _workers.cend();};

    void add_storehouse(Storehouse&& storehouse) {_storehouses.add(std::move(storehouse));};
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return _storehouses.cbegin();};
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return _storehouses.cend();};

    private:
    void remove_reciever(Workers& collection, ElementID id);


    public:
    bool is_consistent() const;
    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);


};
#endif //FACTORY_HPP
