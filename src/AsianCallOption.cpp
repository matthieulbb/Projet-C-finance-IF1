#include "AsianCallOption.h"
#include <stdexcept>

// Constructeur : initialise AsianOption puis le strike.
AsianCallOption::AsianCallOption(const std::vector<double>& time_steps, double strike)
    : AsianOption(time_steps), _strike(strike)
{
    if (_strike < 0.0)
    {
        throw std::invalid_argument("AsianCallOption: strike must be nonnegative.");
    }
}

// Payoff du Call :
// h(z) = z - K si z >= K
// h(z) = 0     sinon
double AsianCallOption::payoff(double z) const
{
    if (z >= _strike)
    {
        return z - _strike;
    }

    return 0.0;
}