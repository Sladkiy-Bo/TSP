#include "TSP.hpp"
#include <iostream>
#include <climits>
#include <sstream>
#include <algorithm>

TSP::TSP() : n(0), final_res(INT_MAX) {}

bool TSP::readGraph() {
    std::string line;
    while (std::getline(std::cin, line) && line.empty()) {}
    std::stringstream ss(line);
    std::vector<int> headers;
    int val;
    while (ss >> val) headers.push_back(val);
    n = headers.size();
    if (n == 0) return false;

    std::vector<int> numbers;
    int num;
    while (std::cin >> num) numbers.push_back(num);
    if ((int)numbers.size() != n * (n + 1)) {
        std::cerr << "Ожидалось " << n*(n+1) << " чисел, получено " << numbers.size() << std::endl;
        return false;
    }

    graph.assign(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        int base = i * (n + 1);
        for (int j = 0; j < n; ++j)
            graph[i][j] = numbers[base + 1 + j];
    }

    visited.assign(n, false);
    final_path.resize(n + 1);
    min1.resize(n);
    min2.resize(n);
    return true;
}

void TSP::computeMins() {
    for (int i = 0; i < n; ++i) {
        int first = INT_MAX, second = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            int w = graph[i][j];
            if (w < first) {
                second = first;
                first = w;
            } else if (w < second) {
                second = w;
            }
        }
        min1[i] = first;
        min2[i] = second;
    }
}

int TSP::greedyTour() {
    std::vector<bool> used(n, false);
    used[0] = true;
    int cur = 0, total = 0;
    for (int step = 1; step < n; ++step) {
        int next = -1, minD = INT_MAX;
        for (int j = 0; j < n; ++j) {
            if (!used[j] && graph[cur][j] < minD) {
                next = j;
                minD = graph[cur][j];
            }
        }
        total += minD;
        cur = next;
        used[cur] = true;
    }
    total += graph[cur][0];
    return total;
}

void TSP::copyToFinal(int curr_path[]) {
    for (int i = 0; i < n; ++i)
        final_path[i] = curr_path[i];
    final_path[n] = curr_path[0];
}

void TSP::TSPRecursive(int curr_bound, int curr_weight, int level, int curr_path[]) {
    if (level == n) {
        int last = curr_path[level - 1];
        if (graph[last][0] != 0) {
            int total = curr_weight + graph[last][0];
            if (total < final_res) {
                copyToFinal(curr_path);
                final_res = total;
            }
        }
        return;
    }

    int prev = curr_path[level - 1];
    for (int i = 0; i < n; ++i) {
        if (graph[prev][i] == 0 || visited[i]) continue;

        int temp = curr_bound;
        curr_weight += graph[prev][i];

        if (level == 1)
            curr_bound -= (min1[prev] + min1[i]) / 2;
        else
            curr_bound -= (min2[prev] + min1[i]) / 2;

        if (curr_bound + curr_weight < final_res) {
            curr_path[level] = i;
            visited[i] = true;
            TSPRecursive(curr_bound, curr_weight, level + 1, curr_path);
            visited[i] = false;    // ← просто снимаем пометку!
        }

        curr_weight -= graph[prev][i];
        curr_bound = temp;
    }
}

void TSP::solve() {
    if (n == 0) return;

    computeMins();
    final_res = greedyTour();

    int* curr_path = new int[n + 1];
    curr_path[0] = 0;
    visited[0] = true;

    int curr_bound = 0;
    for (int i = 0; i < n; ++i)
        curr_bound += (min1[i] + min2[i]);
    curr_bound = (curr_bound + 1) / 2;

    TSPRecursive(curr_bound, 0, 1, curr_path);
    
    delete[] curr_path;
}

void TSP::printResult() const {
    std::cout << "Минимальная стоимость: " << final_res << std::endl;
    std::cout << "Оптимальный маршрут: ";
    for (int i = 0; i <= n; ++i)
        std::cout << final_path[i] << (i < n ? " " : "");
    std::cout << std::endl;
}

int TSP::getResult() const { return final_res; }