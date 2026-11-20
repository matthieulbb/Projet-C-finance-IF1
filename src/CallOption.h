#ifndef CALL_OPTION_H
#define CALL_OPTION_H

#include "EuropeanVanillaOption.h"

// Classe concrete representant un Call europeen.
class CallOption : public EuropeanVanillaOption
{
public:
    // Constructeur : transmet expiry et strike a EuropeanVanillaOption.
    CallOption(double expiry, double strike);

    // Destructeur virtuel.
    virtual ~CallOption() = default;

    // Payoff du Call : h(z) = max(z - K, 0).
    double payoff(double z) const override;

    // Renvoie le type de l'option : call.
    optionType GetOptionType() const override;
};

#endif // CALL_OPTION_H