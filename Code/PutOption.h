#ifndef PUT_OPTION_H
#define PUT_OPTION_H

#include "EuropeanVanillaOption.h"

// Classe concrete representant un Put europeen.
class PutOption : public EuropeanVanillaOption
{
public:
    // Constructeur : transmet expiry et strike a EuropeanVanillaOption.
    PutOption(double expiry, double strike);

    // Destructeur virtuel.
    virtual ~PutOption() = default;

    // Payoff du Put : h(z) = max(K - z, 0).
    double payoff(double z) const override;

    // Renvoie le type de l'option : put.
    optionType GetOptionType() const override;
};

#endif // PUT_OPTION_H