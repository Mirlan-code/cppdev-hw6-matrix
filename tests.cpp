#include "matrix.h"
#include <gtest/gtest.h>

using namespace std;

TEST(TestsMatrix, TestBase) {
    Matrix<int, -1> matrix;
    ASSERT_EQ(matrix.size(), 0); // все ячейки свободны
    auto a = matrix[0][0];
    ASSERT_EQ(a, -1);
    ASSERT_EQ(matrix.size(), 0);
    matrix[100][100] = 314;
    ASSERT_EQ(matrix[100][100], 314);
    ASSERT_EQ(matrix.size(), 1);

    int total = 0;
    for(auto c : matrix) {
        total++;
        size_t x;
        size_t y;
        int v;
        tie(x, y, v) = c;
        ASSERT_EQ(x, 100);
        ASSERT_EQ(y, 100);
        ASSERT_EQ(v, 314);
    }

    ASSERT_EQ(total, 1);
}