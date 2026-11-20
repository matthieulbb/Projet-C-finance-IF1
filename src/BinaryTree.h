#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// Classe template BinaryTree<T>.
// Elle represente l'arbre binomial utilise dans le modele CRR.
// Conformement au cours, l'implementation d'une classe template
// doit etre placee dans le header.
template <typename T>
class BinaryTree
{
private:
    // Profondeur de l'arbre : N.
    int _depth;

    // Arbre stocke sous forme de vecteur de vecteurs.
    // Le niveau n contient n + 1 noeuds.
    std::vector<std::vector<T>> _tree;

    // Verification des indices d'un noeud.
    void checkIndices(int n, int i) const
    {
        if (n < 0 || n > _depth || i < 0 || i > n)
        {
            throw std::out_of_range("BinaryTree: invalid node indices.");
        }
    }

public:
    // Constructeur par defaut : arbre de profondeur 0.
    BinaryTree(int depth = 0)
        : _depth(0), _tree()
    {
        setDepth(depth);
    }

    // Setter de profondeur.
    // Realloue proprement l'arbre.
    void setDepth(int depth)
    {
        if (depth < 0)
        {
            throw std::invalid_argument("BinaryTree: depth must be nonnegative.");
        }

        _depth = depth;

        // Desallocation propre : clear() detruit les vecteurs internes.
        _tree.clear();

        // Allocation du nombre de niveaux.
        _tree.resize(_depth + 1);

        // Allocation du nombre de noeuds par niveau.
        for (int n = 0; n <= _depth; ++n)
        {
            _tree[n].resize(n + 1);
        }
    }

    // Setter d'un noeud.
    void setNode(int n, int i, T value)
    {
        checkIndices(n, i);
        _tree[n][i] = value;
    }

    // Getter d'un noeud.
    T getNode(int n, int i) const
    {
        checkIndices(n, i);
        return _tree[n][i];
    }

    // Affichage de l'arbre.
    // Les valeurs sont centrees en fonction de la largeur maximale.
    void display() const
    {
        // Conversion des noeuds en chaines de caracteres.
        std::vector<std::vector<std::string>> nodes(_depth + 1);
        int maxWidth = 1;

        for (int n = 0; n <= _depth; ++n)
        {
            nodes[n].resize(n + 1);

            for (int i = 0; i <= n; ++i)
            {
                std::ostringstream oss;
                oss << _tree[n][i];
                nodes[n][i] = oss.str();

                const int currentWidth = static_cast<int>(nodes[n][i].size());
                if (currentWidth > maxWidth)
                {
                    maxWidth = currentWidth;
                }
            }
        }

        // Espacement entre deux noeuds.
        const int spacing = maxWidth + 2;

        // Largeur totale de la derniere ligne.
        const int bottomWidth = (_depth + 1) * maxWidth + _depth * spacing;

        // Affichage ligne par ligne.
        for (int n = 0; n <= _depth; ++n)
        {
            const int rowWidth = (n + 1) * maxWidth + n * spacing;
            const int leading = (bottomWidth - rowWidth) / 2;

            std::cout << std::string(leading, ' ');

            for (int i = 0; i <= n; ++i)
            {
                const std::string& text = nodes[n][i];

                // Centrage du texte dans un champ de largeur maxWidth.
                const int padding = maxWidth - static_cast<int>(text.size());
                const int left = padding / 2;
                const int right = padding - left;

                std::cout << std::string(left, ' ')
                          << text
                          << std::string(right, ' ');

                if (i != n)
                {
                    std::cout << std::string(spacing, ' ');
                }
            }

            std::cout << '\n';
        }
    }
};

#endif // BINARY_TREE_H