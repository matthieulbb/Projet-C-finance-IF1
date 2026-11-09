#ifndef CRR_PRICER_H
#define CRR_PRICER_H

#include "Option.h"
#include "BinaryTree.h"

// Classe de pricing par le modele Cox-Ross-Rubinstein.
// Elle utilise un arbre binomial pour stocker les prix H(n, i).
class CRRPricer
{
private:
    // Pointeur non-proprietaire vers l'option.
    const Option* _option;

    // Donnees du modele.
    int _depth;
    double _asset_price;
    double _up;
    double _down;
    double _interest_rate;

    // Arbre contenant les prix H(n, i).
    BinaryTree<double> _tree;

    // Indique si l'arbre a deja ete calcule.
    bool _computed;

    // Probabilite risque-neutre q.
    double riskNeutralProbability() const;

    // Prix du sous-jacent au noeud (n, i).
    double underlyingPrice(int n, int i) const;

    // Formule fermee CRR.
    double closedFormPrice() const;

public:
    // Constructeur demande par le sujet.
    CRRPricer(Option* option,
              int depth,
              double asset_price,
              double up,
              double down,
              double interest_rate);

    // Calcule l'arbre par induction retrograde.
    void compute();

    // Renvoie H(n, i).
    // Si l'arbre n'a pas encore ete calcule, compute() est appele.
    double get(int n, int i);

    // Renvoie le prix de l'option.
    // Si closed_form == false : procedure CRR classique.
    // Si closed_form == true : formule fermee CRR.
    double operator()(bool closed_form = false);
};

#endif // CRR_PRICER_H