#include <bits/stdc++.h>
using namespace std;

const int64_t inf = LLONG_MAX;
int main() {
    // this is number of points
    int n; cin >> n;
    vector<array<int64_t, 2>> points(n);
    for (auto& x : points)
        cin >> x[0] >> x[1];

    // Cost of creating a new segment
    int64_t C;
    cin >> C;

    // sorting by x value as said in slides
    // ! can remove if input is always sorted on x
    sort(points.begin(), points.end());

    // Prefix sum for x, y, x*y, x*x coordinates (0 indexed) used for finding m and c of lines as described in slide 4 
    vector<int64_t> pre_x(n), pre_y(n), pre_xy(n), pre_xx(n);
    pre_x[0] = points[0][0];
    pre_y[0] = points[0][1];
    pre_xy[0] = points[0][0] * points[0][1];
    pre_xx[0] = points[0][0] * points[0][0];
    for (int i = 1; i < n; ++i) {
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

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            // From slide 4 
            int64_t range_x = pre_x[j] - (i ? pre_x[i - 1] : 0);
            int64_t range_y = pre_y[j] - (i ? pre_y[i - 1] : 0);
            int64_t range_xy = pre_xy[j] - (i ? pre_xy[i - 1] : 0);
            int64_t range_xx = pre_xx[j] - (i ? pre_xx[i - 1] : 0);

            // segment length
            int64_t seg_len = j - i + 1;

            int64_t num = seg_len * range_xy - range_x * range_y;
            int64_t den = seg_len * range_xx - range_x * range_x;

            if (den == 0) {
                slope[i][j] = inf;
            } else {
                slope[i][j] = (double)num / den;
            }

            intercept[i][j] = (double)(range_y - slope[i][j] * range_x) / seg_len;

            // adding errors for all points [i, j] for given line in e[i][j] 
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
    // From slide 11
    vector<double> opt(n, inf);
    // best_seg[i] = where this segment came from. 
    vector<int> best_seg(n, -1);
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < j; ++i) {
            double cur = e[i][j] + C + (i ? opt[i - 1] : 0);
            if (opt[j] > cur) {
                opt[j] = cur;
                best_seg[j] = i;
            }
        }
    }

    // Recovering segments using OPT

    vector<vector<int>> segments;
    // ! Can be converted to iterative easily but i am lazy and recursive == seggs
    function<void(int)> find_segment = [&](int j) {
        if (j == 0) return;

        int i = best_seg[j];
        vector<int> segment;
        for (int x = i; x <= j; ++x)
            segment.push_back(x);
        segments.push_back(segment);

        find_segment(i - 1);
    };
    find_segment(n - 1);

    cout << "Minimun cost of making segments = " << opt[n - 1] << endl; 
    cout << "Number of segments = " << segments.size() << endl;
    for (auto &v : segments) {
        cout << "New segment start: ";
        for (int i : v)
            cout << i << ' ';
        cout << endl;
    }
}
