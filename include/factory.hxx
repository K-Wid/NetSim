//
// Created by stasz on 17.01.2026.
//

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <stdexcept>
#include <sstream>

#include "types.hxx"
#include "nodes.hxx"

template<typename Node>
class NodeCollection
{
    private:
    std::list<Node> _nodes;

    public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node&& node){_nodes.push_back(std::move(node));};
    void remove_by_id(ElementID id);

    iterator find_by_id(ElementID id);

    const_iterator find_by_id(ElementID id) const;



    iterator begin(){return _nodes.begin();};
    const_iterator cbegin() const {return _nodes.cbegin();};
    iterator end() {return _nodes.end();};
    const_iterator cend() const {return _nodes.cend();};

};


template<typename Node>
void NodeCollection<Node>::remove_by_id(ElementID id)
{
    auto iter = std::find_if(_nodes.begin(), _nodes.end(), [id](auto& node){ return node.get_id() == id; });
    if (iter != _nodes.end())
    {
        _nodes.erase(iter);
    }

};

template<typename Node>
typename NodeCollection<Node>::iterator NodeCollection<Node>::find_by_id(ElementID id)
{
    auto iter = std::find_if(_nodes.begin(), _nodes.end(), [id](auto& node){ return node.get_id() == id; });
    if (iter != _nodes.end())
    {
        return iter;
    }
    else
    {
        throw std::logic_error("Didn't find such node");
    }
};

template<typename Node>
typename NodeCollection<Node>::const_iterator NodeCollection<Node>::find_by_id(ElementID id) const
{
    auto iter = std::find_if(_nodes.cbegin(), _nodes.cend(), [id](const auto& node){ return node.get_id() == id; });
    if (iter != _nodes.end())
    {
        return iter;
    }
    else
    {
        throw std::logic_error("Didn't find such node");
    }
};






class Factory
{

    using Workers = NodeCollection<Worker>;
    using Storehouses = NodeCollection<Storehouse>;
    using Ramps = NodeCollection<Ramp>;

    private:
    Workers _workers;
    Storehouses _storehouses;
    Ramps _ramps;

    public:
    void add_ramp(Ramp&& ramp) {_ramps.add(std::move(ramp));};
    void remove_ramp(ElementID id);
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){return _ramps.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const{return _ramps.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const{return _ramps.cbegin();};
    NodeCollection<Ramp>::const_iterator ramp_cend() const{return _ramps.cend();};

    void add_worker(Worker&& worker) {_workers.add(std::move(worker));};
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return _workers.find_by_id((id));};
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const{return _workers.find_by_id(id);};
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return _workers.cbegin();};
    NodeCollection<Worker>::const_iterator worker_cend() const {return _workers.cend();};

    void add_storehouse(Storehouse&& storehouse) {_storehouses.add(std::move(storehouse));};
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){return _storehouses.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const{return _storehouses.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return _storehouses.cbegin();};
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return _storehouses.cend();};

    private:
    template<typename Node>
    void remove_reciever(NodeCollection<Node> collection, ElementID id)
    {
        collection.remove_by_id(id);
        for (auto it = _ramps.begin();it != _ramps.end(); it++){
            for (auto [fst, snd] : it->receiver_preferences_) {
                if (fst->get_id() == id)
                {
                    it->receiver_preferences_.remove_receiver(fst);
                }
            }
        }
        for (auto it = _workers.begin(); it != _workers.end(); it++){
            for (auto [fst, snd] : it->receiver_preferences_) {
                if (fst->get_id() == id)
                {
                    it->receiver_preferences_.remove_receiver(fst);
                }
            }
        }
    };
    //void remove_reciever(Workers& collection, ElementID id);
    //void remove_reciever(Storehouses& collection, ElementID id);

    public:
    bool is_consistent();// const;
    void do_deliveries(Time time);
    void do_package_passing();
    void do_work(Time time);


};


// ========== Factory IO ==========

enum class ElementType {
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

struct ParsedLineData {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

// Used for capitalized names - beginnings of lines
ElementType elementtype_from_string(std::string s);

// Used in load_factory_structure() - uses lowercase shortened versions - used in LINK in input file
ElementType link_elem_type_from_string(std::string s);

// Used in load_factory_structure()
ParsedLineData parse_line(std::string line);

Factory load_factory_structure(std::istream& input_stream);

void save_factory_structure(Factory& factory, std::ostream& output_stream);

#endif //FACTORY_HPP
