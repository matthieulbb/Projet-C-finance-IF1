#include "AsianPutOption.h"
#include <stdexcept>

// Constructeur : initialise AsianOption puis le strike.
AsianPutOption::AsianPutOption(const std::vector<double>& time_steps, double strike)
    : AsianOption(time_steps), _strike(strike)
{
    if (_strike < 0.0)
    {
        throw std::invalid_argument("AsianPutOption: strike must be nonnegative.");
    }
}

// Payoff du Put :
// h(z) = K - z si K >= z
// h(z) = 0     sinon
double AsianPutOption::payoff(double z) const
{
    if (_strike >= z)
    {
        return _strike - z;
    }

    return 0.0;
}