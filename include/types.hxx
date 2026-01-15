#ifndef NETSIM_BEZ_GIT_TYPES_HXX
#define NETSIM_BEZ_GIT_TYPES_HXX

#include <functional>
#include <list>

using ElementID = unsigned int;

using Time = unsigned int;
using TimeOffset = unsigned int;
using ProbabilityGenerator = std::function<double()>;

#endif //NETSIM_BEZ_GIT_TYPES_HXX