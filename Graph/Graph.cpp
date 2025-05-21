#include <iostream>
#include <fstream>
#include <vector>
#include <string>


void add_verge(std::vector<int> adj[], int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int get_vertices() {
    std::ifstream input_file("input.txt");
    if (!input_file.is_open()) {
        throw std::runtime_error("File can not be opened!");
    }

    std::string line;
    std::string number;
    std::getline(input_file, line);

    size_t i = 0;
    while (i < line.size() && std::isdigit(line[i])) {
        number += line[i];
        ++i;
    }

    input_file.close();
    return std::stoi(number);
}

void get_data(std::vector<int> adj[], std::vector<std::vector<int>>& adjacency_matrix) {
    std::ifstream input_file("input.txt");
    if (!input_file.is_open()) {
        throw std::runtime_error("File can not be opened!");
    }

    std::string line;
    std::getline(input_file, line);

    while (std::getline(input_file, line)) {
        std::string left_st;
        std::string right_st;

        size_t i = 0;
        while (i < line.size() && std::isdigit(line[i])) {
            left_st += line[i];
            ++i;
        }

        while (i < line.size() && !std::isdigit(line[i])) {
            ++i;
        }

        while (i < line.size() && std::isdigit(line[i])) {
            right_st += line[i];
            ++i;
        }

        int left = std::stoi(left_st);
        int right = std::stoi(right_st);

        add_verge(adj, left, right);
        adjacency_matrix[left][right] = adjacency_matrix[right][left] = 1;
    }

    input_file.close();
}

void APU_til(std::vector<int> adj[], int u,
    std::vector<bool>& visited, std::vector<int>& disc,
    std::vector<int>& low, int& time, int parent,
    std::vector<bool>& isAP) {

    int children = 0;
    visited[u] = true;
    disc[u] = low[u] = ++time;

    for (int v : adj[u]) {
        if (!visited[v]) {
            children++;
            APU_til(adj, v, visited, disc, low, time, u, isAP);

            low[u] = std::min(low[u], low[v]);

            if (parent != -1 && low[v] >= disc[u]) {
                isAP[u] = true;
            }
        }
        else if (v != parent) {
            low[u] = std::min(low[u], disc[v]);
        }
    }

    if (parent == -1 && children > 1) {
        isAP[u] = true;
    }
}

void find(std::vector<int> adj[], int V) {
    std::vector<int> disc(V, 0);
    std::vector<int> low(V, 0);
    std::vector<bool> visited(V, false);
    std::vector<bool> isAP(V, false);
    int time = 0;

    for (int u = 0; u < V; ++u) {
        if (!visited[u]) {
            APU_til(adj, u, visited, disc, low, time, -1, isAP);
        }
    }

    std::cout << "\033[31mArticulation Points: \033[34m";
    bool hasAP = false;

    for (int u = 0; u < V; ++u) {
        if (isAP[u]) {
            std::cout << u << ", ";
            hasAP = true;
        }
    }

    if (!hasAP) std::cout << "There are no Articulation Points!";
    std::cout << "\033[0m\n";

}

int main() {
    try {
        int vertices = get_vertices();
        std::vector<std::vector<int>> adjacency_matrix(vertices + 1, std::vector<int>(vertices + 1, 0));
        std::vector<std::vector<int>> adj_vector(vertices + 1);

        get_data(adj_vector.data(), adjacency_matrix);
        find(adj_vector.data(), vertices);
    }

    catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
