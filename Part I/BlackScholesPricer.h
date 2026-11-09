#ifndef BLACK_SCHOLES_PRICER_H
#define BLACK_SCHOLES_PRICER_H

#include "EuropeanVanillaOption.h"

// Classe de pricing Black-Scholes pour les options europeennes vanille.
// Elle stocke un pointeur non-proprietaire vers une option.
class BlackScholesPricer
{
private:
    // Pointeur vers l'option a pricer.
    // Le pricer ne possede pas l'option : il ne doit donc pas la detruire.
    const EuropeanVanillaOption* _option;

    // Donnees de marche.
    double _asset_price;
    double _interest_rate;
    double _volatility;

public:
    // Constructeur demande par le sujet.
    BlackScholesPricer(EuropeanVanillaOption* option,
                       double asset_price,
                       double interest_rate,
                       double volatility);

    // Operateur fonctionnel : renvoie le prix Black-Scholes.
    double operator()() const;

    // Renvoie le Delta de l'option.
    double delta() const;
};

#endif // BLACK_SCHOLES_PRICER_H