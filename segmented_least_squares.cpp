#include <bits/stdc++.h>
#include <numeric>
using namespace std;

const long double inf = 1e25;
/**
 * Function to recursively find the best set of segments
 * @param j This is the index of the last point for which segements are need to be found.
 * @param segments This stores all the segments
 * @param best_seg This stores the starting index of the point which gives the least penalty ending at every point
 */
void find_segment(int j, vector<vector<int>> &segments, vector<int> &best_seg)
{

    if (j <= 0)
        return;

    int i = best_seg[j];
    vector<int> segment;
    for (int x = i; x <= j; ++x)
        segment.push_back(x);
    segments.push_back(segment);

    find_segment(i - 1, segments, best_seg);
};
/**
 * In main first we find the slope between every points and their corresponding intercepts and store them in slope and intercept vectors. Then we find the error between every two points and store them in e. Then we use opt to store minimum penalty such that opt[j] stores the errors till jth point. Then we best_seg which contains the starting index of where the index came from. Then we use segments to store all the segments.
 */
int main()
{
    // This is number of points
    int n;
    cin >> n;
    cout << n << endl;
    vector<array<int64_t, 2>> points(n);
    for (auto &x : points){
        cin >> x[0] >> x[1];
        cout << x[0] << " " << x[1] << endl;
    }

    // Cost of creating a new segment
    int64_t C;
    cin >> C;

    // Sorting the points in ascending order
    sort(points.begin(), points.end());
    // Start of timer
    auto startSegmented = std::chrono::high_resolution_clock::now();
    // Prefix sum for x, y, x*y, x*x coordinates (0 indexed) used for finding m and c of lines
    vector<int64_t> pre_x(n), pre_y(n), pre_xy(n), pre_xx(n);
    pre_x[0] = points[0][0];
    pre_y[0] = points[0][1];
    pre_xy[0] = points[0][0] * points[0][1];
    pre_xx[0] = points[0][0] * points[0][0];
    for (int i = 1; i < n; ++i)
    {
        pre_x[i] = pre_x[i - 1] + points[i][0];
        pre_y[i] = pre_y[i - 1] + points[i][1];
        pre_xy[i] = pre_xy[i - 1] + points[i][0] * points[i][1];
        pre_xx[i] = pre_xx[i - 1] + points[i][0] * points[i][0];
    }

    // e[i][j] = minimum error of segments best fit for points i to j
    vector<vector<double>> e(n, vector<double>(n));
    // slope[i][j] = slope of segments best fit for points i to j
    vector<vector<double>> slope(n, vector<double>(n));
    // intercept[i][j] = intercept of segments best fit for points i to j
    vector<vector<double>> intercept(n, vector<double>(n));

    // Looping for all set of points
    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            // Finding all the ranges of x, y, x*y, x*x
            int64_t range_x = pre_x[j] - (i ? pre_x[i - 1] : 0);
            int64_t range_y = pre_y[j] - (i ? pre_y[i - 1] : 0);
            int64_t range_xy = pre_xy[j] - (i ? pre_xy[i - 1] : 0);
            int64_t range_xx = pre_xx[j] - (i ? pre_xx[i - 1] : 0);

            // Segment length of current iteration
            int64_t seg_len = j - i + 1;
            // Finding the numerator and denominator for calculating the slope
            int64_t num = seg_len * range_xy - range_x * range_y;
            int64_t den = seg_len * range_xx - range_x * range_x;
            // Calculating the slope and the intecept
                slope[i][j] = (double)num / (double)den;
                intercept[i][j] = (double)(range_y - slope[i][j] * range_x) / (double)seg_len;

            // Adding errors for all points [i, j] for given line in e[i][j]
            for (int k = i; k <= j; ++k) {
                // Error = y - mx - c
                double x = points[k][0];
                double y = points[k][1];
                double m = slope[i][j];
                double c = intercept[i][j];
                double cur_error = y - m * x - c;
                e[i][j] += cur_error * cur_error;
            }
        }
    }

    // opt[i] = optimal solution for prefix [0..i]
    vector<double> opt(n, inf);

    // best_seg[i] = where this segment came from.
    vector<int> best_seg(n, 0);
    for (int j = 0; j < n; ++j)
    {
        for (int i = 0; i < j; ++i)
        {
            double cur = e[i][j] + C + (i ? opt[i - 1] : 0);
            if (opt[j] > cur)
            {
                opt[j] = cur;
                best_seg[j] = i;
            }
        }
    }

    // Recovering segments using OPT
    vector<vector<int>> segments;

    find_segment(n - 1, segments, best_seg);
    // End of timer
    auto stopSegmented = std::chrono::high_resolution_clock::now();
    auto segmentedTime = std::chrono::duration_cast<std::chrono::microseconds>(stopSegmented - startSegmented);
    cerr << setprecision(15) << "Time for getting best segments is: " << (double)segmentedTime.count() / 1000 << " ms" << endl;

    cerr << "Minimun cost = " << opt[n - 1] << endl;
    cerr << "Number of segments = " << segments.size() << endl;
    cout << segments.size() << endl;

    for (auto &v : segments)
    {
        // Prints id of points in segment (0 indexed)
        int i = v.front(), j = v.back();

        if (slope[i][j] == inf)
        {
            double line_x = -intercept[i][j];
            cout << line_x << ' ' << points[i][1] << ' ' << line_x << ' ' << points[j][1] << endl;
            continue;
        }

        double line_x1 = points[i][0], line_x2 = points[j][0];
        double line_y1 = slope[i][j] * line_x1 + intercept[i][j];
        double line_y2 = slope[i][j] * line_x2 + intercept[i][j];

        cout << line_x1 << ' ' << line_y1 << ' ' << line_x2 << ' ' << line_y2 << endl;
    }
}
