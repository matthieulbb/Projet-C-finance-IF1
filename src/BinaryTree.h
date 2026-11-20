#pragma once

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T>
class BinaryTree
{
    private:
        int _depth = 0;
        std::vector<std::vector<T>> _tree{std::vector<T>(1)};

    public:
        void setDepth(int depth);
        void setNode(int n, int i, T value);
        T getNode(int n, int i) const;
        void display() const;
};

template <typename T>
void BinaryTree<T>::setDepth(int depth)
{
    if (depth < 0)
    {
        throw std::invalid_argument("La profondeur doit etre positive ou nulle");
    }

    // Il y a depth + 1 lignes, et n + 1 noeuds sur la ligne n.
    _tree.resize(static_cast<std::size_t>(depth) + 1);
    for (std::size_t n = 0; n < _tree.size(); ++n)
    {
        _tree[n].resize(n + 1);
    }
    _depth = depth;
}

template <typename T>
void BinaryTree<T>::setNode(int n, int i, T value)
{
    // at() lance std::out_of_range si un indice est invalide.
    _tree.at(static_cast<std::size_t>(n)).at(static_cast<std::size_t>(i)) = value;
}

template <typename T>
T BinaryTree<T>::getNode(int n, int i) const
{
    // Retourner une valeur permet aussi d'utiliser BinaryTree<bool>.
    return _tree.at(static_cast<std::size_t>(n)).at(static_cast<std::size_t>(i));
}

template <typename T>
void BinaryTree<T>::display() const
{
    for (std::size_t n = 0; n <= static_cast<std::size_t>(_depth); ++n)
    {
        for (std::size_t i = 0; i < _tree[n].size(); ++i)
        {
            if (i > 0)
            {
                std::cout << ' ';
            }
            std::cout << _tree[n][i];
        }
        std::cout << '\n';
    }
}
