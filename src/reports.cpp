//
// Created by stasz on 28.01.2026.
//
#include "reports.hxx"
#include "nodes.hxx"
#include "factory.hxx"
#include "types.hxx"
#include "storage_types.hxx"
#include <ostream>






void generate_structure_report(const Factory& factory, std::ostream& outputstream){
    outputstream << "\n";
    outputstream << "== LOADING RAMPS ==" << "\n\n";
    std::list<ElementID> worker;
    std::list<ElementID> ramp;
    std::list<ElementID> storehouse;


    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it)
    {
        outputstream << "LOADING RAMP #" << it->get_id()<< std::endl;
        outputstream << "  " << "Delivery interval: " << it->get_delivery_interval()<<std::endl;
        for (auto It = it->receiver_preferences_.cbegin();It != it->receiver_preferences_.cend(); ++It)
        {
            switch (It->first->get_receiver_type()){
            case ReceiverType::Worker:
                worker.push_back(It->first->get_id());
                break;
            case ReceiverType::Storehouse:
                storehouse.push_back(It->first->get_id());
                break;
            case ReceiverType::Ramp:
                ramp.push_back(It->first->get_id());
                break;
        }
        }

        worker.sort();
        ramp.sort();
        storehouse.sort();

        outputstream << "  " << "Receivers:" << std::endl;

        for (auto r: ramp)
        {
            outputstream << "    " << "ramp #" << r << std::endl;
        }

        for (auto s: storehouse)
        {
            outputstream << "    " << "storehouse #" << s << std::endl;
        }

        for (auto w: worker)
        {
            outputstream << "    " << "worker #" << w << std::endl;
        }

        worker.clear();
        ramp.clear();
        storehouse.clear();
        outputstream << "\n";

    }



    outputstream << "\n";
    outputstream << "== WORKERS ==" << "\n\n";


    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it)
    {
        outputstream << "WORKER #" << it->get_id()<< std::endl;
        outputstream << "  " << "Processing time: " << it->get_processing_duration()<<std::endl;
        if (it->get_queue()->get_queue_type() == PackageQueueType::FIFO)
        {
            outputstream << "  " << "Queue type: FIFO" <<std::endl;
        }
        if (it->get_queue()->get_queue_type() == PackageQueueType::LIFO)
        {
            outputstream << "  " << "Queue type: LIFO" <<std::endl;
        }

        for (auto It = it->receiver_preferences_.cbegin();It != it->receiver_preferences_.cend(); ++It)
        {
            switch (It->first->get_receiver_type()){
            case ReceiverType::Worker:
                worker.push_back(It->first->get_id());
                break;
            case ReceiverType::Storehouse:
                storehouse.push_back(It->first->get_id());
                break;
            case ReceiverType::Ramp:
                ramp.push_back(It->first->get_id());
                break;
            }
        }

        worker.sort();
        ramp.sort();
        storehouse.sort();

        outputstream << "  " << "Receivers:" << std::endl;
        for (auto r: ramp)
        {
            outputstream << "    " << "ramp #" << r << std::endl;
        }

        for (auto s: storehouse)
        {
            outputstream << "    " << "storehouse #" << s << std::endl;
        }

        for (auto w: worker)
        {
            outputstream << "    " << "worker #" << w << std::endl;
        }

        worker.clear();
        ramp.clear();
        storehouse.clear();
        outputstream << "\n";
    }


    outputstream << "\n";
    outputstream << "== STOREHOUSES ==" << "\n\n";

    for (auto it = factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it)
    {
        outputstream << "STOREHOUSE #" << it->get_id()<< std::endl;
        outputstream << "\n";
    }

}
