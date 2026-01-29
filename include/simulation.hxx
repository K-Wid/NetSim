#ifndef SIMULATION_HXX
#define SIMULATION_HXX

#include <set>
#include "factory.hxx"

class IntervalReportNotifier {
private:
    TimeOffset interval_;
public:
    IntervalReportNotifier(TimeOffset interval) : interval_(interval) {}
    bool should_generate_report(Time t) { return ((t % interval_) == 1) || (interval_ == 1);}
};

class SpecificTurnsReportNotifier {
private:
    std::set<Time> turns_;
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(turns) {}
    bool should_generate_report(Time t) { return turns_.count(t) != 0; }
};

void simulate(Factory& factory, TimeOffset d, std::function<void (Factory&, Time t)>);

#endif
