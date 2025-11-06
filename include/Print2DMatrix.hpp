#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <type_traits>

namespace MatrixPrint
{

constexpr int DEFAULT_PRECISION{3};


template <typename T>
int get_number_width(T value, int precision = DEFAULT_PRECISION) {
  static_assert(std::is_arithmetic_v<T>, "Type must be numeric.");
  if constexpr (std::is_integral_v<T>) precision = 0;

  int width{0};
  if (value < 0) width++;

  auto absval = std::abs(value);
  int int_part = (absval <= 1) ? 1 : static_cast<int>(std::log10(absval) + 1);
  width += int_part;
  
  if (precision > 0) width += precision + 1;
  return width;
}


template <typename T>
int optimal_width(const std::vector<T> &vec, int precision = DEFAULT_PRECISION) {
  int width_min, width_max;

  if (vec.empty()) return 0;
  auto [min_it, max_it] = std::minmax_element(vec.begin(), vec.end());

  width_min = get_number_width(*min_it, precision);
  width_max = get_number_width(*max_it, precision);

  return std::max(width_max, width_min);
}


template<typename T>
std::ostream & print_boxed_2D(const std::vector<T> &M,
                              size_t nrows, size_t ncols,
                              std::ostream &os = std::cout, bool print_final_rsep = true,
                              int width = -1, int precision = DEFAULT_PRECISION,
                              int r0 = -1, int rf = -1, int c0 = -1, int cf = -1) {
  int npads{};
  std::ostringstream oss;
  
  if (width < 0) width = 2 + optimal_width(M, precision);
  if (r0 < 0) r0 = 0;
  if (rf < 0) rf = nrows;
  if (c0 < 0) c0 = 0;
  if (cf < 0) cf = ncols;
  
  std::string row_seperator = "+";
  for (int j = 0; j < cf - c0; j++) row_seperator += std::string(width, '-') + "+";


  for (int r = r0; r < rf; r++) {
    os << row_seperator << "\n";

    for (int c = c0; c < cf; c++) {
      if constexpr (std::is_floating_point_v<T>) oss << std::fixed << std::setprecision(precision);
      oss << M[r * ncols + c];
      npads = width - oss.tellp();

      os << "|" << std::string(npads / 2, ' ') << oss.str() << std::string(npads - (npads / 2), ' ');
      oss.str(""); oss.clear();
    }
    os << "|" << "\n";
  }

  if (print_final_rsep) os << row_seperator << "\n";
  return os;
}


template<typename T>
std::ostream & print_2D(const std::vector<T> &M,
                        size_t nrows, size_t ncols,
                        std::ostream &os = std::cout, std::string_view delim = ",",
                        int width = -1, int precision = DEFAULT_PRECISION,
                        int r0 = -1, int rf = -1, int c0 = -1, int cf = -1) {

  if (width < 0) width = 1 + optimal_width(M, precision);
  if (r0 < 0) r0 = 0;
  if (rf < 0) rf = nrows;
  if (c0 < 0) c0 = 0;
  if (cf < 0) cf = ncols;

  for (int r = r0; r < rf; r++) {
    for (int c = c0; c < cf; c++) {
      os << std::right << std::setw(width);
      if constexpr (std::is_floating_point_v<T>) os << std::fixed << std::setprecision(precision);
      os << M[r * ncols + c] << delim;
    }
    os << "\n";
  }
  return os;
}

}