#include <string>
#include <utility>
#include <array>

template<const std::string_view &... Strs>
struct cexpr_string_join {
  // Join all strings into a single std::array of chars
  static constexpr auto impl() noexcept {
    constexpr std::size_t len = (Strs.size() + ... + 0);
    std::array<char, len + 1> arr{};
    auto append = [i = 0, &arr](auto const &s) mutable {
      for (auto c : s) arr[i++] = c;
    };
    (append(Strs), ...);
    arr[len] = 0;
    return arr;
  }
  // Give the joined string static storage
  static constexpr auto arr = impl();
  // View as a std::string_view
  static constexpr std::string_view value{arr.data(), arr.size() - 1};
};

constexpr std::array<const std::pair<const std::string_view, int>, 13> lookupTable{{
                                                                                       {"M", 1000},
                                                                                       {"CM", 900},
                                                                                       {"D", 500},
                                                                                       {"CD", 400},
                                                                                       {"C", 100},
                                                                                       {"XC", 90},
                                                                                       {"L", 50},
                                                                                       {"XL", 40},
                                                                                       {"X", 10},
                                                                                       {"IX", 9},
                                                                                       {"V", 5},
                                                                                       {"IV", 4},
                                                                                       {"I", 1}
                                                                                   }};



template<int remainder, int numeralIndex, const std::string_view & roman>
static constexpr std::string_view toRomanRec() {
  if constexpr(remainder == 0) {
    return roman;
  }
  constexpr auto p = lookupTable[numeralIndex];
  if constexpr(remainder >= p.second) {
    return toRomanRec<remainder - p.second, numeralIndex, cexpr_string_join<roman, lookupTable[numeralIndex].first>::value>();
  }
//  else {
//    return toRomanRec<remainder, numeralIndex + 1, roman>();
//  }
  return roman;
}

static constexpr std::string_view str = "";
template<int number>
static constexpr std::string_view solution() {
  return toRomanRec<number, 0, str>();
//  auto x =  toRomanRec(number, 0, "");
//  return x;
}

template<int i>
static constexpr auto test(){
  return lookupTable[i].first;
}

#include <iostream>
int main() {
  auto x = solution<1000>();
  std::cout << x << std::endl;
  auto y = 2;
}