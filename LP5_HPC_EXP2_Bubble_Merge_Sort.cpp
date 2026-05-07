#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// ---------- Sequential Bubble Sort ----------
void sequentialBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ---------- Parallel Bubble Sort (Odd-Even Sort) ----------
void parallelBubbleSort(vector<int>& arr) {

    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        if (i % 2 == 0) {

            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {

                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }

        // Odd phase
        else {

            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2) {

                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// ---------- Merge Function ----------
void merge(vector<int>& arr, int left, int mid, int right) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {

        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        }
        else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

// ---------- Sequential Merge Sort ----------
void sequentialMergeSort(vector<int>& arr, int left, int right) {

    if (left < right) {

        int mid = (left + right) / 2;

        sequentialMergeSort(arr, left, mid);
        sequentialMergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// ---------- Parallel Merge Sort ----------
void parallelMergeSort(vector<int>& arr, int left, int right) {

    if (left < right) {

        int mid = (left + right) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, left, mid);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

// ---------- Print Array ----------
void printArray(vector<int>& arr) {

    for (int x : arr) {
        cout << x << " ";
    }

    cout << endl;
}

// ---------- Main ----------
int main() {

    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    double start, end;

    // Sequential Bubble Sort
    start = omp_get_wtime();

    sequentialBubbleSort(arr1);

    end = omp_get_wtime();

    cout << "\nSequential Bubble Sort:\n";
    printArray(arr1);

    cout << "Execution Time: "
         << end - start
         << " seconds\n";

    // Parallel Bubble Sort
    start = omp_get_wtime();

    parallelBubbleSort(arr2);

    end = omp_get_wtime();

    cout << "\nParallel Bubble Sort:\n";
    printArray(arr2);

    cout << "Execution Time: "
         << end - start
         << " seconds\n";

    // Sequential Merge Sort
    start = omp_get_wtime();

    sequentialMergeSort(arr3, 0, n - 1);

    end = omp_get_wtime();

    cout << "\nSequential Merge Sort:\n";
    printArray(arr3);

    cout << "Execution Time: "
         << end - start
         << " seconds\n";

    // Parallel Merge Sort
    start = omp_get_wtime();

    parallelMergeSort(arr4, 0, n - 1);

    end = omp_get_wtime();

    cout << "\nParallel Merge Sort:\n";
    printArray(arr4);

    cout << "Execution Time: "
         << end - start
         << " seconds\n";

    return 0;
}