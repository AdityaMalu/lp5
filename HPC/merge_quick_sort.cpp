#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void serialBubbleSort(vector<int> &arr){
    int n = arr.size();
    for(int i = 0 ; i < n-1 ; i++){
        for(int j = 0 ; j < n-i-1 ; j++){
            if(arr[j] > arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void parellelBubbleSort(vector<int> &arr){
    int n = arr.size();
    for(int i = 0 ; i < n ; i++){
        #pragma omp parellal for
        for(int j = i%2; j < n-1 ; j+=2){
            if(arr[j] > arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void merge(vector<int> &arr, int l, int m, int r){
    int n1 = m-l+1;
    int n2 = r-m;

    vector<int> L(n1),R(n2);
    for(int i = 0 ; i < n1 ; i++) L[i] = arr[l+i];
    for(int i = 0 ; i < n2 ; i++) R[i] = arr[m+1+i];

    int i =0, j = 0 , k = l;
    while(i<n1 && j < n2){
        arr[k++] = (L[i] <= R[j]) ? L[i++]:R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];

}


void serialMergeSort(vector<int> &arr,int l, int r){
    if(l < r){
        int m = (l+r)/2;
        serialMergeSort(arr,l,m);
        serialMergeSort(arr,m+1,r);
        merge(arr,l,m,r);
    }
    
}

void parellelMergeSort(vector<int> &arr, int l , int r , int depth =0){
    if(l<r){
        int m = (l+r)/2;

        if(depth < 4){
            #pragma omp parallel sections
            {
                #pragma omp section
                parellelMergeSort(arr,l,m,depth+1);
                #pragma omp section
                parellelMergeSort(arr,m+1,r,depth+1);
            }
          
        }
        else{
            serialMergeSort(arr,l,m);
            serialMergeSort(arr,m+1,r);
        }
        merge(arr,l,m,r);
    }
}

void printArray(const vector<int>& arr, int maxPrint = 20) {
    int count = min((int)arr.size(), maxPrint);
    for (int i = 0; i < count; i++) {
        cout << arr[i] << " ";
    }

    for (int i = 0; i < arr.size() - 1; i++) {
        if (arr[i] > arr[i+ 1]) {
            cout<<"Stupid idea"<<endl;
            break;
        }
    }
    if (arr.size() > maxPrint) cout << "...";
    cout << endl;
}


// int main(){
//     int n = 100000;
//     vector<int> data(n);
//     srand(time(0));
//     for(int i = 0 ; i < n ; i++){
//         data[i] = rand()%1000000;
    
//     }

//     vector<int> arr1 = data;
//     vector<int> arr2 = data;

//     auto start = chrono::high_resolution_clock::now();
//     serialBubbleSort(arr1);
//     auto end = chrono::high_resolution_clock::now();
//     chrono::duration<double> serialBubbleTime = end-start;
//     cout<<"Serial Bubble Sort TIme: "<< serialBubbleTime.count()<<"sec\n";

//     arr1 = data;
//     start = chrono::high_resolution_clock::now();
//     parellelBubbleSort(arr1);
//     end = chrono::high_resolution_clock::now();
//     chrono::duration<double> parallelBubbleTime = end - start;
//     cout << "Parallel Bubble Sort time: " << parallelBubbleTime.count() << " sec\n";

//     // Serial Merge Sort
//     arr2 = data;
//     start = chrono::high_resolution_clock::now();
//     serialMergeSort(arr2, 0, arr2.size() - 1);
//     end = chrono::high_resolution_clock::now();
//     chrono::duration<double> serialMergeTime = end - start;
//     cout << "Serial Merge Sort time: " << serialMergeTime.count() << " sec\n";

//     // Parallel Merge Sort
//     arr2 = data;
//     start = chrono::high_resolution_clock::now();
//     parellelMergeSort(arr2, 0, arr2.size() - 1);
//     end = chrono::high_resolution_clock::now();
//     chrono::duration<double> parallelMergeTime = end - start;
//     cout << "Parallel Merge Sort time: " << parallelMergeTime.count() << " sec\n";

//     return 0;
// }


int main() {
    int n = 100000;
    vector<int> data(n);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        data[i] = rand() % 1000000;
    }

    vector<int> arr1 = data;
    vector<int> arr2 = data;

    // Serial Bubble Sort
    double start = omp_get_wtime();
    serialBubbleSort(arr1);
    double end = omp_get_wtime();
    cout << "Serial Bubble Sort Time: " << end - start << " sec\n";
    cout << "Sorted Array (first 20 elements): ";
    printArray(arr1);

    arr1 = data;
    // Parallel Bubble Sort
    start = omp_get_wtime();
    parellelBubbleSort(arr1);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << " sec\n";
    cout << "Sorted Array (first 20 elements): ";
    printArray(arr1);

    // Serial Merge Sort
    arr2 = data;
    start = omp_get_wtime();
    serialMergeSort(arr2, 0, arr2.size() - 1);
    end = omp_get_wtime();
    cout << "Serial Merge Sort Time: " << end - start << " sec\n";
    cout << "Sorted Array (first 20 elements): ";
    printArray(arr2);

    // Parallel Merge Sort
    arr2 = data;
    start = omp_get_wtime();
    parellelMergeSort(arr2, 0, arr2.size() - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " sec\n";
    cout << "Sorted Array (first 20 elements): ";
    printArray(arr2);

    return 0;
}
// @AdityaMalu ➜ /workspaces/BE_Lab_Work (main) $ g++ -fopenmp /workspaces/BE_Lab_Work/HPC/BFS_DFS.cpp -o bfs_dfs
// @AdityaMalu ➜ /workspaces/BE_Lab_Work (main) $ ./bfs_dfs