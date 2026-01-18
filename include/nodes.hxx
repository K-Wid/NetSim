#ifndef NETSIM_NODES_HXX
#define NETSIM_NODES_HXX

#include "helpers.hxx"
#include "package.hxx"
#include "types.hxx"
#include "storage_types.hxx"
//
// class IPackageReceiver {
// public:
//     virtual void receive_package(Package&&) = 0;
//     virtual ElementID get_id() const = 0;
//     virtual IPackageStockpile::const_iterator begin() const;
//     virtual IPackageStockpile::const_iterator cbegin() const;
//     virtual IPackageStockpile::const_iterator end() const;
//     virtual IPackageStockpile::const_iterator cend() const;
//     virtual ~IPackageReceiver() = default;
// };
//
// class ReceiverPreferences {
// public:
//     using preferences_t = std::map<IPackageReceiver*, double>;
//     using const_iterator = preferences_t::const_iterator;
//
//     const_iterator begin() const;
//     const_iterator end() const;
//     const_iterator cbegin() const;
//     const_iterator cend() const;
//
//     ReceiverPreferences(ProbabilityGenerator probability_generator = default_probability_generator) : _probability_generator(probability_generator) {};
//     void add_receiver(IPackageReceiver*);
//     void remove_receiver(IPackageReceiver*);
//     IPackageReceiver* choose_receiver();
//     preferences_t& get_preferences() { return _preferences; }
//
// private:
//     preferences_t _preferences;
//     ProbabilityGenerator _probability_generator;
// };
//
// class PackageSender {
// public:
//     PackageSender() = default;
//     PackageSender(PackageSender&&) = default;
//     void send_package();
//     std::optional<Package>& get_sending_buffer() { return _sending_buffer; }
//
// protected:
//     void push_package(Package&&);
//
// private:
//     std::optional<Package> _sending_buffer;
// };
//
// class Ramp : public PackageSender {
// public:
//     Ramp(ElementID, TimeOffset);
//     void deliver_goods(Time);
//     TimeOffset get_delivery_interval() const;
//     ElementID get_id() const;
// };
//
// class Worker : public PackageSender, public IPackageReceiver {
// public:
//     Worker(ElementID, TimeOffset, std::unique_ptr<IPackageReceiver>);
//     void do_work(Time);
//     TimeOffset get_processing_duration() const;
//     Time get_package_processing_start_time() const;
//
//     void receive_package(Package&&) override;
//     ElementID get_id() const override;
// private:
//     std::optional<Package> _current_package;
// };
//
// class Storehouse : public IPackageReceiver {
// public:
//     Storehouse(ElementID element_id, std::unique_ptr<IPackageReceiver> receiver /* = std::make_unique<PackageQueue>(QueueType::Fifo) */);
//
//     void receive_package(Package&&) override;
//     IPackageStockpile::const_iterator begin() const override { return _package_receiver->begin(); }
//     IPackageStockpile::const_iterator cbegin() const override { return _package_receiver->cbegin(); }
//     IPackageStockpile::const_iterator end() const override { return _package_receiver->end(); }
//     IPackageStockpile::const_iterator cend() const override { return _package_receiver->cend(); }
//     ElementID get_id() const override { return _id; }
// private:
//     ElementID _id;
//     std::unique_ptr<IPackageReceiver> _package_receiver;
// };

enum class ReceiverType {
    Ramp, Worker, Storehouse
};

class IPackageReceiver {
public:
    virtual void receive_package(Package&&) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;

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
private:
    ProbabilityGenerator probability_generator_;
    preferences_t preferences;

    void force_uniform_probabilities();
};

class PackageSender {
private:
    ReceiverPreferences receiver_preferences_;
    std::optional<Package> sending_buffer_;
public:
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
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> package_stockpile = std::make_unique<PackageQueue>(QueueType::Fifo)) : id_(id), stockpile_(std::move(package_stockpile)) {}

    // IPackageReceiver
    void receive_package(Package&& package) override { stockpile_->push(std::move(package)); }
    ElementID get_id() const override { return id_; }
    ReceiverType get_receiver_type() const override { return ReceiverType::Storehouse; }
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

class Worker : public PackageSender, public IPackageReceiver {
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
};

#endif //NETSIM_NODES_HXX