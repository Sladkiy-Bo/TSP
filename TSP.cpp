#include "TSP.hpp"
#include <iostream>
#include <climits>
#include <cstring>
#include <sstream>

TSP::TSP() : n(0), final_res(INT_MAX) {}

bool TSP::readGraph() {
    std::string line;
    
    while (std::getline(std::cin, line) && line.empty()) {}
    
    std::stringstream ss(line);
    std::vector<int> headers;
    int val;
    while (ss >> val) {
        headers.push_back(val);
    }
    n = headers.size();
    
    if (n == 0) return false;
    
    graph.resize(n, std::vector<int>(n, 0));
    
    for (int i = 0; i < n; i++) {
        while (std::getline(std::cin, line) && line.empty()) {}
        
        std::stringstream row_ss(line);
        int row_header;
        row_ss >> row_header;
        
        for (int j = 0; j < n; j++) {
            if (!(row_ss >> graph[i][j])) {
                return false;
            }
        }
    }
    
    visited.resize(n, false);
    final_path.resize(n + 1);
    return true;
}

int TSP::greedyTour() {
    vector<bool> used(n, false);
    int cur = 0, total = 0;
    used[0] = true;
    for (int i = 0; i < n-1; ++i) {
        int next = -1, minD = INT_MAX;
        for (int j = 0; j < n; ++j)
            if (!used[j] && graph[cur][j] < minD) {
                next = j;
                minD = graph[cur][j];
            }
        total += minD;
        cur = next;
        used[cur] = true;
    }
    total += graph[cur][0];
    return total;
}

int TSP::firstMin(int i) {
    int min = INT_MAX;
    for (int k = 0; k < n; k++)
        if (graph[i][k] < min && i != k)
            min = graph[i][k];
    return min;
}

int TSP::secondMin(int i) {
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < n; j++) {
        if (i == j) continue;
        if (graph[i][j] <= first) {
            second = first;
            first = graph[i][j];
        } else if (graph[i][j] <= second && graph[i][j] != first) {
            second = graph[i][j];
        }
    }
    return second;
}

void TSP::copyToFinal(int curr_path[]) {
    for (int i = 0; i < n; i++)
        final_path[i] = curr_path[i];
    final_path[n] = curr_path[0];
}

void TSP::TSPRecursive(int curr_bound, int curr_weight, int level, int curr_path[]) {
    if (level == n) {
        int last = curr_path[level - 1];
        if (graph[last][0] != 0) {
            int curr_res = curr_weight + graph[last][0];
            if (curr_res < final_res) {
                copyToFinal(curr_path);
                final_res = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        int prev = curr_path[level - 1];
        if (graph[prev][i] != 0 && !visited[i]) {
            int temp = curr_bound;
            curr_weight += graph[prev][i];

            if (level == 1)
                curr_bound -= (firstMin(prev) + firstMin(i)) / 2;
            else
                curr_bound -= (secondMin(prev) + firstMin(i)) / 2;

            if (curr_bound + curr_weight < final_res) {
                curr_path[level] = i;
                visited[i] = true;
                TSPRecursive(curr_bound, curr_weight, level + 1, curr_path);
            }

            curr_weight -= graph[prev][i];
            curr_bound = temp;

            std::fill(visited.begin(), visited.end(), false);
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = true;
        }
    }
}

void TSP::solve() {
    if (n == 0) return;
    
    int* curr_path = new int[n + 1];
    std::memset(curr_path, -1, (n + 1) * sizeof(int));
    int curr_bound = 0;
    final_res = greedyTour();

    for (int i = 0; i < n; i++)
        curr_bound += (firstMin(i) + secondMin(i));

    curr_bound = (curr_bound & 1) ? curr_bound / 2 + 1 : curr_bound / 2;

    visited[0] = true;
    curr_path[0] = 0;

    TSPRecursive(curr_bound, 0, 1, curr_path);
    
    delete[] curr_path;
}

void TSP::printResult() const {
    std::cout << "Минимальная стоимость: " << final_res << std::endl;
    std::cout << "Оптимальный маршрут: ";
    for (int i = 0; i <= n; i++)
        std::cout << final_path[i] << " ";
    std::cout << std::endl;
}

int TSP::getResult() const {
    return final_res;
}