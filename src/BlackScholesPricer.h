#ifndef BLACK_SCHOLES_PRICER_H
#define BLACK_SCHOLES_PRICER_H

#include "EuropeanVanillaOption.h"
#include "EuropeanDigitalOption.h"

// Classe de pricing Black-Scholes.
// Elle permet de pricer :
// - les options vanille europeennes ;
// - les options digitales europeennes.
class BlackScholesPricer
{
private:
    // Pointeur non-proprietaire vers l'option.
    const Option* _option;

    // Donnees extraites de l'option.
    double _strike;
    double _expiry;
    optionType _type;
    bool _isDigital;

    // Donnees de marche.
    double _asset_price;
    double _interest_rate;
    double _volatility;

public:
    // Constructeur pour option vanille europeenne.
    BlackScholesPricer(EuropeanVanillaOption* option,
                       double asset_price,
                       double interest_rate,
                       double volatility);

    // Constructeur surcharge pour option digitale europeenne.
    BlackScholesPricer(EuropeanDigitalOption* option,
                       double asset_price,
                       double interest_rate,
                       double volatility);

    // Renvoie le prix Black-Scholes.
    double operator()() const;

    // Renvoie le Delta.
    double delta() const;
};

#endif // BLACK_SCHOLES_PRICER_H