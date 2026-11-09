#include "CallOption.h"

optionType CallOption::GetOptionType()
{
    return optionType::call;
}

double CallOption::payoff(double price)
{
    double payoff = 0;
    
    if(price >= CallOption::getStrike())
    {
        payoff = price - CallOption::getStrike();
    }
    else
    {
        payoff = 0;
    }
    return payoff;
}