#ifndef OPTION_H
#define OPTION_H

// Classe abstraite de base pour toutes les options.
// Elle definit notamment la date d'expiration et la fonction de payoff.
class Option
{
protected:
    // Conformement a la consigne du TD, _expiry est protected
    // afin de permettre l'acces aux classes derivees.
    double _expiry;

public:
    // Constructeur de la classe Option.
    // Il initialise la date d'expiration.
    explicit Option(double expiry);

    // Destructeur virtuel pour assurer une destruction correcte
    // des objets derives via un pointeur de type Option*.
    virtual ~Option() = default;

    // Accesseur en lecture seule sur la date d'expiration.
    double getExpiry() const;

    // Fonction de payoff h(z).
    // Elle est virtuelle pure : les classes derivees devront l'implementer.
    virtual double payoff(double z) const = 0;
};

#endif // OPTION_H