#include "EuropeanDigitalCallOption.h"

// Constructeur : utilise le constructeur de EuropeanDigitalOption.
EuropeanDigitalCallOption::EuropeanDigitalCallOption(double expiry, double strike)
    : EuropeanDigitalOption(expiry, strike)
{
}

// Payoff du Digital Call :
// h(z) = 1 si z >= K
// h(z) = 0 sinon
double EuropeanDigitalCallOption::payoff(double z) const
{
    if (z >= strike())
    {
        return 1.0;
    }

    return 0.0;
}

// Type de l'option : Call.
optionType EuropeanDigitalCallOption::GetOptionType() const
{
    return optionType::call;
}