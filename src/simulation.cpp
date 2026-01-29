#include "simulation.hxx"

void simulate(Factory& factory, TimeOffset d, std::function<void (Factory&, Time t)> handle_reports) {
    if (!factory.is_consistent()) { throw std::logic_error("Provided factory is not consistent! Simulation aborted."); }
    for (Time current_time = 1; current_time <= d; current_time++) {
        factory.do_deliveries(current_time);
        factory.do_package_passing();
        factory.do_work(current_time);
        handle_reports(factory, current_time);
    }
}