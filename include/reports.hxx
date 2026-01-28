//
// Created by stasz on 28.01.2026.
//

#ifndef REPORTS_HPP
#define REPORTS_HPP

#include "factory.hxx"

// ========== Reporting ==========

void generate_structure_report(const Factory& factory, std::ostream& outputstream);

void generate_simulation_turn_report(const Factory& factory, std::ostream& outputstream, Time time);

#endif //REPORT_GENERATION_HPP
