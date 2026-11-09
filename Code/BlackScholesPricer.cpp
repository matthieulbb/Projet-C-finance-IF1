#include "BlackScholesPricer.h"
#include <cmath>
#include <stdexcept>

namespace
{
    // Verification des donnees de marche.
    void validateMarketData(double asset_price, double volatility)
    {
        if (asset_price < 0.0)
        {
            throw std::invalid_argument("BlackScholesPricer: asset_price must be nonnegative.");
        }

        if (volatility < 0.0)
        {
            throw std::invalid_argument("BlackScholesPricer: volatility must be nonnegative.");
        }
    }

    // Fonction de repartition de la loi normale centree-reduite.
    // N(x) = 0.5 * erfc(-x / sqrt(2))
    double normalCdf(double x)
    {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

    // Densite de la loi normale centree-reduite.
    double normalPdf(double x)
    {
        static const double pi = std::acos(-1.0);
        return std::exp(-0.5 * x * x) / std::sqrt(2.0 * pi);
    }

    // Actualisation d'un montant sur t periodes au taux r.
    double discount(double amount, double r, double t)
    {
        return amount * std::exp(-r * t);
    }
}

// Constructeur pour option vanille europeenne.
BlackScholesPricer::BlackScholesPricer(EuropeanVanillaOption* option,
                                       double asset_price,
                                       double interest_rate,
                                       double volatility)
    : _option(option),
      _strike(0.0),
      _expiry(0.0),
      _type(optionType::call),
      _isDigital(false),
      _asset_price(asset_price),
      _interest_rate(interest_rate),
      _volatility(volatility)
{
    if (option == nullptr)
    {
        throw std::invalid_argument("BlackScholesPricer: option must not be null.");
    }

    validateMarketData(asset_price, volatility);

    // BlackScholesPricer est amie de EuropeanVanillaOption,
    // elle peut donc acceder au strike prive.
    _strike = option->_strike;
    _expiry = option->getExpiry();
    _type = option->GetOptionType();
}

// Constructeur surcharge pour option digitale europeenne.
BlackScholesPricer::BlackScholesPricer(EuropeanDigitalOption* option,
                                       double asset_price,
                                       double interest_rate,
                                       double volatility)
    : _option(option),
      _strike(0.0),
      _expiry(0.0),
      _type(optionType::call),
      _isDigital(true),
      _asset_price(asset_price),
      _interest_rate(interest_rate),
      _volatility(volatility)
{
    if (option == nullptr)
    {
        throw std::invalid_argument("BlackScholesPricer: option must not be null.");
    }

    validateMarketData(asset_price, volatility);

    // BlackScholesPricer est amie de EuropeanDigitalOption,
    // elle peut donc acceder au strike prive.
    _strike = option->_strike;
    _expiry = option->getExpiry();
    _type = option->GetOptionType();
}

// Calcul du prix Black-Scholes.
double BlackScholesPricer::operator()() const
{
    if (_option == nullptr)
    {
        throw std::logic_error("BlackScholesPricer: option is null.");
    }

    const double S = _asset_price;
    const double K = _strike;
    const double T = _expiry;
    const bool isCall = (_type == optionType::call);

    // Cas limite : expiration immediate.
    if (T <= 0.0)
    {
        return _option->payoff(S);
    }

    // Cas limite : strike nul ou negatif.
    if (K <= 0.0)
    {
        if (_isDigital)
        {
            // Digital Call : si K <= 0, l'option est toujours in the money
            // pour un sous-jacent positif ou nul.
            if (isCall)
            {
                return discount(1.0, _interest_rate, T);
            }

            // Digital Put : si K < 0, impossible pour un sous-jacent positif.
            if (K < 0.0)
            {
                return 0.0;
            }

            // Cas K == 0 : le put digital ne paie que si S == 0.
            return (S <= 0.0) ? discount(1.0, _interest_rate, T) : 0.0;
        }

        // Vanille : Call avec K = 0 vaut S, Put avec K = 0 vaut 0.
        return isCall ? S : 0.0;
    }

    // Cas limite : sous-jacent nul.
    if (S <= 0.0)
    {
        if (_isDigital)
        {
            // Digital Call : out of the money.
            // Digital Put : in the money si K > 0.
            return isCall ? 0.0 : discount(1.0, _interest_rate, T);
        }

        // Vanille : Call vaut 0, Put vaut K actualise.
        return isCall ? 0.0 : discount(K, _interest_rate, T);
    }

    // Cas limite : volatilite nulle.
    // On utilise alors la valeur forward deterministe.
    if (_volatility <= 0.0)
    {
        const double forward = S * std::exp(_interest_rate * T);
        return discount(_option->payoff(forward), _interest_rate, T);
    }

    const double sqrtT = std::sqrt(T);
    const double sigmaSqrtT = _volatility * sqrtT;

    if (_isDigital)
    {
        // Prix digital cash-or-nothing.
        // Call digital : e^{-rT} N(d2)
        // Put digital  : e^{-rT} N(-d2)
        const double d2 =
            (std::log(S / K) +
             (_interest_rate - 0.5 * _volatility * _volatility) * T) /
            sigmaSqrtT;

        if (isCall)
        {
            return discount(normalCdf(d2), _interest_rate, T);
        }

        return discount(normalCdf(-d2), _interest_rate, T);
    }

    // Formule Black-Scholes pour option vanille.
    const double d1 =
        (std::log(S / K) +
         (_interest_rate + 0.5 * _volatility * _volatility) * T) /
        sigmaSqrtT;

    const double d2 = d1 - sigmaSqrtT;

    if (isCall)
    {
        // Call = S * N(d1) - K * exp(-rT) * N(d2)
        return S * normalCdf(d1) -
               discount(K, _interest_rate, T) * normalCdf(d2);
    }

    // Put = K * exp(-rT) * N(-d2) - S * N(-d1)
    return discount(K, _interest_rate, T) * normalCdf(-d2) -
           S * normalCdf(-d1);
}

// Calcul du Delta Black-Scholes.
double BlackScholesPricer::delta() const
{
    if (_option == nullptr)
    {
        throw std::logic_error("BlackScholesPricer: option is null.");
    }

    const double S = _asset_price;
    const double K = _strike;
    const double T = _expiry;
    const bool isCall = (_type == optionType::call);

    // Cas limite : expiration immediate.
    if (T <= 0.0)
    {
        // Pour une option digitale, le delta est mathematiquement
        // singulier au strike. On renvoie 0 hors distribution.
        if (_isDigital)
        {
            return 0.0;
        }

        if (isCall)
        {
            return (S > K) ? 1.0 : 0.0;
        }

        return (S < K) ? -1.0 : 0.0;
    }

    // Cas limite : strike nul ou negatif.
    if (K <= 0.0)
    {
        // Le prix d'une digitale ne depend plus de S dans ce cas limite.
        if (_isDigital)
        {
            return 0.0;
        }

        return isCall ? 1.0 : 0.0;
    }

    // Cas limite : sous-jacent nul.
    if (S <= 0.0)
    {
        if (_isDigital)
        {
            return 0.0;
        }

        return isCall ? 0.0 : -1.0;
    }

    // Cas limite : volatilite nulle.
    if (_volatility <= 0.0)
    {
        // Pour une digitale, le delta est nul partout sauf au seuil.
        if (_isDigital)
        {
            return 0.0;
        }

        const double discountedStrike = discount(K, _interest_rate, T);

        if (isCall)
        {
            return (S > discountedStrike) ? 1.0 : 0.0;
        }

        return (S < discountedStrike) ? -1.0 : 0.0;
    }

    const double sqrtT = std::sqrt(T);
    const double sigmaSqrtT = _volatility * sqrtT;

    if (_isDigital)
    {
        // Delta digital Call :
        // e^{-rT} phi(d2) / (S sigma sqrt(T))
        //
        // Delta digital Put :
        // -e^{-rT} phi(d2) / (S sigma sqrt(T))
        const double d2 =
            (std::log(S / K) +
             (_interest_rate - 0.5 * _volatility * _volatility) * T) /
            sigmaSqrtT;

        const double commonDigitalDelta =
            discount(normalPdf(d2), _interest_rate, T) / (S * sigmaSqrtT);

        return isCall ? commonDigitalDelta : -commonDigitalDelta;
    }

    // Delta vanille classique.
    const double d1 =
        (std::log(S / K) +
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