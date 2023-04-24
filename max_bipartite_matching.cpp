// #include "./Ford_fulkerson_max_flow.cpp"
#include <bits/stdc++.h>

using namespace std;

// Remove later
vector<vector<int>> ford_fulkerson(int start, int end, int n, int m, vector<vector<int>>& adj, map<array<int, 2>, int>& edg_to_i) {
    // initial same 
    auto res_adj = adj;

    // as in slides
    vector<int> f(m);

    auto get_path = [&](int start, int end, vector<vector<int>>& res_adj, int del) {
        vector<bool> vis(n);
        vis[start] = 1;
        queue<int> q;
        q.push(start);
        vector<int> par(n, -1);

        while (q.size()) {
            int u = q.front();
            q.pop();

            for (int i = 0; i < n; ++i) {
                if (!vis[i] and res_adj[u][i] and res_adj[u][i] >= del) {
                    par[i] = u;
                    vis[i] = 1;
                    q.push(i);
                }
            }
        }

        vector<int> path;
        if (par[end] == -1)
            return path;
        while (end != -1) {
            path.push_back(end);
            end = par[end];
        }
        reverse(path.begin(), path.end());
        return path;
    };


    auto augment = [&](vector<int>& f, vector<int>& path, vector<vector<int>>& res_adj) {
        int bottle_neck = INT_MAX;
        for (int i = 1; i < (int)path.size(); ++i) {
            bottle_neck = min(bottle_neck, res_adj[path[i - 1]][path[i]]);
        }

        // dbg(bottle_neck);
        for (int i = 1; i < (int)path.size(); ++i) {
            res_adj[path[i - 1]][path[i]] -= bottle_neck;
            res_adj[path[i]][path[i - 1]] += bottle_neck;
            if (edg_to_i.count({ path[i - 1], path[i] })) {
                int edg = edg_to_i[{path[i - 1], path[i]}];
                // check if forward or backward edge
                f[edg] -= bottle_neck;
            }
            if (edg_to_i.count({ path[i], path[i - 1] })) {
                int edg = edg_to_i[{path[i], path[i - 1]}];
                // check if forward or backward edge
                f[edg] += bottle_neck;
            }
        }
    };

    int mx_cap = 0;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            mx_cap = max(mx_cap, adj[i][j]);
        }
    }

    int del = 1 << (int)floor(log2(mx_cap));

    while(del >= 1){
        while (true) {
            auto path = get_path(start, end, res_adj, del);
            // dbg(path);
            if (path.size() == 0)
                break;

            // f and res_adj is updated inside it
            augment(f, path, res_adj);
        }
        del /= 2;
    }
    return res_adj;
}

int main(){

    /**
        Input Format - Total number of nodes (n), Number of nodes in the left bipartite set (bi_index), number of edges (m)
        Each of the next m lines must consist of two integers u, v. u <= bi_index, v > bi_index. u is a node from the left set and v is a node from the right set
    */

    int n, m, bi_index;
    cin >> n >> m >> bi_index;

    vector<vector<int>> adj(n+2, vector<int>(n+2, 0));

    int cnt = 0;
    map<array<int, 2>, int> etoi;

    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        assert(u <= bi_index && v > bi_index);
        adj[u][v] = 1;
        etoi[{u,v}] = cnt++;
    }

    for(int i = 1; i <= bi_index; i++){
        adj[0][i] = 1;
        etoi[{0,i}] = cnt++;
    }

    for(int i = bi_index + 1; i <= n + 1; i++){
        adj[i][n+1] = 1;
        etoi[{i,n+1}] = cnt++;
    }

    auto res_adj = ford_fulkerson(0, n+1, n+2, m+n, adj, etoi);

    vector<pair<int, int>> max_matching;
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