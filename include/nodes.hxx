#ifndef NETSIM_NODES_HXX
#define NETSIM_NODES_HXX

#include "helpers.hxx"
#include "package.hxx"
#include "types.hxx"
#include "storage_types.hxx"

// Zawiera EXERCISE_ID_NODES
#include "config.hpp"

enum class ReceiverType {
    Ramp, Worker, Storehouse
};

class IPackageReceiver {
public:
    virtual void receive_package(Package&&) = 0;
    virtual ElementID get_id() const = 0;
#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    virtual ReceiverType get_receiver_type() const = 0;
#endif
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator probability_generator = default_probability_generator) : probability_generator_(probability_generator) {};
    void add_receiver(IPackageReceiver*);
    void remove_receiver(IPackageReceiver*);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const { return preferences; }

    const_iterator begin() const { return preferences.begin(); }
    const_iterator cbegin() const { return preferences.cbegin(); }
    const_iterator end() const { return preferences.end(); }
    const_iterator cend() const { return preferences.cend(); }
private:
    ProbabilityGenerator probability_generator_;
    preferences_t preferences;

    void force_uniform_probabilities();
};

class PackageSender {
private:
    std::optional<Package> sending_buffer_;
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const { return sending_buffer_; }
protected:
    void push_package(Package&&);
};

class Storehouse : public IPackageReceiver {
private:
    const ElementID id_;
    std::unique_ptr<IPackageStockpile> stockpile_;
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> package_stockpile = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), stockpile_(std::move(package_stockpile)) {}
    Storehouse(Storehouse&&) = default;
    // IPackageReceiver
    void receive_package(Package&& package) override { stockpile_->push(std::move(package)); }
    ElementID get_id() const override { return id_; }
#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const override { return ReceiverType::Storehouse; }
#endif
    IPackageStockpile::const_iterator begin() const override { return stockpile_->begin(); }
    IPackageStockpile::const_iterator cbegin() const override { return stockpile_->cbegin(); }
    IPackageStockpile::const_iterator end() const override { return stockpile_->end(); }
    IPackageStockpile::const_iterator cend() const override { return stockpile_->cend(); }

    ~Storehouse() override = default;
};

class Ramp : public PackageSender {
private:
    const ElementID id_;
    TimeOffset delivery_interval_;
public:
    Ramp(ElementID id, TimeOffset delivery_interval) : PackageSender(), id_(id), delivery_interval_(delivery_interval) {}
    void deliver_goods(Time);
    TimeOffset get_delivery_interval() const { return delivery_interval_; }
    ElementID get_id() const { return id_; }
};

class Worker : public IPackageReceiver, public PackageSender {
private:
    const ElementID id_;
    TimeOffset processing_duration_;
    Time processing_start_time_;
    std::unique_ptr<IPackageQueue> package_queue_;
    std::optional<Package> package_currently_processed;
public:
    Worker(ElementID id, TimeOffset processing_duration, std::unique_ptr<IPackageQueue> package_queue) : id_(id), processing_duration_(processing_duration), package_queue_(std::move(package_queue)) {}
    void do_work(Time);
    TimeOffset get_processing_duration() const { return processing_duration_; }
    Time get_package_processing_start_time() const { return processing_start_time_; }

    void receive_package(Package&& package) override { package_queue_->push(std::move(package)); }
    ElementID get_id() const override { return id_; }
#if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const override { return ReceiverType::Worker; }
#endif
    IPackageStockpile::const_iterator begin() const override { return package_queue_->begin(); }
    IPackageStockpile::const_iterator cbegin() const override { return package_queue_->cbegin(); }
    IPackageStockpile::const_iterator end() const override { return package_queue_->end(); }
    IPackageStockpile::const_iterator cend() const override { return package_queue_->cend(); }
};

#endif //NETSIM_NODES_HXX