#ifndef ASIAN_PUT_OPTION_H
#define ASIAN_PUT_OPTION_H

#include "AsianOption.h"

// Put asiatique.
// Le payoff classique est celui d'un Put : h(z) = max(K - z, 0).
class AsianPutOption : public AsianOption
{
private:
    // Strike K.
    double _strike;

public:
    // Constructeur : dates d'observation puis strike.
    AsianPutOption(const std::vector<double>& time_steps, double strike);

    // Destructeur virtuel.
    virtual ~AsianPutOption() = default;

    // Payoff classique du Put.
    double payoff(double z) const override;
};

#endif // ASIAN_PUT_OPTION_H