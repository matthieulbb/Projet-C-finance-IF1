#ifndef EUROPEAN_DIGITAL_CALL_OPTION_H
#define EUROPEAN_DIGITAL_CALL_OPTION_H

#include "EuropeanDigitalOption.h"

// Option digitale Call europeenne.
// Payoff : h(z) = 1 si z >= K, 0 sinon.
class EuropeanDigitalCallOption : public EuropeanDigitalOption
{
public:
    // Constructeur : transmet expiry et strike a EuropeanDigitalOption.
    EuropeanDigitalCallOption(double expiry, double strike);

    // Destructeur virtuel.
    virtual ~EuropeanDigitalCallOption() = default;

    // Payoff du Digital Call.
    double payoff(double z) const override;

    // Renvoie le type de l'option : call.
    optionType GetOptionType() const override;
};

#endif // EUROPEAN_DIGITAL_CALL_OPTION_H