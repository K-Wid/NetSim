//
// Created by stasz on 17.01.2026.
//

#include "factory.hxx"
#include "nodes.hxx"
#include <types.hxx>

/*void Factory::remove_reciever(Workers& collection, ElementID id)
{
;
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
}

void Factory::remove_reciever(Storehouses& collection, ElementID id)
{
    collection.remove_by_id(id);

    for (auto it = _workers.begin(); it != _workers.end(); it++){
        for (auto [fst, snd] : it->receiver_preferences_) {
            if (fst->get_id() == id)
            {
                it->receiver_preferences_.remove_receiver(fst);
            }
        }
    }
}*/

enum class node_color
{
    Visited, NotVisited, Verified
};

bool is_sender_having_reacheable_storehouse(PackageSender* sender,std::map<PackageSender*,node_color> Color)
{
    if (Color[sender] == node_color::Verified)
    {
        return true;
    }

    Color[sender] = node_color::Visited;

    if (sender->receiver_preferences_.get_preferences().empty() )
    {
        throw std::logic_error("No  receiver were found");
    }

    bool is_sender_having_at_least_one_other_receiver_that_is_not_himself = false;

    for (auto Sender: sender->receiver_preferences_)
    {
        if (Sender.first->get_receiver_type() == ReceiverType::Storehouse)
        {
            is_sender_having_at_least_one_other_receiver_that_is_not_himself = false;
        }
        else
        {
            IPackageReceiver* receiver_ptr = Sender.first;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if (sendrecv_ptr == sender)
            {
                break;
            }
            is_sender_having_at_least_one_other_receiver_that_is_not_himself = true;
            if (Color[sendrecv_ptr] ==node_color::NotVisited)
            {
                return is_sender_having_reacheable_storehouse(sender, Color);
            }

        }

    }

    Color[sender] = node_color::Visited;
    if (is_sender_having_at_least_one_other_receiver_that_is_not_himself)
    {
        return true;
    }
    else
    {
        throw std::logic_error("Issue :((");
    }
}

bool Factory::is_consistent() //const
{
    std::map<PackageSender*,node_color> color;
    for (auto& worker: _workers)
    {
        color.insert(std::pair<PackageSender*,node_color>(&worker,node_color::NotVisited));
    }
    for (auto& ramp: _ramps)
    {
        color.insert(std::pair<PackageSender*,node_color>(&ramp,node_color::NotVisited));
    }

    try
    {
        for (auto& RRamp: _ramps)
        {
            is_sender_having_reacheable_storehouse(&RRamp, color);
        }
    }
    catch (...)
    {
        return false;
    }

    return true;



}



void Factory::do_deliveries(Time time)
{
    for (auto it = _ramps.begin();it != _ramps.end(); it++)
    //for (auto ramp: _ramps)
    {
        it->deliver_goods(time);
    }
}


void Factory::do_package_passing()
{
    /*for (auto ramp: _ramps)
    {
        ramp.send_package();
    }
    for (auto worker: _workers)
    {
        worker.send_package();
    }*/
    for (auto it = _ramps.begin(); it != _ramps.end(); it++)
    {
        it->send_package();
    }
    for (auto it = _workers.begin(); it != _workers.end(); it++)
    {
        it->send_package();
    }

}


void Factory::do_work(Time time)
{
    /*for (auto worker: _workers)
    {
        worker.do_work(time);
    }*/
    for (auto it= _workers.begin(); it != _workers.end(); it++)
    {
        it->do_work(time);
    }
}

void Factory::remove_ramp(ElementID id) {
    // Usunąć rampę z preferencji pracowników i magazynów
    _ramps.remove_by_id(id);
}

void Factory::remove_worker(ElementID id) {
    // Usunąć pracownika z preferencji ramp i magazynów
    _workers.remove_by_id(id);
}

void Factory::remove_storehouse(ElementID id) {
    // Usunąć magazyn z preferencji ramp i pracowników
    _storehouses.remove_by_id(id);
}


// ========== Factory IO ==========

ParsedLineData parse_line(std::string& line) {
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);

    while (std::getline(token_stream, token, ' ')) {
        tokens.push_back(token);
    }

    ParsedLineData parsed_line;

    std::string& type = tokens.front();
    if (type == "RAMP") {
        parsed_line.element_type = ElementType::RAMP;
    } else if (type == "WORKER") {
        parsed_line.element_type = ElementType::WORKER;
    } else if (type == "STOREHOUSE") {
        parsed_line.element_type = ElementType::STOREHOUSE;
    } else if (type == "LINK") {
        parsed_line.element_type = ElementType::LINK;
    } else {
        throw std::invalid_argument("Beggining of a parsed line not recognisable. Line : " + line);
    }


}
