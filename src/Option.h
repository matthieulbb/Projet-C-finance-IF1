#ifndef OPTION_H
#define OPTION_H

#include <vector>

// Classe abstraite de base pour toutes les options.
// Elle contient :
// - la date d'expiration ;
// - le payoff classique h(z) ;
// - le payoff sur un chemin complet ;
// - une methode virtuelle pour identifier les options asiatiques.
class Option
{
protected:
    // Conformement a la consigne du TD, _expiry est protected.
    double _expiry;

public:
    // Constructeur : initialise la date d'expiration.
    explicit Option(double expiry);

    // Destructeur virtuel pour la destruction polymorphique.
    virtual ~Option() = default;

    // Accesseur sur la date d'expiration.
    double getExpiry() const;

    // Payoff classique h(z).
    virtual double payoff(double z) const = 0;

    // Payoff dependant du chemin.
    // La version non surchargee renvoie h(S_tm), c'est-a-dire
    // le payoff sur la derniere valeur du chemin.
    virtual double payoffPath(const std::vector<double>& path) const;

    // Renvoie false pour une option non asiatique.
    // Cette methode est surchargee dans AsianOption.
    virtual bool isAsianOption() const;
};

#endif // OPTION_H