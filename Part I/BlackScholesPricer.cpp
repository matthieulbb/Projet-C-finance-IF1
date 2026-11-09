#include "BlackScholesPricer.h"
#include <cmath>
#include <stdexcept>

namespace
{
    // Fonction de repartition de la loi normale centree-reduite.
    // N(x) = 0.5 * erfc(-x / sqrt(2))
    double normalCdf(double x)
    {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

    // Actualisation d'un montant sur t periodes au taux r.
    double discount(double amount, double r, double t)
    {
        return amount * std::exp(-r * t);
    }
}

// Constructeur : initialise les donnees de pricing.
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
                                       double asset_price,
                                       double interest_rate,
                                       double volatility)
    : _option(option),
      _asset_price(asset_price),
      _interest_rate(interest_rate),
      _volatility(volatility)
{
    if (_option == nullptr)
    {
        throw std::invalid_argument("BlackScholesPricer: option must not be null.");
    }

    if (_asset_price < 0.0)
    {
        throw std::invalid_argument("BlackScholesPricer: asset_price must be nonnegative.");
    }

    if (_volatility < 0.0)
    {
        throw std::invalid_argument("BlackScholesPricer: volatility must be nonnegative.");
    }
}

// Calcul du prix Black-Scholes.
double BlackScholesPricer::operator()() const
{
    const double T = _option->getExpiry();
    const double K = _option->_strike;
    const bool isCall = (_option->GetOptionType() == call);

    // Cas limite : expiration immediate.
    if (T <= 0.0)
    {
        return _option->payoff(_asset_price);
    }

    // Cas limite : strike nul.
    if (K <= 0.0)
    {
        if (isCall)
        {
            return _asset_price;
        }

        return 0.0;
    }

    // Cas limite : underlying nul.
    if (_asset_price <= 0.0)
    {
        if (isCall)
        {
            return 0.0;
        }

        return discount(K, _interest_rate, T);
    }

    // Cas limite : volatilite nulle.
    // On utilise alors la valeur forward deterministe.
    if (_volatility <= 0.0)
    {
        const double forward = _asset_price * std::exp(_interest_rate * T);
        return discount(_option->payoff(forward), _interest_rate, T);
    }

    // Parametres classiques de Black-Scholes.
    const double sqrtT = std::sqrt(T);
    const double sigmaSqrtT = _volatility * sqrtT;

    const double d1 =
        (std::log(_asset_price / K) +
         (_interest_rate + 0.5 * _volatility * _volatility) * T) /
        sigmaSqrtT;

    const double d2 = d1 - sigmaSqrtT;

    if (isCall)
    {
        // Call = S * N(d1) - K * exp(-rT) * N(d2)
        return _asset_price * normalCdf(d1) -
               discount(K, _interest_rate, T) * normalCdf(d2);
    }

    // Put = K * exp(-rT) * N(-d2) - S * N(-d1)
    return discount(K, _interest_rate, T) * normalCdf(-d2) -
           _asset_price * normalCdf(-d1);
}

// Calcul du Delta Black-Scholes.
double BlackScholesPricer::delta() const
{
    const double T = _option->getExpiry();
    const double K = _option->_strike;
    const bool isCall = (_option->GetOptionType() == call);

    // Cas limite : expiration immediate.
    // Le delta est alors le comportement de la derivee du payoff.
    if (T <= 0.0)
    {
        if (isCall)
        {
            return (_asset_price > K) ? 1.0 : 0.0;
        }

        return (_asset_price < K) ? -1.0 : 0.0;
    }

    // Cas limite : strike nul.
    if (K <= 0.0)
    {
        if (isCall)
        {
            return 1.0;
        }

        return 0.0;
    }

    // Cas limite : underlying nul.
    if (_asset_price <= 0.0)
    {
        if (isCall)
        {
            return 0.0;
        }

        return -1.0;
    }

    // Cas limite : volatilite nulle.
    if (_volatility <= 0.0)
    {
        const double discountedStrike = discount(K, _interest_rate, T);

        if (isCall)
        {
            return (_asset_price > discountedStrike) ? 1.0 : 0.0;
        }

        return (_asset_price < discountedStrike) ? -1.0 : 0.0;
    }

    // Calcul standard du delta.
    const double sqrtT = std::sqrt(T);
    const double sigmaSqrtT = _volatility * sqrtT;

    const double d1 =
        (std::log(_asset_price / K) +
         (_interest_rate + 0.5 * _volatility * _volatility) * T) /
        sigmaSqrtT;

    if (isCall)
    {
        // Delta Call = N(d1)
        return normalCdf(d1);
    }

    // Delta Put = N(d1) - 1
    return normalCdf(d1) - 1.0;
}