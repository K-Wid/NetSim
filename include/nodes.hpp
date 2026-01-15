#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "package.hpp"
#include "types.hpp"

class IPackageReceiver {
public:
    virtual void receive_package(Package&&) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    ReceiverPreferences(ProbabilityGenerator);
    void add_receiver(IPackageReceiver*);
    void remove_receiver(IPackageReceiver*);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences();
};

class PackageSender {
public:
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    void send_package();
    std::optional<Package>& get_sending_buffer();

protected:
    void push_package(Package&&);
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID, TimeOffset);
    void deliver_goods(Time);
    TimeOffset get_delivery_interval() const;
    ElementID get_id() const;
};

class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID, TimeOffset, std::unique_ptr<IPackageReceiver>);
    void do_work(Time);
    TimeOffset get_processing_duration() const;
    Time get_package_processing_start_time() const;

    void receive_package(Package&&) override;
    ElementID get_id() const override;
};

class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID, std::unique_ptr<IPackageReceiver>);

    void receive_package(Package&&) override;
    ElementID get_id() const override;
};

#endif //NETSIM_NODES_HPP