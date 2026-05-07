#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>

using namespace std;

int main() {

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int sum = 0;
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    double start, end;

    // Start time
    start = omp_get_wtime();

    // Parallel Reduction
    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {

        sum += arr[i];

        if (arr[i] < min_val)
            min_val = arr[i];

        if (arr[i] > max_val)
            max_val = arr[i];
    }

    double average = (double)sum / n;

    // End time
    end = omp_get_wtime();

    // Output
    cout << "\nSum = " << sum << endl;

    cout << "Minimum = " << min_val << endl;

    cout << "Maximum = " << max_val << endl;

    cout << "Average = " << average << endl;

    cout << "Execution Time = "
         << end - start
         << " seconds\n";

    return 0;
}