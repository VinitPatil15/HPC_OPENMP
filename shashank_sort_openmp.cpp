#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include <chrono>

using namespace std::chrono;

// Sequential Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
            }
        }
    }
}

// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int n) {
    #pragma omp parallel
    {
        for (int i = 0; i < n-1; i++) {
            #pragma omp for
            for (int j = 0; j < n-i-1; j++) {
                if (arr[j] > arr[j+1]) {
                    std::swap(arr[j], arr[j+1]);
                }
            }
        }
    }
}

// Merge function for merge sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Sequential Merge Sort
void mergeSort(int arr[], int l, int r) {
    if (l >= r) return;

    int m = l + (r - l) / 2;
    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);
    merge(arr, l, m, r);
}

// Parallel Merge Sort
void parallelMergeSort(int arr[], int l, int r) {
    if (l >= r) return;

    int m = l + (r - l) / 2;
    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(arr, l, m);
        #pragma omp section
        parallelMergeSort(arr, m + 1, r);
    }
    merge(arr, l, m, r);
}

// Print array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int SIZE = 900;
    int arr1[SIZE], arr2[SIZE];

    // Initialize arrays with random values
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        arr1[i] = rand() % 100;
        arr2[i] = arr1[i];
    }

    std::cout << "Original Array: ";
    printArray(arr1, SIZE);

    // Sequential Bubble Sort
    auto start = high_resolution_clock::now();
    bubbleSort(arr1, SIZE);
    auto stop = high_resolution_clock::now();
    std::cout << "Sequential Bubble Sort: ";
    printArray(arr1, SIZE);
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "Time taken by your algorithm: " << duration.count() << " milliseconds" << std::endl;

    // Parallel Bubble Sort
    parallelBubbleSort(arr2, SIZE);
    std::cout << "Parallel Bubble Sort: ";
    printArray(arr2, SIZE);


    // Reset arrays
    for (int i = 0; i < SIZE; i++) {
        arr1[i] = arr2[i] = rand() % 100;
    }

    std::cout << "Original Array: ";
    printArray(arr1, SIZE);

    // Sequential Merge Sort
    mergeSort(arr1, 0, SIZE - 1);
    std::cout << "Sequential Merge Sort: ";
    printArray(arr1, SIZE);

    // Parallel Merge Sort
    parallelMergeSort(arr2, 0, SIZE - 1);
    std::cout << "Parallel Merge Sort: ";
    printArray(arr2, SIZE);

    return 0;
}
