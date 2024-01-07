//
// Copyright (c) 2024 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <array>
#include <iostream>
#include <mph>
#include <string_view>

constexpr auto dummy_policies = [](const auto symbols, const auto span, [[maybe_unused]] auto &&...args) {
  if (not std::size(span)) {
    return -1;
  } else if constexpr (constexpr auto pext_direct = mph::pext_direct<2>{};
                       requires { pext_direct(symbols, span, std::forward<decltype(args)>(args)...); }) {
    return int(pext_direct(symbols, span, std::forward<decltype(args)>(args)...)) - 1;
  } else {
    static_assert([](auto &&) { return false; }(span), "No luck!");
  }
};

int main() {
  using std::literals::operator""sv;

  static constexpr std::array symbols{
      "FBC"sv,
      "SPY"sv,
      "CDC"sv,
  };

  const auto hash = mph::hash{[] { return symbols; }, dummy_policies};

  std::cout << hash(""sv);     // -1
  std::cout << hash("FO"sv);   // -1
  std::cout << hash("FOO"sv);  // -1

  std::cout << hash("FBC"sv);  // 0
  std::cout << hash("SPY"sv);  // 1
  std::cout << hash("CDC"sv);  // 2
}
