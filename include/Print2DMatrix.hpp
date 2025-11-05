#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <type_traits>
#include <cmath>
#include <limits>

inline constexpr size_t DEFAULT_WIDTH{0};
inline constexpr int DEFAULT_PRECISION{3};


template <typename T>
size_t optimal_width(T value, int precision = DEFAULT_PRECISION) {
  size_t opt_width{2};
  size_t n = value < 0 ? static_cast<unsigned long long>(-value) : static_cast<unsigned long long>(value);

  if (value == 0) {
    opt_width++;

  } else {
    while (n > 0) {
      n /= 10;
      opt_width++;
    }
  }

  if constexpr(std::is_integral_v<T>) return opt_width;
  else if (precision == 0) return opt_width;
  else return opt_width + precision + 1;
}


std::string get_rowseparator(size_t ncols, size_t width) {
  std::string row_seperator = "+";
  // Build a row separator like +----------+----------+...
  for (size_t j = 0; j < ncols; j++) row_seperator += std::string(width, '-') + "+";

  return "\n" + row_seperator + "\n";
}

template<typename T>
std::string formatCell(T value, size_t width, int precision) {
  std::ostringstream oss;
  if constexpr (std::is_floating_point_v<T>) {
    oss << std::fixed << std::setprecision(precision) << value;
  } else {
    oss << value;
  }

  std::string str = oss.str();

  if (width < str.size()) return str;  // too long for the box
    
  size_t pad = width - str.size();
  size_t left = pad / 2;
  size_t right = pad - left;

  return std::string(left, ' ') + str + std::string(right, ' ');
}

template<typename T>
void print2term(const std::vector<T> &M, size_t nrows, size_t ncols, std::ostream &os = std::cout,
  bool print_final_rsep = true, size_t width = DEFAULT_WIDTH, int precision = DEFAULT_PRECISION) {
  
  if (width == DEFAULT_WIDTH) {
    T max_val = *std::max_element(M.begin(), M.end());
    width = optimal_width(max_val, precision);
  }
  
  std::string row_seperator = get_rowseparator(ncols, width);

  for (size_t i = 0; i < nrows; i++) {
    os << row_seperator;

    for (size_t j = 0; j < ncols; j++) {
      os << "|";
      os << formatCell(M[i * ncols + j], width, precision);
    }
    os << "|";
  }
  if (print_final_rsep) os << row_seperator;
}


template<typename T>
void print_corner(const std::vector<T> &M, size_t nrows, size_t ncols, size_t cornersize = 4, std::ostream &os = std::cout,
  bool print_final_rsep = true, size_t width = DEFAULT_WIDTH, int precision = DEFAULT_PRECISION) {
    std::size_t nrows_small = std::min(cornersize, nrows);
    std::size_t ncols_small = nrows > 0 ? std::min(cornersize, ncols) : 0;

    if (width == DEFAULT_WIDTH) {
      T max_val = *std::max_element(M.begin(), M.end());
      width = optimal_width(max_val, precision);
    }
    
    std::string row_seperator = get_rowseparator(ncols_small, width);
    
    for (size_t i = 0; i < nrows_small; i++) {
      os << row_seperator;

      for (size_t j = 0; j < ncols_small; j++) {
        os << "|";
        os << formatCell(M[i * ncols_small + j], width, precision);
      }
      os << "|";
    }
    if (print_final_rsep) os << row_seperator;
}


template<typename T>
void print2file(const std::vector<T> &M, size_t nrows, size_t ncols, std::ostream &os = std::cout,
  int precision = DEFAULT_PRECISION, std::string delim = ", ") {
  for (size_t i = 0; i < nrows; i++) {
    os << "\n";
    for (size_t j = 0; j < ncols; j++) {
      os << std::fixed << std::setprecision(precision) << M[i * ncols + j] << delim;
    }
  }
}

/* TEST FOR FUNCTIONS

int main() {
    std::vector<double> mat = { 3.14159, 2.71828, 1.41421, 0.57721, -1.0, 42.0};
    print2DMatrix<double>(mat, size_t{3}, size_t{2});

    // std::vector<int> mat = { 3, 2, 1, 0, -1, 42};
    // print2DMatrix<int>(mat, size_t{3}, size_t{2}, 10, 2);

    return 0;
}

*/
