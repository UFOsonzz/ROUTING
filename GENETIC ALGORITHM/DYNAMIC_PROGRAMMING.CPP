#include <bits/stdc++.h>

using namespace std;

const int NODE_NUM_LIM = 20;
const int VALUE_LIM = 1e9;

int main() {
    int node_num;
    cin >> node_num;
    int adj_matrix[node_num + 2][node_num + 2];

    for (int i = 1; i <= node_num; i++) 
        for (int j = 1; j <= node_num; j++) 
            cin >> adj_matrix[i][j];
    
    vector<vector<pair<int, int>>> shortest_tour;
    shortest_tour.resize((1 << node_num) + 2, vector<pair<int, int>>(node_num + 2));

    for (int i = 0; i < 1 << node_num; i++)
        for (int j = 0; j < node_num; j++)
            shortest_tour[i][j] = {VALUE_LIM, -1};
        
    shortest_tour[1][0] = {0, -1};

    for (int visited_mask = 0; visited_mask < 1 << node_num; visited_mask++) {
        for (int current_city = 0; current_city < node_num; current_city++) {
            if (shortest_tour[visited_mask][current_city].first == VALUE_LIM)
                continue;
            if (visited_mask >> current_city & 1) {
                // neu da tham current_city
                for (int next_city = 0; next_city < node_num; next_city++) {
                    if (visited_mask >> next_city & 1)
                        continue;
                    // neu da tham next_city thi khong xet
                    int new_visited_mask = visited_mask | 1 << next_city;
                    int current_value = shortest_tour[visited_mask][current_city].first + adj_matrix[current_city + 1][next_city + 1];
                    if (current_value < shortest_tour[new_visited_mask][next_city].first)
                        shortest_tour[new_visited_mask][next_city] = {current_value, current_city};
                }
            }
        }
    }

    int last_city, current_min_tour = VALUE_LIM;
    for (int endpoint = 0; endpoint < node_num; endpoint++) {
        if (shortest_tour[(1 << node_num) - 1][endpoint].first + adj_matrix[endpoint + 1][1] < current_min_tour) {
            current_min_tour = shortest_tour[(1 << node_num) - 1][endpoint].first + adj_matrix[endpoint + 1][1];
            last_city = endpoint;
        }
    }

    cout << "DP_answer: " << current_min_tour << '\n';
    vector<int> tour;
    int current_visited_mask = (1 << node_num) - 1;
    do {
        tour.push_back(last_city + 1);
        int next_city = shortest_tour[current_visited_mask][last_city].second;
        current_visited_mask ^= 1 << last_city;
        last_city = next_city;
    } while (last_city != -1);

    reverse(tour.begin(), tour.end());
    tour.push_back(1);

    cout << "shortest tour: ";
    for (auto city : tour)
        cout << city << " ";
}