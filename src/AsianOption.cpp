#include "AsianOption.h"
#include <stdexcept>

namespace
{
    // Determination de la date d'expiration a partir des time steps.
    // Le dernier time step est utilise comme expiry.
    double extractExpiry(const std::vector<double>& time_steps)
    {
        if (time_steps.empty())
        {
            throw std::invalid_argument("AsianOption: time_steps must not be empty.");
        }

        if (time_steps.back() < 0.0)
        {
            throw std::invalid_argument("AsianOption: time_steps must be nonnegative.");
        }

        return time_steps.back();
    }

    // Verification supplementaire des time steps.
    void validateTimeSteps(const std::vector<double>& time_steps)
    {
        if (time_steps.empty())
        {
            throw std::invalid_argument("AsianOption: time_steps must not be empty.");
        }

        for (std::size_t i = 0; i < time_steps.size(); ++i)
        {
            if (time_steps[i] < 0.0)
            {
                throw std::invalid_argument("AsianOption: time_steps must be nonnegative.");
            }

            if (i > 0 && time_steps[i] < time_steps[i - 1])
            {
                throw std::invalid_argument("AsianOption: time_steps must be nondecreasing.");
            }
        }
    }
}

// Constructeur : initialise Option avec le dernier time step,
// puis stocke le vecteur complet des dates d'observation.
AsianOption::AsianOption(const std::vector<double>& time_steps)
    : Option(extractExpiry(time_steps)), _time_steps(time_steps)
{
    validateTimeSteps(_time_steps);
}

// Getter : renvoie les dates d'observation.
const std::vector<double>& AsianOption::getTimeSteps() const
{
    return _time_steps;
}

// Payoff asiatique :
// on calcule la moyenne arithmetique du chemin,
// puis on appelle payoff(average).
double AsianOption::payoffPath(const std::vector<double>& path) const
{
    if (path.empty())
    {
        throw std::invalid_argument("AsianOption: path must not be empty.");
    }

    double sum = 0.0;

    for (std::size_t i = 0; i < path.size(); ++i)
    {
        sum += path[i];
    }

    const double average = sum / static_cast<double>(path.size());

    return payoff(average);
}

// Une option asiatique renvoie true.
bool AsianOption::isAsianOption() const
{
    return true;
}