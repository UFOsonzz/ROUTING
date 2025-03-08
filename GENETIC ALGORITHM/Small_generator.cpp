#include <bits/stdc++.h>

using namespace std;

const int BATCH_SIZE = 20;
const int NODE_NUM_LIM = 20;
const int WEIGHT_LIM = 100;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

int Rand(int Min, int Max) {
    return Min + rng() % (Max - Min + 1);
}

void generate(ofstream& input) {
    int node_num = Rand(8, NODE_NUM_LIM);
    input << node_num << '\n';
    for (int i = 1; i <= node_num; i++) {
        for (int j = 1; j <= node_num; j++) {
            int weightij = Rand(1, WEIGHT_LIM);
            input << weightij << " ";
        }

        input << '\n';
    }

    input.close();
}

int main() {
    for (int test_num = 1; test_num <= BATCH_SIZE; test_num++) {
        string test_name = "TEST\\TSP" + to_string(test_num) + ".inp";
        ofstream input(test_name);
        generate(input);
        cerr << "FINISHED TEST " << test_num << "!\n"; 
    }
}