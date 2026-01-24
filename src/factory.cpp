//
// Created by stasz on 17.01.2026.
//

#include "factory.hxx"
#include "nodes.hxx"
#include <types.hxx>

void Factory::remove_reciever(Workers& collection, ElementID id){
    auto x = collection.find_by_id(id);
    x->receiver_preferences_.remove_receiver(id);
}

