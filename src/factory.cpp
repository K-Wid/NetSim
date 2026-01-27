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

ElementType elementtype_from_string(std::string s) {
    if (s == "LOADING_RAMP") { return ElementType::RAMP; }
    if (s == "WORKER") { return ElementType::WORKER; }
    if (s == "STOREHOUSE") { return ElementType::STOREHOUSE; }
    if (s == "LINK") { return ElementType::LINK; }
    throw std::invalid_argument("Unknown element type: "+s);
}

ElementType link_elem_type_from_string(std::string s) {
    if (s == "ramp") { return ElementType::RAMP; }
    if (s == "worker") { return ElementType::WORKER; }
    if (s == "store") { return ElementType::STOREHOUSE; }
    throw std::invalid_argument("Unknown element type: "+s);
}

ParsedLineData parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    while (std::getline(token_stream, token, ' ')) {
        tokens.push_back(token);
    }
    if (tokens.empty()) { throw std::invalid_argument("Unable to parse. Given line doesn't include space.\n > "+line); }

    ParsedLineData parsed_line;

    std::string type = tokens.front();
    tokens.erase(tokens.begin());
    parsed_line.element_type = elementtype_from_string(type);

    for (auto& entry : tokens) {
        std::istringstream pair(entry);
        std::vector<std::string> pair_tokens;
        while (std::getline(pair, token, '=')) {
            pair_tokens.push_back(token);
        }
        if (pair_tokens.size() != 2) { throw std::invalid_argument("Incorrect KEY=VALUE format\n > "+line); }
        parsed_line.parameters[pair_tokens[0]] = pair_tokens[1];
    }

    return parsed_line;
}

Factory load_factory_structure(std::istream &input_stream) {
    Factory factory;

    std::string line;
    while (std::getline(input_stream, line)) {
        if (line.empty() || line[0] == ';') { continue; }
        ParsedLineData parsed_line = parse_line(line);

        switch (parsed_line.element_type) {
            case ElementType::RAMP: {
                ElementID id = std::stoul(parsed_line.parameters["id"]);
                TimeOffset delivery_interval = std::stoul(parsed_line.parameters["delivery-interval"]);
                factory.add_ramp(Ramp(id, delivery_interval));
                break;
            }
            case ElementType::WORKER: {
                ElementID id = std::stoul(parsed_line.parameters["id"]);
                TimeOffset processing_time = std::stoul(parsed_line.parameters["processing-time"]);
                PackageQueueType queue_type(PackageQueueType::FIFO);
                if (parsed_line.parameters["queue-type"] == "FIFO") {
                    queue_type = PackageQueueType::FIFO;
                } else if (parsed_line.parameters["queue-type"] == "LIFO") {
                    queue_type = PackageQueueType::LIFO;
                }
                factory.add_worker(Worker(id, processing_time, std::make_unique<PackageQueue>(queue_type)));
                break;
            }
            case ElementType::STOREHOUSE: {
                ElementID id = std::stoul(parsed_line.parameters["id"]);
                factory.add_storehouse(Storehouse(id));
                break;
            }
            case ElementType::LINK: {
                std::string src = parsed_line.parameters["src"];
                std::istringstream src_stream(src);
                std::string temp;
                std::getline(src_stream, temp, '-');
                ElementType src_type = link_elem_type_from_string(temp);
                std::getline(src_stream, temp, '-');
                ElementID src_id = std::stoul(temp);

                std::string dest = parsed_line.parameters["dest"];
                std::istringstream dest_stream(dest);
                std::getline(dest_stream, temp, '-');
                ElementType dest_type = link_elem_type_from_string(temp);
                std::getline(dest_stream, temp, '-');
                ElementID dest_id = std::stoul(temp);

                switch (src_type) {
                    case ElementType::RAMP: {
                        Ramp& r = *factory.find_ramp_by_id(src_id);
                        switch (dest_type) {
                            case ElementType::WORKER: {
                                Worker& w = *factory.find_worker_by_id(dest_id);
                                r.receiver_preferences_.add_receiver(&w);
                                break;
                            }
                            case ElementType::STOREHOUSE: {
                                Storehouse& s = *factory.find_storehouse_by_id(dest_id);
                                r.receiver_preferences_.add_receiver(&s);
                                break;
                            }
                        }
                        break;
                    }
                    case ElementType::WORKER: {
                        Worker& w = *factory.find_worker_by_id(src_id);
                        switch (dest_type) {
                            case ElementType::WORKER: {
                                Worker& w2 = *factory.find_worker_by_id(dest_id);
                                w.receiver_preferences_.add_receiver(&w2);
                                break;
                            }
                            case ElementType::STOREHOUSE: {
                                Storehouse& s = *factory.find_storehouse_by_id(dest_id);
                                w.receiver_preferences_.add_receiver(&s);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }

    }

    return factory;
}

void save_factory_structure(Factory &factory, std::ostream &output_stream) {
    output_stream << "; == LOADING RAMPS ==\n\n";
    for (auto ramp_it = factory.ramp_cbegin(); ramp_it != factory.ramp_cend(); ++ramp_it) {
        output_stream << "LOADING_RAMP id=" << ramp_it->get_id() << " delivery-interval=" << ramp_it->get_delivery_interval() << '\n';
    }
    output_stream << "\n; == WORKERS ==\n\n";
    for (auto worker_it = factory.worker_cbegin(); worker_it != factory.worker_cend(); ++worker_it) {
        output_stream << "WORKER id=" << worker_it->get_id() << " processing-time=" << worker_it->get_processing_duration() << " queue-type=" << ((worker_it->get_queue()->get_queue_type() == PackageQueueType::FIFO)? "FIFO" : "LIFO") << '\n';
    }
    output_stream << "\n; == STOREHOUSES ==\n\n";
    for (auto storehouse_it = factory.storehouse_cbegin(); storehouse_it != factory.storehouse_cend(); ++storehouse_it) {
        output_stream << "STOREHOUSE id=" << storehouse_it->get_id() << '\n';
    }
    output_stream << "\n; == LINKS ==\n\n";
    for (auto ramp_it = factory.ramp_cbegin(); ramp_it != factory.ramp_cend(); ++ramp_it) {
        for (const auto& [receiver, p] : ramp_it->receiver_preferences_) {
            output_stream << "LINK src=ramp-" << ramp_it->get_id() << " dest=" << ((receiver->get_receiver_type() == ReceiverType::Worker)? "worker-" : "store-") << receiver->get_id() << '\n';
        }
        output_stream << "\n";
    }
    for (auto worker_it = factory.worker_cbegin(); worker_it != factory.worker_cend(); ++worker_it) {
        for (const auto& [receiver, p] : worker_it->receiver_preferences_) {
            output_stream << "LINK src=worker-" << worker_it->get_id() << " dest=" << ((receiver->get_receiver_type() == ReceiverType::Worker)? "worker-" : "store-") << receiver->get_id() << '\n';
        }
        output_stream << "\n";
    }
}
