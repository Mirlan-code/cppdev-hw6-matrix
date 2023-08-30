#include "matrix.h"

#include <iostream>

using namespace std;

int main (int, char **) {
    Matrix<int, 0> matrix;
    int n = 10;
    for (int i = 0; i < n; i++) {
        matrix[i][i] = i;
        matrix[i][n - i - 1] = n - i - 1;
    }

    for (int i = 1; i < n - 1; i++) {
        for (int j = 1; j < n - 1; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Size: " << matrix.size() << endl;

    for(auto c : matrix) {
        size_t x;
        size_t y;
        int v;
        tie(x, y, v) = c;
        cout << x << " " << y << ": " << v << endl;
    }

    return 0;
}
