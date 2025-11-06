#include "Print2DMatrix.hpp"

int main() {
    std::vector<double> mat1 = { 3.14159, 2.71828, 1.41421, 0.57721, -1.0, 42.0};
    std::vector<int> mat2 = { 3, 2, 1, 0, -1, 42};

    std::cout << "\nBoxed, double matrix:\n";
    MatrixPrint::print_boxed_2D(mat1, size_t{3}, size_t{2});
    std::cout << "\nRegular, double matrix:\n";
    MatrixPrint::print_2D(mat1, size_t{3}, size_t{2}, std::cout, ",", -1, 3, -1, -1, -1, -1, 2, 2);
    std::cout << "\nBoxed, double matrix:\n";
    MatrixPrint::print_2D(mat2, size_t{3}, size_t{2});

    return 0;
}