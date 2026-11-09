#include "PutOption.h"

optionType PutOption::GetOptionType()
{
    return optionType::put;
}

double PutOption::payoff(double price)
{
    double payoff = 0;
    if(EuropeanVanillaOption::getStrike() >= price )
    {
        payoff = getStrike() - price;
    }
    return payoff;
}