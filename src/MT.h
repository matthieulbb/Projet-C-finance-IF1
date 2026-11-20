#ifndef MT_H
#define MT_H

#include <random>

// Classe singleton encapsulant un generateur std::mt19937.
// Une seule instance de generateur est utilisee dans tout le programme.
class MT
{
private:
    // Generateur pseudo-aleatoire.
    std::mt19937 _generator;

    // Constructeur prive : empeche l'instanciation directe.
    MT();

    // Destructeur prive : le cycle de vie est gere par le singleton.
    ~MT() = default;

    // Copie et affectation supprimees.
    MT(const MT&) = delete;
    MT& operator=(const MT&) = delete;

    // Acces a l'instance unique.
    static MT& instance();

public:
    // Tirage uniforme sur [0, 1].
    static double rand_unif();

    // Tirage gaussien standard N(0, 1).
    static double rand_norm();
};

#endif // MT_H