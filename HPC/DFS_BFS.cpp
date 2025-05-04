#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int64_t a = 557759;
int64_t b = 205559;
int64_t c = 877361;
int64_t k = 100;
int64_t n = 10e3 + 3;
vector<bool> visited = vector<bool> (n, false);
bool found = false;

bool serialBFS(int64_t x, int64_t y) {
    visited.assign(n, false);

    queue<int64_t> q;
    q.push(x);
    visited[x] = 1;

    while (!q.empty()) {
        int64_t u = q.front();
        q.pop();

        if (u == y) return true;

        for (int i = 0; i < k; i++) {
            int64_t v = (u * a + b * i + c) % n;
            if (!visited[v]) {
                q.push(v);
                visited[v] = 1;
            }
        }
    }
    return false;
}

bool parallelBFS(int64_t x, int64_t y) {
    visited.assign(n, false);

    vector<int64_t> v1, v2;
    v1.push_back(x);
    visited[x] = 1;

    found = false;

    vector<int64_t>& current = v1;
    vector<int64_t>& next = v2;

    while (!current.empty()) {
        #pragma omp parallel for 
        for (int i = 0; i < current.size(); i++) {
            int64_t u = current[i];

            if (u == y) {
                found = true;
            }

            for (int j = 0; j < k; j++) {
                int64_t v = (u * a + b * j + c) % n;
                if (!visited[v]) {
                    #pragma omp critical
                    {
                        next.push_back(v);
                        visited[v] = 1;
                    }
                }
            }
        }

        if (found) return true;
        swap(current, next);
        next.clear();
    }
    return false;
}

bool serialDFS(int64_t x, int64_t y) {
    visited.assign(n, false);

    stack<int64_t> s;
    s.push(x);
    visited[x] = 1;

    while (!s.empty()) {
        int64_t u = s.top();
        s.pop();

        if (u == y) return true;

        for (int i = 0; i < k; i++) {
            int64_t v = (u * a + b * i + c) % n;
            if (!visited[v]) {
                s.push(v);
                visited[v] = 1;
            }
        }
    }
    return false;
}

void parallelDFSUtil(int64_t u, int64_t y) {
    if (found) return;

    bool skip = false;
    #pragma omp critical
    {
        if (visited[u]) skip = true;
        else visited[u] = 1;
    }

    if (skip) return;

    if (u == y) {
        found = true;
        return;
    }

    for (int i = 0; i < k; i++) {
        int64_t v = (u * a + b * i + c) % n;

        #pragma omp task if (!found)
        {
            parallelDFSUtil(v, y);
        }
    }
}

bool parallelDFS(int64_t x, int64_t y) {
    found = false;
    visited.assign(n, false);

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            parallelDFSUtil(x, y);
        }
    }

    return found;
}

int main() {
    int64_t x = 123456789 % n;
    int64_t y = 987654321 % n;

    double t1 = omp_get_wtime();
    bool result1 = serialBFS(x, y);
    double t2 = omp_get_wtime();

    if (result1) {
        cout << "Time taken by serial BFS " << (t2 - t1) << " seconds" << endl;
    } else {
        cout << "Node not found" << endl;
    }

    double t3 = omp_get_wtime();
    bool result2 = parallelBFS(x, y);
    double t4 = omp_get_wtime();

    if (result2) {
        cout << "Time taken by parallel BFS " << (t4 - t3) << " seconds" << endl;
    } else {
        cout << "Node not found" << endl;
    }

    double t5 = omp_get_wtime();
    bool result3 = serialDFS(x, y);
    double t6 = omp_get_wtime();

    if (result3) {
        cout << "Time taken by serial DFS " << (t6 - t5) << " seconds" << endl;
    } else {
        cout << "Node not found" << endl;
    }

    double t7 = omp_get_wtime();
    bool result4 = parallelDFS(x, y);
    double t8 = omp_get_wtime();

    if (result4) {
        cout << "Time taken by parallel DFS " << (t8 - t7) << " seconds" << endl;
    } else {
        cout << "Node not found" << endl;
    }

    return 0;
}

// @AdityaMalu ➜ /workspaces/BE_Lab_Work (main) $ g++ -fopenmp /workspaces/BE_Lab_Work/HPC/BFS_DFS.cpp -o bfs_dfs
// @AdityaMalu ➜ /workspaces/BE_Lab_Work (main) $ ./bfs_dfs