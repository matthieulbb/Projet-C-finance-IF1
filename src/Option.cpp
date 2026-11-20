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

// Version par defaut de payoffPath.
// Pour une option europeenne, le chemin est de longueur m = 1,
// et le payoff depend seulement de la derniere valeur S_tm.
double Option::payoffPath(const std::vector<double>& path) const
{
    if (path.empty())
    {
        throw std::invalid_argument("Option: path must not be empty.");
    }

    return payoff(path.back());
}

// Une option n'est pas asiatique par defaut.
bool Option::isAsianOption() const
{
    return false;
}