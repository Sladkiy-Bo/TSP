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

    int firstMin(int i);
    int secondMin(int i);
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