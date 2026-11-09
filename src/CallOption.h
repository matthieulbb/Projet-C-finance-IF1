#pragma once
#include "EuropeanVanillaOption.h"

class CallOption : public EuropeanVanillaOption
{
    public:
    
        using EuropeanVanillaOption::EuropeanVanillaOption;
        double payoff(double price) override;
        optionType GetOptionType() override;

};