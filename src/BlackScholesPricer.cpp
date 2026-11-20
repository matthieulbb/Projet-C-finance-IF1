#include "BlackScholesPricer.h"
#include <cmath>
#include <stdexcept>

BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option, double asset_price, double inetrest_rate, double volatility) : _option(option), _asset_price(asset_price), _interest_rate(inetrest_rate), _volatility(volatility)
{
} 

double BlackScholesPricer::operator()()
{
    double T = _option->getExpiry();
    double K = _option->_strike;
    if (K == 0.0)
    {
        if (_option->GetOptionType() == optionType::call)
        {
            return _asset_price;
        }
        return 0.0;
    }
    if (_volatility == 0.0)
    {
        double prixFinal = _asset_price * std::exp(_interest_rate * T);
        return std::exp(-_interest_rate * T) * _option->payoff(prixFinal);
    }
    double price = 0;
    if(T ==0.0)
    {
        return _option->payoff(_asset_price);
    }

    double d1 = (std::log(_asset_price / K) + (_interest_rate + _volatility * _volatility / 2.0) * T) / (_volatility * std::sqrt(T));

    double d2 = d1 - _volatility * std::sqrt(T);

    if(_option->GetOptionType() == optionType::call){

        price = _asset_price*0.5*std::erfc(-d1/std::sqrt(2)) - K*std::exp(-_interest_rate*T)* 0.5*std::erfc(-d2/std::sqrt(2));
    }
    else
    {
        price = K *std::exp(-_interest_rate * T) * 0.5* std::erfc(d2 / std::sqrt(2.0))- _asset_price * 0.5 * std::erfc(d1 / std::sqrt(2.0));
    }
    return price;

}
double BlackScholesPricer::delta(){
    double T = _option->getExpiry();
    double K = _option->_strike;
    if (K == 0.0)
    {
        if (_option->GetOptionType() == optionType::call)
        {
            return 1.0;
        }
        return 0.0;
    }
    
    if (T == 0.0 || _volatility == 0.0)
    {
        double seuil = K * std::exp(-_interest_rate * T);
        if (_asset_price > seuil && _option->GetOptionType() == optionType::call) {return 1.0;}
        if (_asset_price > seuil && _option->GetOptionType() == optionType::put) {return 0.0;}
        if (_asset_price < seuil && _option->GetOptionType() == optionType::call) {return 0.0;}
        if (_asset_price < seuil && _option->GetOptionType() == optionType::put) {return -1.0;}

        if(_asset_price == seuil)
        {
            throw std::domain_error("Delta non defini au seuil");
        }

    }
    double d1 = (std::log(_asset_price / K) + (_interest_rate + _volatility * _volatility / 2.0) * T) / (_volatility * std::sqrt(T));
    double Nd1 = 0.5 * std::erfc(-d1 / std::sqrt(2.0));

    if (_option->GetOptionType() == optionType::call)
    {
        return Nd1;
    }
    else
    {
        return Nd1 - 1.0;
    }
}