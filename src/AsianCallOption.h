#ifndef ASIAN_CALL_OPTION_H
#define ASIAN_CALL_OPTION_H

#include "AsianOption.h"

// Call asiatique.
// Le payoff classique est celui d'un Call : h(z) = max(z - K, 0).
class AsianCallOption : public AsianOption
{
private:
    // Strike K.
    double _strike;

public:
    // Constructeur : dates d'observation puis strike.
    AsianCallOption(const std::vector<double>& time_steps, double strike);

    // Destructeur virtuel.
    virtual ~AsianCallOption() = default;

    // Payoff classique du Call.
    double payoff(double z) const override;
};

#endif // ASIAN_CALL_OPTION_H