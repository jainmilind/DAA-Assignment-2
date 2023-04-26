#include "./Ford_fulkerson_max_flow.cpp"

using namespace std;

/**
 * Function to run the Ford Fulkerson and also to get the min_cut
 */
int main()
{
    // Number of Vertices
    int n;
    cin >> n;
    // Number of Edges
    int m;
    cin >> m;
    cout << n << ' ' << m << endl
         << endl;
    // Creating a adjanceny matrix
    vector<vector<int>> adj(n, vector<int>(n, 0));
    map<array<int, 2>, int> edg_to_i;

    for (int i = 0; i < m; ++i)
    {
        int x, y, c;
        cin >> x >> y >> c;
        x--, y--;
        adj[x][y] = c;
        edg_to_i[{x, y}] = i;
    }

    // Source and sink of ford fulkerson
    int start, end;
    cin >> start >> end;
    // As 0 indexed
    start--, end--;

    auto startFord = std::chrono::high_resolution_clock::now();
    // Getting the residual graph
    auto res_adj = ford_fulkerson(start, end, n, m, adj, edg_to_i);
    auto stopFord = std::chrono::high_resolution_clock::now();
    auto fordTime = std::chrono::duration_cast<std::chrono::microseconds>(stopFord - startFord);
    cerr << setprecision(15) << "Time for decomposing is: " << (double)fordTime.count() / 1000 << " ms" << endl;

    int max_flow = 0;
    for (int i = 0; i < n; i++)
    {
        max_flow += adj[0][i] - res_adj[0][i];
    }
    cout << "Maximum Flow: " << max_flow << endl
         << endl;

    cout << "The edges (with flow) of the maximum flow graph are: \n";
    // Edge list printing for easy checking on https://csacademy.com/app/graph_editor/
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (adj[i][j])
                cout << i + 1 << ' ' << j + 1 << ' ' << res_adj[j][i] << endl;

    cout << endl;

    // Getting the minimum cut
    auto min_cut = minimum_cut(start, end, n, m, res_adj, adj);

    cout << "The vertices of the minimum cut are:\n";

    for (auto e : min_cut)
    {
        cout << e[0] << " " << e[1] << endl;
    }

    return 0;
}