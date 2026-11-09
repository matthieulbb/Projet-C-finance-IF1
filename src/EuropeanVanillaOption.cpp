#include "EuropeanVanillaOption.h"
#include <stdexcept>


EuropeanVanillaOption::EuropeanVanillaOption(double expiry, double strike) : Option(expiry), _strike(strike)
{
    if(expiry < 0 || strike < 0)
    {
        throw std::invalid_argument("Les arguments (expiry et strike) doivent être positifs ou nuls");
    }
}