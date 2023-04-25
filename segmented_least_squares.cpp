#include <bits/stdc++.h>
#include <numeric>
using namespace std;

const int64_t inf = 1e10;

// Function to recursively find the best set of segments
void find_segment(int j, vector<vector<int>> &segments, vector<int> &best_seg)
{
    
    if (j == 0)
        return;

    int i = best_seg[j];
    vector<int> segment;
    for (int x = i; x <= j; ++x)
        segment.push_back(x);
    segments.push_back(segment);

    find_segment(i,segments,best_seg);
};

int main()
{
    // this is number of points
    int n;
    cin >> n;
    vector<array<int64_t, 2>> points(n);
    for (auto &x : points)
        cin >> x[0] >> x[1];

    // Cost of creating a new segment
    int64_t C;
    cin >> C;

    // Sorting the points in ascending order
    sort(points.begin(), points.end());

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

            if (den == 0)
            {
                slope[i][j] = inf;
                intercept[i][j] = -(double)range_x / seg_len;
            }
            else
            {
                slope[i][j] = (double)num / den;
                intercept[i][j] = (double)(range_y - slope[i][j] * range_x) / seg_len;
            }
            // Calculating the intecept

            // Adding errors for all points [i, j] for given line in e[i][j]
            for (int k = i; k <= j; ++k)
            {
                // Error = y - mx - c
                double x = points[k][0];
                double y = points[k][1];
                double coef_x = slope[i][j] == inf ? 1 : slope[i][j];
                double coef_y = slope[i][j] == inf ? 0 : -1;
                double c = intercept[i][j];
                double cur_error =  - coef_y * y + coef_x * x + c;
                e[i][j] += cur_error * cur_error;
            }
        }
    }

    // opt[i] = optimal solution for prefix [0..i]
    vector<double> opt(n, inf);

    // best_seg[i] = where this segment came from.
    vector<int> best_seg(n, -1);
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

    find_segment(n - 1, segments,best_seg);

    cout << "Minimun cost of making segments = " << opt[n - 1] << endl;
    cout << "Number of segments = " << segments.size() << endl;
    for (auto &v : segments)
    {
        // Prints id of points in segment (0 indexed)
        int i = v.front(), j = v.back();

        if(slope[i][j] == inf){
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


