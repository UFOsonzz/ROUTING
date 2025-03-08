#include <bits/stdc++.h>

using namespace std;

const int BATCH_SIZE = 20;
const int epoch = 5;

int read_file(ifstream& file) {
    int ans = 0;
    string line;
    getline(file, line);
    cout << line << '\n';
    for (int i = 11; i < line.size(); i++) 
        ans = ans * 10 + (line[i] - '0');
    getline(file, line);
    cout << line << '\n';
    return ans;
}

double evaluate(string dp, string ga) {
    ifstream outputdp(dp);
    ifstream outputga(ga);
    if (!outputdp.is_open() || !outputga.is_open()) {
        cout << "Error 1\n";
    } 

    int ans_dp = read_file(outputdp);
    int ans_ga = read_file(outputga);

    double precision = (double)ans_dp / (double)ans_ga * 100.0;
    if (precision > 100) {
        cout << "Error 2\n";
    }

    return precision;
}

int main() {

    double current_precision = 100;
    double average_precision = 100;
    for (int e = 1; e <= epoch; e++) {
        cout << "BATCH " << e << ": \n";
        cout << "Generating testcases...\n";
        system("g++ Small_generator.cpp -o Small_generator && Small_generator");
        for (int i = 1; i <= BATCH_SIZE; i++) {
            string testcase = "TEST\\TSP" + to_string(i) + ".INP";
            cout << "RUNNING TESTCASE " << i << ":\n";
            string rundp = "DYNAMIC_PROGRAMMING < " + testcase + " > outputdp.txt";
            string runga = "GENETIC_ALGORITHM < " + testcase + " > outputga.txt";
            system(rundp.c_str());
            system(runga.c_str());
            
            double precision = evaluate("outputdp.txt", "outputga.txt");
            if (i == 0)
                current_precision = precision;
            else current_precision = (current_precision * (i - 1) + precision) / (double)i;
            cout << "current precision: " << current_precision << '\n';
            system("del outputdp.txt outputga.txt");
        }

        if (e == 1) 
            average_precision = current_precision;
        else average_precision = (average_precision + (e - 1) + current_precision) / (double)e;
    }

    cout << "AVERAGE PRECISION: " << average_precision;
}