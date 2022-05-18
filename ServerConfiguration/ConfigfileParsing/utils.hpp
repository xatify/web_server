#ifndef __UTILS_HPP__
#define __UTILS_HPP__

struct is_digit {
	bool operator () (char c) { return std::isdigit (c); }
};

template<class InputIterator, class UnaryPredicate>
bool all_of_ (InputIterator first, InputIterator last, UnaryPredicate pred) {
  while (first!=last) {
	if (!pred(*first)) return false;
	++first;
  }
  return true;
}

#endif