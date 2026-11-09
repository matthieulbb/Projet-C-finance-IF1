#include "Option.h"

Option::Option(double expiry) : _expiry(expiry)
{
}

double Option::getExpiry()
{
    return _expiry;
}