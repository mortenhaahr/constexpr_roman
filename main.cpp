#include <boost/hana.hpp>

using namespace boost;
using namespace hana::literals;

constexpr auto lookupTable = boost::hana::make_tuple(
    boost::hana::make_pair(BOOST_HANA_STRING("M"), 1000),
    boost::hana::make_pair(BOOST_HANA_STRING("CM"), 900),
    boost::hana::make_pair(BOOST_HANA_STRING("D"), 500),
    boost::hana::make_pair(BOOST_HANA_STRING("CD"), 400),
    boost::hana::make_pair(BOOST_HANA_STRING("C"), 100),
    boost::hana::make_pair(BOOST_HANA_STRING("XC"), 90),
    boost::hana::make_pair(BOOST_HANA_STRING("L"), 50),
    boost::hana::make_pair(BOOST_HANA_STRING("XL"), 40),
    boost::hana::make_pair(BOOST_HANA_STRING("X"), 10),
    boost::hana::make_pair(BOOST_HANA_STRING("IX"), 9),
    boost::hana::make_pair(BOOST_HANA_STRING("V"), 5),
    boost::hana::make_pair(BOOST_HANA_STRING("IV"), 4),
    boost::hana::make_pair(BOOST_HANA_STRING("I"), 1)
);

template<int remainder, int numeralIndex = 0>
static constexpr auto toRoman() {
  // Weird Hana syntax for retrieving tuple elements...
  constexpr auto first = hana::first(lookupTable[hana::size_t<numeralIndex>{}]);
  constexpr auto second = hana::second(lookupTable[hana::size_t<numeralIndex>{}]);

  if constexpr(remainder == 0) {
    return BOOST_HANA_STRING("");
  } else if constexpr(remainder >= second) {
    return first + toRoman<remainder - second, numeralIndex>();
  } else {
    return toRoman<remainder, numeralIndex + 1>();
  }
}

#define PRINT
#ifdef PRINT
#include <iostream>
#endif

int main() {
  auto x = toRoman<2022>();
  static_assert(x == BOOST_HANA_STRING("MMXXII"), "Not compile-time");
  #ifdef PRINT
  std::cout << x.c_str() << std::endl;
  #endif
}