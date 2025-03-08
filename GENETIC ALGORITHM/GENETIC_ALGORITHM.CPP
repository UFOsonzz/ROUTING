#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int Rand(int Min, int Max) {
    return Min + rng() % (Max - Min + 1);
}

class TSP {
    private : 
        int node_num;
        vector<vector<int>> adj_matrix;
        vector<pair<int, vector<int>>> population;
        int population_size = 100;
        int mutation_cap = 10000;
        int mutation_rate = 5000;
        int cooling_rate = 5;
        
    public : 

    int calc_fitness(vector<int> individual) {
        int fitness = 0;
        for (int i = 0; i < node_num; i++) 
            fitness += adj_matrix[individual[i]][individual[(i + 1) % node_num]];
        
        return fitness;
    }
    
    TSP(int node_num, vector<vector<int>> adj_matrix) :
        node_num(node_num), adj_matrix(adj_matrix) {
            for (int i = 0; i < population_size; i++) {
                vector<int> individual(node_num);
                iota(individual.begin(), individual.end(), 1);
                random_shuffle(individual.begin(), individual.end());
                int fitness = calc_fitness(individual);
                population.emplace_back(fitness, individual);
            }
        }

    pair<int, int> select_parents(vector<int>& chances) {
        int first_parent = chances[Rand(0, (int)chances.size() - 1)];
        int second_parent = chances[Rand(0, (int)chances.size() - 1)];
        if (second_parent == first_parent) {
            if (second_parent > 0)
                second_parent--;
            else second_parent++;
        }

        return {first_parent, second_parent};
    }

    pair<int, vector<int>> crossover(pair<int, vector<int>> x, pair<int, vector<int>> y) {
        vector<int> strong_candidate[node_num + 2];
        for (int i = 0; i < node_num; i++) {
            strong_candidate[x.second[i]].push_back(x.second[(i + 1) % node_num]);
            strong_candidate[y.second[i]].push_back(y.second[(i + 1) % node_num]);
        }

        bool visited[node_num + 2];
        memset(visited, false, sizeof(visited));
        vector<int> child;
        int start = Rand(1, node_num);
        child.push_back(start);
        visited[start] = true;

        while ((int)child.size() < node_num) {
            int u = child.back();
            bool found = false;
            for (auto c : strong_candidate[u]) {
                if (!visited[c]) {
                    visited[c] = true;
                    child.push_back(c);
                    found = true;
                    break;
                }
            }

            if (found)
                continue;
            
            int min_dist = INF;
            int next_city = -1;
            
            for (int i = 1; i <= node_num; i++) {
                if (!visited[i] && adj_matrix[u][i] < min_dist) {
                    min_dist = adj_matrix[u][i];
                    next_city = i;
                }
            }

            visited[next_city] = true;
            child.push_back(next_city);
        }

        int fitness = calc_fitness(child);
        return {fitness, child};
    }

    void mutate(pair<int, vector<int>>& individual) {
        // swap 2 city
        int first_city = Rand(1, node_num - 1);
        int second_city = Rand(0, first_city - 1);

        swap(individual.second[first_city], individual.second[second_city]);
        int new_fitness = calc_fitness(individual.second);
        individual.first = new_fitness;
    }

    void evolve() {
        sort(population.begin(), population.end(), [&](pair<int, vector<int>> x, pair<int, vector<int>> y) {
            return x.first < y.first;
        });

        vector<int> chances;
        for (int i = 0; i < population_size; i++) 
            for (int j = 1; j <= population_size - i; j++) 
                chances.push_back(i);
        
        vector<pair<int, vector<int>>> new_population;
        for (int i = 0; i < population_size; i++) {
            pair<int, int> parents = select_parents(chances);
            pair<int, vector<int>> child = crossover(population[parents.first], population[parents.second]);
            if (Rand(0, mutation_cap) < mutation_rate) mutate(child);
            new_population.push_back(child);
        }

        population = new_population;
        mutation_rate -= cooling_rate;
    }

    pair<int, vector<int>> best_tour() {
        sort(population.begin(), population.end(), [&](pair<int, vector<int>> x, pair<int, vector<int>> y) {
            return x.first < y.first;
        });

        return population[0];
    }
};

int main() {
    int node_num;
    cin >> node_num; 
    vector<vector<int>> adj_matrix;
    adj_matrix.resize(node_num + 1, vector<int>(node_num + 1));

    for (int i = 1; i <= node_num; i++)
        for (int j = 1; j <= node_num; j++)
            cin >> adj_matrix[i][j];

    TSP tsp(node_num, adj_matrix);
    int evolution_num = 1000;

    pair<int, vector<int>> result;

    for (int i = 1; i <= evolution_num; i++) {
        tsp.evolve();
        if (i == 1) 
            result = tsp.best_tour();
        else {
            pair<int, vector<int>> cur_best = tsp.best_tour();
            if (cur_best.first < result.first)
                result = cur_best;
        }
    }

    cout << "GA_answer: " << result.first << '\n';
    for (int i = 0; i < node_num; i++) {
        if (result.second[i] == 1) {
            rotate(result.second.begin(), result.second.begin() + i, result.second.end());
            break;
        }
    }

    result.second.push_back(1);
    cout << "shortest tour: ";
    for (auto city : result.second) 
        cout << city << " ";
}