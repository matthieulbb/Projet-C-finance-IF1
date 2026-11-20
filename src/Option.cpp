#include "Option.h"
#include <stdexcept>

// Constructeur : initialise _expiry et verifie sa validite.
Option::Option(double expiry)
    : _expiry(expiry)
{
    if (_expiry < 0.0)
    {
        throw std::invalid_argument("Option: expiry must be nonnegative.");
    }
}

// Accesseur : renvoie la date d'expiration.
double Option::getExpiry() const
{
    return _expiry;
}