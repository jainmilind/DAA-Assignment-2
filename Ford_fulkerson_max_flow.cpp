#include <array>
#include <bits/stdc++.h>
using namespace std;

/**
 * Function to get a path with a source vertex, sink vertex and a value of delta
 * @param start This is the start vertex from where we need to find a path.
 * @param end This is the end vertex to where we need to find a path.
 * @param res_adj This is the adjacency matrix for the Residual Graph.
 * @param del This is delta value to ensure we get the augmented path.
 * @return It returns the path with the given values of source vertex, sink vertex and delta
 */
vector<int> get_path(int start, int end, vector<vector<int>> &res_adj, int del)
{
    int n = res_adj.size();
    // We create a vector to check if the vertex is visited
    vector<bool> vis(n);
    vis[start] = 1;
    queue<int> q;
    q.push(start);
    // We store the parent to each vertex in the path
    vector<int> par(n, -1);
    // Doing BFS to get a path
    while (q.size())
    {
        int u = q.front();
        q.pop();

        for (int i = 0; i < n; ++i)
        {
            // We take those vertices which have a path to an adjacent vertex with atleast a weight of delta
            if (!vis[i] and res_adj[u][i] and res_adj[u][i] >= del)
            {
                par[i] = u;
                vis[i] = 1;
                q.push(i);
            }
        }
    }
    vector<int> path;
    // If there is no parent for the sink then there was no path that was found
    if (par[end] == -1)
        return path;
    // Since the source also has no parent we find the path by iterating till the source
    while (end != -1)
    {
        path.push_back(end);
        end = par[end];
    }
    // Since the path we recieved is in reverse, we need to reverse it
    reverse(path.begin(), path.end());
    return path;
};

/**
 * Function to update the residual graph along the path with the bottle neck
 * @param f This is array of flows in each edge
 * @param path This is the augmented path for which we have to update the flow.
 * @param res_adj This is the adjacency matrix for the Residual Graph.
 * @param edg_to_i This is a map which assigns each edge a value to easier access.
 */
void augment(vector<int> &f, vector<int> &path, vector<vector<int>> &res_adj, map<array<int, 2>, int> &edg_to_i)
{
    // Initializing the bottle neck to the max value
    int bottle_neck = INT_MAX;
    // Iterating through the path to find the bottleneck
    for (int i = 1; i < (int)path.size(); ++i)
    {
        bottle_neck = min(bottle_neck, res_adj[path[i - 1]][path[i]]);
    }
    // Updating the residual graph and also updating the flow of each edge
    for (int i = 1; i < (int)path.size(); ++i)
    {
        // Reducing the forward edge by the bottleneck
        res_adj[path[i - 1]][path[i]] -= bottle_neck;
        // Increasing the backward edge by the bottleneck
        res_adj[path[i]][path[i - 1]] += bottle_neck;
        // If it is a forward path increase the flow on the edge
        if (edg_to_i.count({path[i - 1], path[i]}))
        {
            int edg = edg_to_i[{path[i - 1], path[i]}];
            f[edg] += bottle_neck;
        }
        // If it is a backward path decrease the flow on the edge
        if (edg_to_i.count({path[i], path[i - 1]}))
        {
            int edg = edg_to_i[{path[i], path[i - 1]}];
            f[edg] -= bottle_neck;
        }
    }
};

/**
 * Function to run Ford Fulkerson algorithm
 * @param start This is the start vertex for the graph on which we have to run Ford Fulkerson algorithm.
 * @param end This is the end vertex for the graph on which we have to run Ford Fulkerson algorithm.
 * @param n This is the number of vertices.
 * @param m This is the number of edges.
 * @param adj This is the adjacency matrix for the Graph.
 * @param edg_to_i This is a map which assigns each edge a value to easier access.
 * @return It returns the adjacency matrix for the final Graph.
 */
vector<vector<int>> ford_fulkerson(int start, int end, int n, int m, vector<vector<int>> &adj, map<array<int, 2>, int> &edg_to_i)
{
    // Residual graph which is same as present graph initially
    auto res_adj = adj;
    vector<int> f(m);
    // Finding the maximum capacity possible
    int mx_cap = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mx_cap = max(mx_cap, adj[i][j]);
        }
    }
    // Calculating the value of delta
    int del = 1 << (int)floor(log2(mx_cap));
    // Changing the value of delta till there is a path from start to end vertex
    while (del >= 1)
    {
        while (true)
        {
            // Finding a path from source to end vertex
            auto path = get_path(start, end, res_adj, del);
            // dbg(path);
            // If no such path exists then break out
            if (path.size() == 0)
                break;

            // f and res_adj is updated inside it
            augment(f, path, res_adj, edg_to_i);
        }
        del /= 2;
    }
    return res_adj;
}

/**
 * Function to get all the vertices that can be visited from source vertex
 * @param u This is the source vertex .
 * @param vis This is the vector to keep track of all the visited vertices.
 * @param res_adj This is the adjacency matrix for the final Graph.
 */
void reach_from_source(int u, vector<bool> &vis, vector<vector<int>> &res_adj)
{
    int n = vis.size();
    // Marking the source as visible
    vis[u] = 1;
    for (int i = 0; i < n; ++i)
    {
        // If the vertex is not visited and there is an edge between u and this vertex, recursively call this function to find all the points u can reach from this point
        if (!vis[i] and res_adj[u][i])
        {
            reach_from_source(i, vis, res_adj);
        }
    }
}

/**
 * Function to find the edges that comprise the minimum cut.
 * @param start This is the start vertex for the graph on which we have to run Ford Fulkerson algorithm.
 * @param end This is the end vertex for the graph on which we have to run Ford Fulkerson algorithm.
 * @param n This is the number of vertices.
 * @param m This is the number of edges.
 * @param res_adj This is the adjacency matrix for the Residual Graph.
 * @param adj This is the adjacency matrix for the Graph.
 * @return It returns the edges that comprise the minimum cut.
 */
vector<array<int, 2>> minimum_cut(int start, int end, int n, int m, vector<vector<int>> &res_adj, vector<vector<int>> &adj)
{
    vector<array<int, 2>> mincut;

    vector<bool> vis(n);
    // Marking all the vertices that can be visible from the source vertex
    reach_from_source(start, vis, res_adj);
    // Adding the edges between vertices into mincut such that one vertex can be reached from source and another vertex cannot reached from source
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (vis[i] and !vis[j] and adj[i][j])
                mincut.push_back({i, j});

    return mincut;
}
