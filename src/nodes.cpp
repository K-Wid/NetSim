#include "../include/nodes.hxx"

// ========== ReceiverPreferences ==========

void ReceiverPreferences::add_receiver(IPackageReceiver* package_receiver) {
    preferences.insert({package_receiver, 0.0});
    force_uniform_probabilities();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* package_receiver) {
    preferences.erase(package_receiver);
    if (!preferences.empty()) {
        force_uniform_probabilities();
    }
}

void ReceiverPreferences::force_uniform_probabilities() {
    double individual_probability = 1.0 / static_cast<double>(preferences.size());
    for (auto [fst, snd] : preferences) {
        preferences.at(fst) =  individual_probability;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences.empty()) {
        return nullptr;
    }

    double probability_sum = 0.0;
    double random_number = probability_generator_();
    for (auto entry : preferences) {
        probability_sum += entry.second;
        if (probability_sum > random_number) {
            return entry.first;
        }
    }
    return preferences.begin()->first;
}


// ========== PackageSender ==========

void PackageSender::send_package() {
    if (!sending_buffer_.has_value()) {
        return;
    }
    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
    receiver->receive_package(std::move(sending_buffer_.value()));
    sending_buffer_.reset();
}

void PackageSender::push_package(Package&& package) {
    sending_buffer_.emplace(std::move(package));
}


// ========== Ramp ==========

void Ramp::deliver_goods(Time current_time) {
    if (((current_time % delivery_interval_) == 1) || (delivery_interval_ == 1)) {
        push_package(Package());
    }
}


// ========== Worker ==========

void Worker::do_work(Time current_time) {
    if (!package_currently_processed.has_value() && !package_queue_->empty()) {
        package_currently_processed.emplace(package_queue_->pop());
        processing_start_time_ = current_time;
    }
    if (current_time >= processing_start_time_ + processing_duration_ - 1) {
        if (package_currently_processed.has_value()) {
            push_package(std::move(package_currently_processed.value()));
            package_currently_processed.reset();
        }
        if (!package_queue_->empty()) {
            package_currently_processed.emplace(package_queue_->pop());
            processing_start_time_ = current_time;
        }
    }
}
