#include <bits/stdc++.h>
#include <omp.h>
using namespace std;


void sequentialOps(const vector<int> &arr, int &minVal, int &maxVal, long long &sum, double &avg){
    minVal = numeric_limits<int>::max();
    maxVal = numeric_limits<int>::min();
    sum = 0;
    for(int val:arr){
        if(val < minVal) minVal = val;
        if(val > maxVal) maxVal = val;
        sum += val;
    }
    avg = static_cast<double>(sum) /arr.size();
}

void parallelOps(const vector<int> &arr, int &minVal, int &maxVal, long long &sum, double &avg){
    minVal = numeric_limits<int>::max();
    maxVal = numeric_limits<int>::min();
    sum = 0;

    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for(int val:arr){
        if(val < minVal) minVal = val;
        if(val > maxVal) maxVal = val;
        sum += val;
    }
    avg = static_cast<double>(sum) /arr.size();
}

// int main() {
//     int n = 10000000;  // Large dataset
//     std::vector<int> data(n);
//     srand(time(0));
//     for (int i = 0; i < n; i++)
//         data[i] = rand() % 1000000;

//     int minVal, maxVal;
//     long long sum;
//     double avg;

//     // Sequential
//     auto start = std::chrono::high_resolution_clock::now();
//     sequentialOps(data, minVal, maxVal, sum, avg);
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> seqTime = end - start;
//     std::cout << "Sequential:\n";
//     std::cout << "Min: " << minVal << ", Max: " << maxVal << ", Sum: " << sum << ", Avg: " << avg << "\n";
//     std::cout << "Time: " << seqTime.count() << " sec\n";

//     // Parallel
//     start = std::chrono::high_resolution_clock::now();
//     parallelOps(data, minVal, maxVal, sum, avg);
//     end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> parTime = end - start;
//     std::cout << "\nParallel:\n";
//     std::cout << "Min: " << minVal << ", Max: " << maxVal << ", Sum: " << sum << ", Avg: " << avg << "\n";
//     std::cout << "Time: " << parTime.count() << " sec\n";

//     return 0;
// }

int main() {
    int n = 10000000;  // Large dataset
    vector<int> data(n);
    srand(time(0));
    for (int i = 0; i < n; i++)
        data[i] = rand() % 1000000;

    int minVal, maxVal;
    long long sum;
    double avg;

    // Sequential
    double start = omp_get_wtime();
    sequentialOps(data, minVal, maxVal, sum, avg);
    double end = omp_get_wtime();
    cout << "Sequential:\n";
    cout << "Min: " << minVal << ", Max: " << maxVal << ", Sum: " << sum << ", Avg: " << avg << "\n";
    cout << "Time: " << end - start << " sec\n";

    // Parallel
    start = omp_get_wtime();
    parallelOps(data, minVal, maxVal, sum, avg);
    end = omp_get_wtime();
    cout << "\nParallel:\n";
    cout << "Min: " << minVal << ", Max: " << maxVal << ", Sum: " << sum << ", Avg: " << avg << "\n";
    cout << "Time: " << end - start << " sec\n";

    return 0;
}