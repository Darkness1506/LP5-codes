// Parallel BFS and DFS using OpenMP
// Compile using:
// g++ -fopenmp bfs_dfs_openmp.cpp -o graph

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    // Add edge to undirected graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Parallel Breadth First Search
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "\nParallel BFS Traversal: ";

        while (!q.empty()) {
            int size = q.size();
            vector<int> currentLevel;

            // Collect current level nodes
            for (int i = 0; i < size; i++) {
                int node = q.front();
                q.pop();

                currentLevel.push_back(node);
                cout << node << " ";
            }

            // Parallel processing of neighbors
            #pragma omp parallel for
            for (int i = 0; i < currentLevel.size(); i++) {
                int node = currentLevel[i];

                for (int neighbor : adj[node]) {

                    if (!visited[neighbor]) {

                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }

        cout << endl;
    }

    // Parallel DFS Utility Function
    void parallelDFSUtil(int node, vector<bool> &visited) {

        #pragma omp critical
        {
            if (visited[node])
                return;

            visited[node] = true;
            cout << node << " ";
        }

        // Create parallel tasks for neighbors
        #pragma omp parallel for
        for (int i = 0; i < adj[node].size(); i++) {
            int neighbor = adj[node][i];

            if (!visited[neighbor]) {
                parallelDFSUtil(neighbor, visited);
            }
        }
    }

    // Parallel DFS
    void parallelDFS(int start) {
        vector<bool> visited(V, false);

        cout << "\nParallel DFS Traversal: ";

        parallelDFSUtil(start, visited);

        cout << endl;
    }
};

int main() {

    int V, E;

    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "Enter number of edges: ";
    cin >> E;

    Graph g(V);

    cout << "Enter edges (u v):\n";

    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int start;

    cout << "Enter starting vertex: ";
    cin >> start;

    double start_time, end_time;

    // Parallel BFS
    start_time = omp_get_wtime();

    g.parallelBFS(start);

    end_time = omp_get_wtime();

    cout << "BFS Execution Time: "
         << end_time - start_time
         << " seconds\n";

    // Parallel DFS
    start_time = omp_get_wtime();

    g.parallelDFS(start);

    end_time = omp_get_wtime();

    cout << "DFS Execution Time: "
         << end_time - start_time
         << " seconds\n";

    return 0;
}