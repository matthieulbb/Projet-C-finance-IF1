#include "CallOption.h"

optionType CallOption::GetOptionType()
{
    return optionType::call;
}

double CallOption::payoff(double price)
{
    double payoff = 0;
    if(CallOption::GetOptionType() == optionType::call)
    {
        if(price >= CallOption::getStrike())
        {
            payoff = price - CallOption::getStrike();
        }
        else
        {
            payoff = 0;
        }
    }
    else if(CallOption::GetOptionType() == optionType::put)
    {
        if(price <= CallOption::getStrike())
        {
            payoff = CallOption::getStrike() - price;
        }
        else
        {
            payoff = 0;
        }
    }
    return payoff;
}