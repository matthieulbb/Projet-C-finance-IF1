#pragma once
#include "Option.h"

enum class optionType
{
    call,
    put
};

class EuropeanVanillaOption : public Option
{
    private:
        double _strike;
    public:
        EuropeanVanillaOption(double expiry,double strike);
        virtual optionType GetOptionType() = 0;
};