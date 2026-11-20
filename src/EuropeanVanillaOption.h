#ifndef EUROPEAN_VANILLA_OPTION_H
#define EUROPEAN_VANILLA_OPTION_H

#include "Option.h"

// Declaration anticipee de BlackScholesPricer
// afin de pouvoir le declarer comme classe amie.
class BlackScholesPricer;

// Type d'option demande par le sujet.
// Les valeurs sont en minuscules : call, put.
// Cette forme permet d'utiliser call ou optionType::call.
enum optionType
{
    call,
    put
};

// Classe abstraite pour les options europeennes vanille.
// Elle ajoute le strike K et le type d'option.
class EuropeanVanillaOption : public Option
{
    // BlackScholesPricer doit pouvoir acceder au strike.
    friend class BlackScholesPricer;

private:
    // Strike K de l'option europeenne.
    double _strike;

protected:
    // Accesseur protege pour les classes derivees.
    // Il n'est pas public afin de respecter la consigne
    // "No additional public members should be created".
    double strike() const;

public:
    // Constructeur : initialise expiry et strike.
    // Il appelle le constructeur de Option.
    EuropeanVanillaOption(double expiry, double strike);

    // Destructeur virtuel.
    virtual ~EuropeanVanillaOption() = default;

    // Methode virtuelle pure renvoyant le type d'option.
    virtual optionType GetOptionType() const = 0;
};

#endif // EUROPEAN_VANILLA_OPTION_H