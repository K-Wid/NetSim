//
// Created by stasz on 17.01.2026.
//

#include "factory.hxx"
#include "nodes.hxx"
#include <types.hxx>

void Factory::remove_reciever(Workers& collection, ElementID id)
{
;
    _workers.remove_by_id(id);
    for (auto w:_ramps){
        for (auto [fst, snd] : w.receiver_preferences_) {
            if (fst->get_id() == id)
            {
                w.receiver_preferences_.remove_receiver(fst);
            }
        }
    }
    for (auto w:_workers){
        for (auto [fst, snd] : w.receiver_preferences_) {
            if (fst->get_id() == id)
            {
                w.receiver_preferences_.remove_receiver(fst);
            }
        }
    }
}

void Factory::remove_reciever(Storehouses& collection, ElementID id)
{
    _storehouses.remove_by_id(id);

    for (auto w:_workers){
        for (auto [fst, snd] : w.receiver_preferences_) {
            if (fst->get_id() == id)
            {
                w.receiver_preferences_.remove_receiver(fst);
            }
        }
    }
}

enum class node_color
{
    Visited, NotVisited, Verified
};

bool is_sender_having_reacheable_storehouse(Ramp ramp,std::map<PackageSender*,node_color> Color)
{

}

bool Factory::is_consistent() const
{
    std::map<PackageSender*,node_color> color;
    for (auto worker: _workers)
    {
        color.insert(std::pair<PackageSender*,node_color>(&worker,node_color::NotVisited));
    }
    for (auto ramp: _ramps)
    {
        color.insert(std::pair<PackageSender*,node_color>(&ramp,node_color::NotVisited));
    }

    try
    {
        for (auto RRamp: _ramps)
        {
            is_sender_having_reacheable_storehouse(RRamp, color);
        }
    }



}



void Factory::do_deliveries(Time time)
{
    for (auto ramp: _ramps)
    {
        ramp.deliver_goods(time);
    }
}


void Factory::do_package_passing()
{
    for (auto ramp: _ramps)
    {
        ramp.send_package();
    }
    for (auto worker: _workers)
    {
        worker.send_package();
    }
}


void Factory::do_work(Time time)
{
    for (auto worker: _workers)
    {
        worker.do_work(time);
    }
}

