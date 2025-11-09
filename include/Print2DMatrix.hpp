#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <type_traits>

namespace MatrixPrint
{


constexpr int DEFAULT_WIDTH{-1}, DEFAULT_PRECISION{3};
constexpr size_t DEFAULT_POS_VALUE = std::numeric_limits<size_t>::max();


template <typename T>
int get_number_width(const T value, const int precision = DEFAULT_PRECISION) {
  static_assert(std::is_arithmetic_v<T>, "Type must be numeric.");
  
  int width{0};
  if (value < 0) width++;

  auto absval = std::abs(value);
  int int_part = (absval <= 1) ? 1 : static_cast<int>(std::log10(absval) + 1);
  width += int_part;
  
  if constexpr (std::is_floating_point_v<T>) {
    if (precision > 0) {
      width += precision + 1;
    }
  }

  return width;
}


template <typename T>
int optimal_width(const std::vector<T> &vec, const int precision = DEFAULT_PRECISION) {
  int width_min, width_max;

  if (vec.empty()) return 0;
  auto [min_it, max_it] = std::minmax_element(vec.begin(), vec.end());

  width_min = get_number_width(*min_it, precision);
  width_max = get_number_width(*max_it, precision);

  return std::max(width_max, width_min);
}


template<typename T>
std::ostream & print_boxed_2D(const std::vector<T> &M,
                              const size_t nrows, const size_t ncols,
                              std::ostream &os = std::cout, const bool print_final_rsep = true,
                              int width = DEFAULT_WIDTH, int precision = DEFAULT_PRECISION,
                              size_t r0 = DEFAULT_POS_VALUE, size_t rf = DEFAULT_POS_VALUE,
                              size_t c0 = DEFAULT_POS_VALUE, size_t cf = DEFAULT_POS_VALUE) {
  std::ostringstream oss;

  width = (width == DEFAULT_WIDTH) ? 2 + optimal_width(M, precision) : width;
  r0 = (r0 == DEFAULT_POS_VALUE) ? size_t{} : r0;
  rf = (rf == DEFAULT_POS_VALUE) ? nrows : rf;
  c0 = (c0 == DEFAULT_POS_VALUE) ? size_t{} : c0;
  cf = (cf == DEFAULT_POS_VALUE) ? ncols : cf;
  assert((rf >= r0) && (rf - r0 <= nrows) && (cf >= c0) && (cf - c0 <= ncols));

  size_t padding;
  std::string row_seperator = "+";
  for (size_t j = 0; j < cf - c0; j++) row_seperator += std::string(width, '-') + "+";

  for (size_t r = r0; r < rf; r++) {
    os << row_seperator << "\n";

    for (size_t c = c0; c < cf; c++) {
      if constexpr (std::is_floating_point_v<T>) oss << std::fixed << std::setprecision(precision);
      oss << M[r * ncols + c];
      padding = (width >= oss.tellp()) ? width - oss.tellp() : size_t{};

      os << "|" << std::string(padding / 2, ' ') << oss.str() << std::string(padding - (padding / 2), ' ');
      oss.str(""); oss.clear();
    }
    os << "|" << "\n";
  }

  if (print_final_rsep) os << row_seperator << "\n";
  return os;
}


template<typename T>
std::ostream & print_2D(const std::vector<T> &M,
                        const size_t nrows, const size_t ncols,
                        std::ostream &os = std::cout, std::string_view delim = ",",
                        int width = DEFAULT_WIDTH, int precision = DEFAULT_PRECISION,
                        size_t r0 = DEFAULT_POS_VALUE, size_t rf = DEFAULT_POS_VALUE,
                        size_t c0 = DEFAULT_POS_VALUE, size_t cf = DEFAULT_POS_VALUE) {

  width = (width == DEFAULT_WIDTH) ? 1 + optimal_width(M, precision) : width;
  r0 = (r0 == DEFAULT_POS_VALUE) ? size_t{} : r0;
  rf = (rf == DEFAULT_POS_VALUE) ? nrows : rf;
  c0 = (c0 == DEFAULT_POS_VALUE) ? size_t{} : c0;
  cf = (cf == DEFAULT_POS_VALUE) ? ncols : cf;
  assert((rf >= r0) && (rf - r0 <= nrows) && (cf >= c0) && (cf - c0 <= ncols));

  for (size_t r = r0; r < rf; r++) {
    for (size_t c = c0; c < cf; c++) {
      os << std::right << std::setw(width);
      if constexpr (std::is_floating_point_v<T>) os << std::fixed << std::setprecision(precision);
      os << M[r * ncols + c] << delim;
    }
    os << "\n";
  }
  return os;
}

}