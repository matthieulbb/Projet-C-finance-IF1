#pragma once 
#include "EuropeanVanillaOption.h"

class PutOption : public EuropeanVanillaOption
{
    public:
        using EuropeanVanillaOption::EuropeanVanillaOption;
        optionType GetOptionType() override;
        double payoff(double price) override;

};