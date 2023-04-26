// #include "./Ford_fulkerson_max_flow.cpp"
#include "Ford_fulkerson_max_flow.cpp"

using namespace std;

int main(){

    /**
        Input Format - Total number of nodes (n), Number of nodes in the left bipartite set (bi_index), number of edges (m)
        Each of the next m lines must consist of two integers u, v. u <= bi_index, v > bi_index. u is a node from the left set and v is a node from the right set
    */

    int n, m, bi_index;
    cin >> n >> m >> bi_index;

    // Adjaceny matrix of the bipartite graph
    vector<vector<int>> adj(n+2, vector<int>(n+2, 0));

    int cnt = 0;
    // etoi maps every edge to an integer
    map<array<int, 2>, int> etoi;

    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        assert(u <= bi_index && v > bi_index);
        adj[u][v] = 1;
        etoi[{u,v}] = cnt++;
    }

    // Adding edges from source to left bipartite set
    for(int i = 1; i <= bi_index; i++){
        adj[0][i] = 1;
        etoi[{0,i}] = cnt++;
    }

    // Adding edges from right bipartite set to sink
    for(int i = bi_index + 1; i <= n + 1; i++){
        adj[i][n+1] = 1;
        etoi[{i,n+1}] = cnt++;
    }

    // Applying ford fulkerson algorightm to get residual adjacency matrix.
    auto res_adj = ford_fulkerson(0, n+1, n+2, m+n, adj, etoi);

    // Stores all the edges of the maximum matching
    vector<pair<int, int>> max_matching;

    // Find the edges of the maximum matching by checking if the flow of the edge is 1
    for(int i = 1; i <= bi_index; i++){
        for(int j = bi_index + 1; j <= n; j++){
            if(adj[i][j] && !res_adj[i][j])
                max_matching.push_back({i, j});
        }
    }

    cout << max_matching.size() << endl;

    for(auto edge : max_matching){
        cout << edge.first << " " << edge.second << endl;
    }

    return 0;
}