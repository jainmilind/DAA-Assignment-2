#include <array>
#include <bits/stdc++.h>
using namespace std;
// template <typename A, typename B> ostream& operator<<(ostream& os, const pair<A, B>& p) { return os << "(" << p.first << ", " << p.second << ")"; }
// template <typename T_container, typename T = typename enable_if < !is_same<T_container, string>::value, typename T_container::value_type >::type > ostream& operator<<(ostream& os, const T_container& v) { os << '{'; string sep; for (const T& x : v) os << sep << x, sep = ", "; return os << '}'; }
// template <typename T> void debug_out(string s, T t) { cout << "[" << s << ": " << t << "]\n"; }
// template <typename T, typename... U> void debug_out(string s, T t, U... u) { int w = 0, c = 0; while (w < (int)s.size()) { if (s[w] == '(') c++; if (s[w] == ')') c--; if (!c and s[w] == ',') break; w++; } cout << "[" << s.substr(0, w) << ": " << t << "] "; debug_out(s.substr(w + 2, (int)s.size() - w - 1), u...); }
// #define dbg(x...) debug_out(#x, x)


vector<int> get_path (int start, int end, vector<vector<int>>& res_adj, int del) {

    int n = res_adj.size();

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


void augment(vector<int>& f, vector<int>& path, vector<vector<int>>& res_adj, map<array<int, 2>, int>& edg_to_i) {
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
            f[edg] += bottle_neck;
        }

        if (edg_to_i.count({ path[i], path[i - 1] })) {
            int edg = edg_to_i[{path[i], path[i - 1]}];
            f[edg] -= bottle_neck;
        }
    }
};
// TODO: Apply `Choosing Good Augmenting Paths` its very trivial to add so i am not doing

vector<vector<int>> ford_fulkerson(int start, int end, int n, int m, vector<vector<int>>& adj, map<array<int, 2>, int>& edg_to_i) {
    // initial same 
    auto res_adj = adj;

    // as in slides
    vector<int> f(m);

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
            augment(f, path, res_adj, edg_to_i);
        }
        del /= 2;
    }
    return res_adj;
}

void reach_from_source(int u, vector<bool> &vis, vector<vector<int>>& res_adj){
    int n = vis.size();
    vis[u] = 1;
    for (int i = 0; i < n; ++i) {
        if (!vis[i] and res_adj[u][i]) {
            reach_from_source(i, vis, res_adj);
        }
    }
}

// ! remove the link later
// https://www.geeksforgeeks.org/minimum-cut-in-a-directed-graph/
// TODO: test this code 
// iska solution slides me nahi tha
vector<array<int, 2>> minimum_cut(int start, int end, int n, int m, vector<vector<int>>& res_adj, vector<vector<int>>& adj) {
    vector<array<int, 2>> mincut;

    vector<bool> vis(n);
    
    reach_from_source(start, vis, res_adj);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (vis[i] and !vis[j] and adj[i][j]) 
                mincut.push_back({i, j});
    
    return mincut;
}

int main() {
    // number of vertices
    int n; cin >> n;
    // number of edges
    int m; cin >> m;
    cout << n << ' ' << m << endl;
    // using adjanceny matrix as mentioned in slides
    vector<vector<int>> adj(n, vector<int>(n, 0));
    map<array<int, 2>, int> edg_to_i;

    for (int i = 0; i < m; ++i) {
        int x, y, c; cin >> x >> y >> c;
        x--, y--;
        adj[x][y] = c;
        edg_to_i[{x, y}] = i;
    }

    // start end of ford fulkerson
    int start, end;
    cin >> start >> end;
    // as 0 indexed
    start--, end--;


    auto res_adj = ford_fulkerson(start, end, n, m, adj, edg_to_i);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cout << res_adj[i][j] << " \n"[j == n - 1];
        
    cout << endl;
    

    // edge list printing for easy checking on https://csacademy.com/app/graph_editor/
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (res_adj[i][j])
                cout << i + 1 << ' ' << j + 1 << ' ' << res_adj[i][j] << endl;


    // minimum cut
    auto min_cut = minimum_cut(start, end, n, m, res_adj, adj);

    
}
