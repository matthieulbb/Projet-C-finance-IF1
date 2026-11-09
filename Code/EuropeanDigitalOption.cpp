#include "EuropeanDigitalOption.h"
#include <stdexcept>

// Constructeur : initialise la base Option puis le strike.
EuropeanDigitalOption::EuropeanDigitalOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (_strike < 0.0)
    {
        throw std::invalid_argument("EuropeanDigitalOption: strike must be nonnegative.");
    }
}

// Accesseur protege utilise par les classes derivees.
double EuropeanDigitalOption::strike() const
{
    return _strike;
}