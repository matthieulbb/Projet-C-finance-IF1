#pragma once
#include "Option.h"

enum class optionType
{
    call,
    put
};

class EuropeanVanillaOption : public Option
{
    protected:
        double getStrike();
    private:
        double _strike;
        friend class BlackScholesPricer;
    public:
        EuropeanVanillaOption(double expiry,double strike);
        virtual optionType GetOptionType() = 0;
};