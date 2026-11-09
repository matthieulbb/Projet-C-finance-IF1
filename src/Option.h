#pragma once

class Option
{
    private: 
        double _expiry;
    public:
        Option(double expiry);
        virtual double payoff(double price) = 0;
        double getExpiry();
};
