#ifndef BLACK_SCHOLES_MC_PRICER_H
#define BLACK_SCHOLES_MC_PRICER_H

#include "Option.h"
#include <vector>

// Classe de pricing Monte Carlo sous le modele Black-Scholes.
// Elle ne stocke aucun chemin : seuls les statistiques agrégées sont conservées.
class BlackScholesMCPricer
{
private:
    // Pointeur non-proprietaire vers l'option.
    const Option* _option;

    // Donnees de marche.
    double _initial_price;
    double _interest_rate;
    double _volatility;

    // Nombre de chemins deja generes pour estimer le prix.
    long long _nb_paths;

    // Moyenne courante des payoffs actualises.
    double _mean;

    // Terme utilise pour la variance empirique
    // (methode de Welford).
    double _m2;

    // Dates de simulation.
    // Pour une option europeenne : {T}.
    // Pour une option asiatique : {t_1, ..., t_m}.
    std::vector<double> _time_steps;

    // Mise a jour incrementale de la moyenne et de la variance.
    void updateEstimate(double discounted_payoff);

public:
    // Constructeur demande par le sujet.
    BlackScholesMCPricer(Option* option,
                         double initial_price,
                         double interest_rate,
                         double volatility);

    // Getter : nombre de chemins deja generes.
    long long getNbPaths() const;

    // Genere nb_paths nouveaux chemins et met a jour l'estimation.
    void generate(int nb_paths);

    // Renvoie l'estimation courante du prix.
    // Lance une exception si aucun chemin n'a ete genere.
    double operator()() const;

    // Renvoie l'intervalle de confiance a 95% :
    // [lower_bound, upper_bound]
    std::vector<double> confidenceInterval() const;
};

#endif // BLACK_SCHOLES_MC_PRICER_H