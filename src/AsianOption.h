#ifndef ASIAN_OPTION_H
#define ASIAN_OPTION_H

#include "Option.h"
#include <vector>

// Classe abstraite pour les options asiatiques.
// Elle stocke les dates t_1, ..., t_m.
class AsianOption : public Option
{
private:
    // Dates d'observation du sous-jacent.
    std::vector<double> _time_steps;

public:
    // Constructeur : prend les dates t_1, ..., t_m.
    // La date d'expiration de l'option est le dernier time step.
    explicit AsianOption(const std::vector<double>& time_steps);

    // Destructeur virtuel.
    virtual ~AsianOption() = default;

    // Getter des dates d'observation.
    const std::vector<double>& getTimeSteps() const;

    // Payoff asiatique :
    // h(S_t1, ..., S_tm) = h( (1/m) sum_k S_tk )
    //
    // Conformement a la consigne, le mot-cle "virtual" n'est pas
    // repete ici. La fonction reste neanmoins virtuelle car elle
    // surcharge une fonction virtuelle de la classe Option.
    double payoffPath(const std::vector<double>& path) const override;

    // Surcharge de isAsianOption().
    bool isAsianOption() const override;
};

#endif // ASIAN_OPTION_H