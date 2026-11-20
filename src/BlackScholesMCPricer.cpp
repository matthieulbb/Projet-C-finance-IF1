#include "BlackScholesMCPricer.h"
#include "AsianOption.h"
#include "MT.h"
#include <cmath>
#include <stdexcept>

// Constructeur : initialise le pricer Monte Carlo.
BlackScholesMCPricer::BlackScholesMCPricer(Option* option,
                                           double initial_price,
                                           double interest_rate,
                                           double volatility)
    : _option(option),
      _initial_price(initial_price),
      _interest_rate(interest_rate),
      _volatility(volatility),
      _nb_paths(0),
      _mean(0.0),
      _m2(0.0),
      _time_steps()
{
    if (_option == nullptr)
    {
        throw std::invalid_argument("BlackScholesMCPricer: option must not be null.");
    }

    if (_initial_price < 0.0)
    {
        throw std::invalid_argument("BlackScholesMCPricer: initial_price must be nonnegative.");
    }

    if (_volatility < 0.0)
    {
        throw std::invalid_argument("BlackScholesMCPricer: volatility must be nonnegative.");
    }

    // Determination des dates de simulation.
    // Pour une option non asiatique, il n'y a qu'une seule date : T.
    if (_option->isAsianOption())
    {
        // L'option est asiatique : on recupere les dates t_1, ..., t_m.
        const AsianOption* asian_option = static_cast<const AsianOption*>(_option);
        _time_steps = asian_option->getTimeSteps();
    }
    else
    {
        // Option europeenne : m = 1.
        _time_steps.push_back(_option->getExpiry());
    }

    if (_time_steps.empty())
    {
        throw std::invalid_argument("BlackScholesMCPricer: time steps must not be empty.");
    }

    // Verification des dates de simulation.
    for (std::size_t i = 0; i < _time_steps.size(); ++i)
    {
        if (_time_steps[i] < 0.0)
        {
            throw std::invalid_argument("BlackScholesMCPricer: time steps must be nonnegative.");
        }

        if (i > 0 && _time_steps[i] < _time_steps[i - 1])
        {
            throw std::invalid_argument("BlackScholesMCPricer: time steps must be nondecreasing.");
        }
    }
}

// Getter : nombre de chemins deja generes.
long long BlackScholesMCPricer::getNbPaths() const
{
    return _nb_paths;
}

// Mise a jour incrementale de la moyenne et de la variance.
// Cette methode correspond a l'idee de mise a jour en ligne
// vue dans les exercices de TD.
void BlackScholesMCPricer::updateEstimate(double discounted_payoff)
{
    ++_nb_paths;

    const double delta = discounted_payoff - _mean;
    _mean += delta / static_cast<double>(_nb_paths);
    const double delta2 = discounted_payoff - _mean;
    _m2 += delta * delta2;
}

// Generation de nb_paths chemins Black-Scholes.
// Aucun chemin n'est stocke dans l'objet : le vecteur path
// est seulement une variable locale temporaire.
void BlackScholesMCPricer::generate(int nb_paths)
{
    if (nb_paths < 0)
    {
        throw std::invalid_argument("BlackScholesMCPricer: nb_paths must be nonnegative.");
    }

    if (nb_paths == 0)
    {
        return;
    }

    const double drift = _interest_rate - 0.5 * _volatility * _volatility;
    const double maturity = _time_steps.back();
    const double discount_factor = std::exp(-_interest_rate * maturity);

    // Un seul chemin est garde en memoire a la fois.
    std::vector<double> path(_time_steps.size());

    for (int p = 0; p < nb_paths; ++p)
    {
        double previous_time = 0.0;
        double current_price = _initial_price;

        // Simulation du chemin S_t1, ..., S_tm.
        for (std::size_t k = 0; k < _time_steps.size(); ++k)
        {
            const double dt = _time_steps[k] - previous_time;

            if (dt > 0.0)
            {
                const double z = MT::rand_norm();

                // Formule de simulation Black-Scholes :
                // S_tk = S_t{k-1} * exp((r - sigma^2 / 2) dt + sigma sqrt(dt) Zk)
                current_price *= std::exp(drift * dt + _volatility * std::sqrt(dt) * z);
            }

            path[k] = current_price;
            previous_time = _time_steps[k];
        }

        // Payoff sur le chemin simule.
        const double payoff_value = _option->payoffPath(path);

        // Actualisation du payoff.
        const double discounted_payoff = discount_factor * payoff_value;

        // Mise a jour de l'estimation.
        updateEstimate(discounted_payoff);
    }
}

// Operateur : renvoie l'estimation courante.
double BlackScholesMCPricer::operator()() const
{
    if (_nb_paths == 0)
    {
        throw std::logic_error("BlackScholesMCPricer: price is undefined because no path has been generated.");
    }

    return _mean;
}

// Intervalle de confiance a 95%.
// On utilise la variance empirique des payoffs actualises.
std::vector<double> BlackScholesMCPricer::confidenceInterval() const
{
    if (_nb_paths == 0)
    {
        throw std::logic_error("BlackScholesMCPricer: confidence interval is undefined because no path has been generated.");
    }

    const double estimate = _mean;

    // Avec un seul chemin, la variance empirique n'est pas estimable.
    // On renvoie un intervalle reduit a l'estimation.
    if (_nb_paths == 1)
    {
        return { estimate, estimate };
    }

    double variance = _m2 / static_cast<double>(_nb_paths - 1);

    // Protection contre les erreurs numeriques pouvant donner
    // une variance tres legerement negative.
    if (variance < 0.0)
    {
        variance = 0.0;
    }

    const double standard_error = std::sqrt(variance / static_cast<double>(_nb_paths));
    const double half_width = 1.96 * standard_error;

    return { estimate - half_width, estimate + half_width };
}