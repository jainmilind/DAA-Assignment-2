#include "Ford_fulkerson_max_flow.cpp"
#include <cstdlib>

using namespace std;

// Checks if the graph is bipartite, if it is it removes the 
void graph_2_col(int u, vector<vector<int>>& adj, vector<int>& col){

    for(int v = 1; v < adj[u].size(); v++){
        if(adj[u][v] && !col[v]){
            adj[u][v] = col[u] == 1;
            adj[v][u] = col[u] == 2;
            col[v] = 3 ^ col[u];
            graph_2_col(v, adj, col);
        }
        else if(adj[u][v] && col[u] == col[v]){
            cerr << "Graph is not bipartite\n";
            exit(EXIT_FAILURE);
        }
    }

    return;
}

/**
 * Function to run the Ford Fulkerson and use it to solve the Bipartite Matching Problem
 */
int main()
{

    int n, m;
    cin >> n >> m;

    // Adjaceny matrix of the bipartite graph
    vector<vector<int>> adj(n + 2, vector<int>(n + 2, 0));

    int cnt = 0;
    // etoi maps every edge to an integer
    map<array<int, 2>, int> etoi;

    // Each of the next m lines must consist of two integers u, v. u <= bi_index, v > bi_index. u is a node from the left set and v is a node from the right set
    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        // assert(u <= bi_index && v > bi_index);
        adj[u][v] = 1;
        adj[v][u] = 1;
        etoi[{u, v}] = cnt++;
    }

    vector<int> col(n+1, 0);
    
    for(int i = 1; i <= n; i++){
        if(!col[i]){
            col[i] = 1;
            graph_2_col(i, adj, col);
        }
    }
    
    // Start of timer
    auto startMatching = std::chrono::high_resolution_clock::now();
    
    //Colour the graph to check if it is bipartite
    for(int i = 1; i <= n; i++){
        if(col[i] == 1){
            adj[0][i] = 1;
        }
        else{
            adj[i][n+1] = 1;
        }
    }

    // Applying ford fulkerson algorightm to get residual adjacency matrix.
    auto res_adj = ford_fulkerson(0, n + 1, n + 2, m + n, adj, etoi);

    // Stores all the edges of the maximum matching
    vector<pair<int, int>> max_matching;

    // Find the edges of the maximum matching by checking if the flow of the edge is 1
    for(int i = 1; i <= n; i++){
        if(col[i] != 1)
            continue;
        
        for(int j = 1; j < adj[i].size(); j++){
            if(adj[i][j] == 1 && res_adj[i][j] == 0){
                max_matching.push_back({i, j});
            }
        }
    }

    // End of timer
    auto stopMatching = std::chrono::high_resolution_clock::now();
    auto matchingTime = std::chrono::duration_cast<std::chrono::microseconds>(stopMatching - startMatching);
    cerr << setprecision(15) << "Time for solving Bipartite Matching is: " << (double)matchingTime.count() / 1000 << " ms" << endl;
    cout << max_matching.size() << endl;

    for (auto edge : max_matching)
    {
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}