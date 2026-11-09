#include "PutOption.h"

// Constructeur : utilise le constructeur de EuropeanVanillaOption.
PutOption::PutOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike)
{
}

// Payoff du Put :
// h(z) = K - z si K >= z
// h(z) = 0     sinon
double PutOption::payoff(double z) const
{
    const double K = strike();

    if (K >= z)
    {
        return K - z;
    }

    return 0.0;
}

// Type de l'option : Put.
optionType PutOption::GetOptionType() const
{
    return put;
}