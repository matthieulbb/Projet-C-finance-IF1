#include "CallOption.h"

// Constructeur : utilise le constructeur de EuropeanVanillaOption.
CallOption::CallOption(double expiry, double strike)
    : EuropeanVanillaOption(expiry, strike)
{
}

// Payoff du Call :
// h(z) = z - K si z >= K
// h(z) = 0     sinon
double CallOption::payoff(double z) const
{
    const double K = strike();

    if (z >= K)
    {
        return z - K;
    }

    return 0.0;
}

// Type de l'option : Call.
optionType CallOption::GetOptionType() const
{
    return call;
}