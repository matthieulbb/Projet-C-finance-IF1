#include "MT.h"

// Constructeur prive : initialise le generateur.
// Une graine fixe est utilisee afin d'avoir des resultats reproductibles.
// Si vous souhaitez des tirages differents a chaque execution,
// vous pouvez remplacer la graine fixe par std::random_device.
MT::MT()
    : _generator(42u)
{
}

// Instance unique du singleton.
// Le static local garantit une seule instance.
MT& MT::instance()
{
    static MT instance;
    return instance;
}

// Tirage uniforme sur [0, 1].
double MT::rand_unif()
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(instance()._generator);
}

// Tirage gaussien standard N(0, 1).
double MT::rand_norm()
{
    std::normal_distribution<double> distribution(0.0, 1.0);
    return distribution(instance()._generator);
}