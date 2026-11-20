#include "BinaryTree.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

void require(bool condition)
{
    if (!condition)
    {
        throw std::runtime_error("Echec d'un test BinaryTree");
    }
}

template <typename Exception, typename Function>
void expectException(Function action)
{
    try
    {
        action();
    }
    catch (const Exception&)
    {
        return;
    }
    throw std::runtime_error("Exception attendue non lancee");
}

int main()
{
    BinaryTree<double> prices;
    require(prices.getNode(0, 0) == 0.0);
    prices.setDepth(3);
    prices.setNode(0, 0, 12.5);
    prices.setNode(3, 3, 7.25);
    require(prices.getNode(0, 0) == 12.5);
    require(prices.getNode(3, 3) == 7.25);

    // Agrandir conserve les valeurs et initialise les nouveaux noeuds.
    prices.setDepth(5);
    require(prices.getNode(3, 3) == 7.25);
    require(prices.getNode(5, 5) == 0.0);

    // Reduire supprime les lignes devenues inutiles.
    prices.setDepth(1);
    require(prices.getNode(0, 0) == 12.5);
    expectException<std::out_of_range>([&] { prices.getNode(2, 0); });
    prices.setDepth(3);
    require(prices.getNode(3, 3) == 0.0);
    expectException<std::invalid_argument>([&] { prices.setDepth(-1); });
    require(prices.getNode(0, 0) == 12.5);

    expectException<std::out_of_range>([&] { prices.getNode(-1, 0); });
    expectException<std::out_of_range>([&] { prices.getNode(1, -1); });
    expectException<std::out_of_range>([&] { prices.getNode(1, 2); });
    expectException<std::out_of_range>([&] { prices.setNode(4, 0, 1.0); });
    expectException<std::out_of_range>([&] { prices.setNode(1, 2, 1.0); });
    expectException<std::out_of_range>([&] { prices.setNode(-1, 0, 1.0); });
    expectException<std::out_of_range>([&] { prices.setNode(1, -1, 1.0); });

    prices.setDepth(0);
    require(prices.getNode(0, 0) == 12.5);
    expectException<std::out_of_range>([&] { prices.getNode(1, 0); });

    BinaryTree<bool> exercise;
    exercise.setDepth(2);
    require(!exercise.getNode(2, 2));
    exercise.setNode(2, 2, true);
    require(exercise.getNode(2, 2));
    exercise.setDepth(3);
    require(exercise.getNode(2, 2));
    require(!exercise.getNode(3, 3));

    BinaryTree<int> example;
    example.setDepth(2);
    int value = 1;
    for (int n = 0; n <= 2; ++n)
    {
        for (int i = 0; i <= n; ++i)
        {
            example.setNode(n, i, value++);
        }
    }

    std::ostringstream output;
    std::streambuf* previous = std::cout.rdbuf(output.rdbuf());
    example.display();
    std::cout.rdbuf(previous);
    require(output.str() == "1\n2 3\n4 5 6\n");

    std::cout << "Tests BinaryTree OK\n";
}
