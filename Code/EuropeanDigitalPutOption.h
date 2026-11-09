#ifndef EUROPEAN_DIGITAL_PUT_OPTION_H
#define EUROPEAN_DIGITAL_PUT_OPTION_H

#include "EuropeanDigitalOption.h"

// Option digitale Put europeenne.
// Payoff : h(z) = 1 si z <= K, 0 sinon.
class EuropeanDigitalPutOption : public EuropeanDigitalOption
{
public:
    // Constructeur : transmet expiry et strike a EuropeanDigitalOption.
    EuropeanDigitalPutOption(double expiry, double strike);

    // Destructeur virtuel.
    virtual ~EuropeanDigitalPutOption() = default;

    // Payoff du Digital Put.
    double payoff(double z) const override;

    // Renvoie le type de l'option : put.
    optionType GetOptionType() const override;
};

#endif // EUROPEAN_DIGITAL_PUT_OPTION_H