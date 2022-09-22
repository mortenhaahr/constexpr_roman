#include <string_view>
#include <array>

// Shamelessly stolen from: https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
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

static constexpr std::array<int, 13> lookupTableIndex = {
    1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
};

// NOTE:
// In previous commit I was using an array of pairs with std::string_view and int.
// That idea was more elegant. However, according to Søren Hansen and https://timsong-cpp.github.io/cppwp/n4618/temp.arg.nontype it is
// std language undefined since:
// "For a non-type template-parameter of reference or pointer type, the value of the constant expression shall not refer to a subobject".
// `std::string_view` is a subobject in this context so you should not be able to access them compile-time.
// Therefore, in order to be standard compliant I had to make a mapping function instead of using an array. The monstrosity is seen below.
// One could look into how e.g. boost::hana does it.
// (This might be the reason why `boost::hana::pair` uses the syntax `hana::first(my_pair)` instead of `my_pair.first`)
// **It now compiles on all toolchains** (where previous commit only GCC worked because of luck)
template<int index>
struct RomanGen {
  static constexpr auto impl() noexcept {
    if constexpr(index == 0)
      return "M";
    else if constexpr(index == 1)
      return "CM";
    else if constexpr(index == 2)
      return "D";
    else if constexpr(index == 3)
      return "CD";
    else if constexpr(index == 4)
      return "C";
    else if constexpr(index == 5)
      return "XC";
    else if constexpr(index == 6)
      return "L";
    else if constexpr(index == 7)
      return "XL";
    else if constexpr(index == 8)
      return "X";
    else if constexpr(index == 9)
      return "IX";
    else if constexpr(index == 10)
      return "V";
    else if constexpr(index == 11)
      return "IV";
    else if constexpr(index == 12)
      return "I";
  }
  static constexpr std::string_view value = impl();
};

template<int remainder, int numeralIndex, const std::string_view &roman>
static constexpr std::string_view toRoman() {
  constexpr auto index = lookupTableIndex[numeralIndex];
  if constexpr(remainder == 0) {
    return roman;
  } else if constexpr(remainder >= index) {
    return toRoman<remainder - index,
                   numeralIndex,
                   cexpr_string_join<roman, RomanGen<numeralIndex>::value>::value>();
  } else {
    return toRoman<remainder, numeralIndex + 1, roman>();
  }
}

static constexpr std::string_view str{""};
template<int number>
static constexpr std::string_view solution() {
  return toRoman<number, 0, str>();
}

#include <iostream>
int main() {
  constexpr auto x = solution<2022>();
  static_assert(x == "MMXXII");
  std::cout << x << std::endl;
}