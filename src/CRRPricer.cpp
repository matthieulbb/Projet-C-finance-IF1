#include "CRRPricer.h"
#include <cmath>
#include <stdexcept>

// Constructeur : initialise le pricer et verifie l'absence d'arbitrage.
CRRPricer::CRRPricer(Option* option,
                     int depth,
                     double asset_price,
                     double up,
                     double down,
                     double interest_rate)
    : _option(option),
      _depth(depth),
      _asset_price(asset_price),
      _up(up),
      _down(down),
      _interest_rate(interest_rate),
      _tree(),
      _computed(false)
{
    if (_option == nullptr)
    {
        throw std::invalid_argument("CRRPricer: option must not be null.");
    }

    if (_depth < 0)
    {
        throw std::invalid_argument("CRRPricer: depth must be nonnegative.");
    }

    if (_asset_price <= 0.0)
    {
        throw std::invalid_argument("CRRPricer: asset_price must be positive.");
    }

    if (_down <= -1.0)
    {
        throw std::invalid_argument("CRRPricer: down must be greater than -1.");
    }

    if (_up <= _down)
    {
        throw std::invalid_argument("CRRPricer: up must be greater than down.");
    }

    if (_interest_rate <= -1.0)
    {
        throw std::invalid_argument("CRRPricer: interest_rate must be greater than -1.");
    }

    // Condition d'absence d'arbitrage : D < R < U.
    if (!(_down < _interest_rate && _interest_rate < _up))
    {
        throw std::invalid_argument("CRRPricer: arbitrage condition violated (D < R < U).");
    }

    // Creation de la structure d'arbre de profondeur depth.
    _tree.setDepth(_depth);
}

// Probabilite risque-neutre :
// q = (R - D) / (U - D)
double CRRPricer::riskNeutralProbability() const
{
    return (_interest_rate - _down) / (_up - _down);
}

// Prix du sous-jacent au noeud (n, i) :
// S(n, i) = S0 * (1 + U)^i * (1 + D)^(n - i)
double CRRPricer::underlyingPrice(int n, int i) const
{
    return _asset_price *
           std::pow(1.0 + _up, i) *
           std::pow(1.0 + _down, n - i);
}

// Procedure CRR par induction retrograde.
void CRRPricer::compute()
{
    const double q = riskNeutralProbability();
    const double grossRiskFreeRate = 1.0 + _interest_rate;

    // Etape terminale : H(N, i) = h(S(N, i)).
    for (int i = 0; i <= _depth; ++i)
    {
        const double stockPrice = underlyingPrice(_depth, i);
        _tree.setNode(_depth, i, _option->payoff(stockPrice));
    }

    // Induction retrograde :
    // H(n, i) = [q H(n + 1, i + 1) + (1 - q) H(n + 1, i)] / (1 + R)
    for (int n = _depth - 1; n >= 0; --n)
    {
        for (int i = 0; i <= n; ++i)
        {
            const double continuationValue =
                q * _tree.getNode(n + 1, i + 1) +
                (1.0 - q) * _tree.getNode(n + 1, i);

            _tree.setNode(n, i, continuationValue / grossRiskFreeRate);
        }
    }

    _computed = true;
}

// Getter : renvoie H(n, i).
// Si necessaire, l'arbre est calcule avant lecture.
double CRRPricer::get(int n, int i)
{
    if (!_computed)
    {
        compute();
    }

    return _tree.getNode(n, i);
}

// Operateur de pricing.
double CRRPricer::operator()(bool closed_form)
{
    if (closed_form)
    {
        return closedFormPrice();
    }

    if (!_computed)
    {
        compute();
    }

    return _tree.getNode(0, 0);
}

// Formule fermee CRR :
// H(0, 0) = 1 / (1 + R)^N *
//           sum_{i=0}^N C(N, i) q^i (1 - q)^(N - i) h(S(N, i))
double CRRPricer::closedFormPrice() const
{
    const int N = _depth;

    if (N == 0)
    {
        return _option->payoff(_asset_price);
    }

    const double q = riskNeutralProbability();
    const double discountFactor = std::pow(1.0 + _interest_rate, -N);

    // Cas limites theoriques, normalement exclus par l'absence d'arbitrage.
    if (q <= 0.0)
    {
        return discountFactor * _option->payoff(underlyingPrice(N, 0));
    }

    if (q >= 1.0)
    {
        return discountFactor * _option->payoff(underlyingPrice(N, N));
    }

    double sum = 0.0;

    // Probabilite binomiale pour i = 0 :
    // C(N, 0) q^0 (1 - q)^N = (1 - q)^N
    double probability = std::pow(1.0 - q, N);

    for (int i = 0; i <= N; ++i)
    {
        const double stockPrice = underlyingPrice(N, i);
        sum += probability * _option->payoff(stockPrice);

        // Passage de la probabilite i a la probabilite i + 1 :
        // P(i + 1) = P(i) * (N - i) / (i + 1) * q / (1 - q)
        if (i < N)
        {
            probability *= static_cast<double>(N - i) /
                           static_cast<double>(i + 1) *
                           q /
                           (1.0 - q);
        }
    }

    return discountFactor * sum;
}