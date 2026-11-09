#ifndef EUROPEAN_DIGITAL_OPTION_H
#define EUROPEAN_DIGITAL_OPTION_H

#include "EuropeanVanillaOption.h"

// Declaration anticipee de BlackScholesPricer
// afin de pouvoir le declarer comme classe amie.
class BlackScholesPricer;

// Classe abstraite pour les options digitales europeennes.
// Elle est construite de maniere analogue a EuropeanVanillaOption.
class EuropeanDigitalOption : public Option
{
    // BlackScholesPricer doit pouvoir acceder au strike.
    friend class BlackScholesPricer;

private:
    // Strike K de l'option digitale.
    double _strike;

protected:
    // Accesseur protege pour les classes derivees.
    // Il n'est pas public afin de respecter la consigne
    // "No additional public members should be created".
    double strike() const;

public:
    // Constructeur : initialise expiry et strike.
    EuropeanDigitalOption(double expiry, double strike);

    // Destructeur virtuel.
    virtual ~EuropeanDigitalOption() = default;

    // Methode virtuelle pure renvoyant le type d'option.
    virtual optionType GetOptionType() const = 0;
};

#endif // EUROPEAN_DIGITAL_OPTION_H