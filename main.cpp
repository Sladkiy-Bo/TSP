#include "TSP.hpp"
#include <iostream>

int main() {
    TSP tsp;
    
    std::cout << "Введите матрицу смежности (сначала строка заголовков, потом строки данных):" << std::endl;
    
    if (tsp.readGraph()) {
        tsp.solve();
        tsp.printResult();
    } else {
        std::cout << "Ошибка ввода матрицы!" << std::endl;
    }
    
    return 0;
}