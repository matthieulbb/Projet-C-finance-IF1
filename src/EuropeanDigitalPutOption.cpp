#include "EuropeanDigitalPutOption.h"

// Constructeur : utilise le constructeur de EuropeanDigitalOption.
EuropeanDigitalPutOption::EuropeanDigitalPutOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike)
{
}

// Payoff du Digital Put :
// h(z) = 1 si z <= K
// h(z) = 0 sinon
double EuropeanDigitalPutOption::payoff(double z) const
{
    if (z <= strike())
    {
        return 1.0;
    }

    return 0.0;
}

// Type de l'option : Put.
optionType EuropeanDigitalPutOption::GetOptionType() const
{
    return optionType::put;
}