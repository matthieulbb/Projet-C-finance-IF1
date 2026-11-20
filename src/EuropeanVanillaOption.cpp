#include "EuropeanVanillaOption.h"
#include <stdexcept>

// Constructeur : initialise la base Option puis le strike.
// Le sujet demande de s'assurer que les arguments sont positifs ou nuls.
EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike)
    : Option(expiry), _strike(strike)
{
    if (_strike < 0.0)
    {
        throw std::invalid_argument("EuropeanVanillaOption: strike must be nonnegative.");
    }
}

// Accesseur protege utilise par les classes derivees.
double EuropeanVanillaOption::strike() const
{
    return _strike;
}