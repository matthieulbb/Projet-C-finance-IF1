#pragma once

class Option{

public: 
    Option (double expiry);     //Constructeur
    virtual ~Option()=default;      //destructeur virtuel
    double getExpiry() const;       //getter
    virtual double payoff(double) const =0;     //virtuel pure

private: 
    double _expiry;
};