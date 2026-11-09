#include "EuropeanVanillaOption.h"
#include <stdexcept>

EuropeanVanillaOption::EuropeanVanillaOption(double expiry,double strike): Option(expiry), _strike(strike)
{
    if(expiry<0||strike<0){
        throw std::invalid_argument("expiry and strike must be nonnegative");
    }
}