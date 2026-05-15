#ifndef TSP_HPP
#define TSP_HPP

#include <vector>
#include <string>

class TSP {
private:
    int n;
    int final_res;
    std::vector<int> final_path;
    std::vector<bool> visited;
    std::vector<std::vector<int>> graph;
    
    std::vector<int> min1, min2;   // предвычисленные минимумы

    void computeMins();
    int greedyTour();
    void TSPRecursive(int curr_bound, int curr_weight, int level, int curr_path[]);
    void copyToFinal(int curr_path[]);

public:
    TSP();
    bool readGraph();
    void solve();
    void printResult() const;
    int getResult() const;
};

#endif