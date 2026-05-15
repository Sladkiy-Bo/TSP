#include "TSP.hpp"
#include <iostream>

int main() {
    TSP tsp;
    std::cout << "Введите матрицу смежности (первая строка – заголовки, затем строки данных):\n";
    if (tsp.readGraph()) {
        tsp.solve();
        tsp.printResult();
    } else {
        std::cout << "Ошибка ввода матрицы!" << std::endl;
    }
    return 0;
}