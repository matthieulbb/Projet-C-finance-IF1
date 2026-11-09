#pragma once
#include "Option.h"

class EuropeanVanillaOption : public Option
{

    public:
        EuropeanVanillaOption(double expiry, double strike);

    private: 
        double _strike;

};