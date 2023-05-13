/**
 * @file StdC++.h
 * @author shirokuma1101
 * @version 1.1
 * @date 2023-05-14
 *
 * @copyright Copyright (c) 2023 shirokuma1101. All rights reserved.
 * @license MIT License (see LICENSE.txt file)
 */

#pragma once

#include "Utility/Macro.h"

/**
 * @brief C++ Standard Library headers.
 * @note "https://en.cppreference.com/w/cpp/header"
 */

// Concepts library
#if __cplusplus >= MACRO_CPP20
#include <concepts>
#endif

// Coroutines library
#if __cplusplus >= MACRO_CPP20
#include <coroutine>
#endif

// Utilities library
#include <bitset>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <typeinfo>
#include <utility>
#if __cplusplus >= MACRO_CPP11
#include <chrono>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <typeindex>
#endif
#if __cplusplus >= MACRO_CPP17
#include <any>
#include <optional>
#include <variant>
#endif
#if __cplusplus >= MACRO_CPP20
#include <compare>
#include <source_location>
#include <version>
#endif
#if __cplusplus >= MACRO_CPP23
#include <expected>
#endif

// Dynamic memory management
#include <memory>
#include <new>
#if __cplusplus >= MACRO_CPP11
#include <scoped_allocator>
#endif
#if __cplusplus >= MACRO_CPP17
#include <memory_resource>
#endif

// Numeric limits
#include <cfloat>
#include <climits>
#include <limits>
#if __cplusplus >= MACRO_CPP11
#include <cinttypes>
#include <cstdint>
#endif
#if __cplusplus >= MACRO_CPP23
#include <stdfloat>
#endif

// Error handling
#include <cassert>
#include <cerrno>
#include <exception>
#include <stdexcept>
#if __cplusplus >= MACRO_CPP11
#include <system_error>
#endif
#if __cplusplus >= MACRO_CPP23
#include <stacktrace>
#endif

// Strings library
#include <cctype>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <string>
#if __cplusplus >= MACRO_CPP11
#include <cuchar>
#endif
#if __cplusplus >= MACRO_CPP17
#include <charconv>
#include <string_view>
#endif
#if __cplusplus >= MACRO_CPP20
#include <format>
#endif

// Containers library
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#if __cplusplus >= MACRO_CPP11
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#endif
#if __cplusplus >= MACRO_CPP20
#include <span>
#endif
#if __cplusplus >= MACRO_CPP23
#include <flat_map>
#include <flat_set>
#include <mdspan>
#endif

// Iterators library
#include <iterator>

// Ranges library
#if __cplusplus >= MACRO_CPP20
#include <ranges>
#endif
#if __cplusplus >= MACRO_CPP23
#include <generator>
#endif

// Algorithms library
#include <algorithm>
#if __cplusplus >= MACRO_CPP17
#include <execution>
#endif

// Numerics library
#include <cmath>
#include <complex>
#include <numeric>
#include <valarray>
#if __cplusplus >= MACRO_CPP11
#include <cfenv>
#include <random>
#include <ratio>
#endif
#if __cplusplus >= MACRO_CPP20
#include <bit>
#include <numbers>
#endif

// Localization library
#include <clocale>
#include <locale>
#if (__cplusplus >= MACRO_CPP11) && (__cplusplus < MACRO_CPP17)
#include <codecvt> // deprecated:C++17
#endif

// Input / output library
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <streambuf>
#if __cplusplus < MACRO_CPP98
#include <strstream> // deprecated:C++98
#endif
#if __cplusplus >= MACRO_CPP20
#include <syncstream>
#endif
#if __cplusplus >= MACRO_CPP23
#include <print>
#include <spanstream>
#endif

// Filesystem library
#if __cplusplus >= MACRO_CPP17
#include <filesystem>
#endif

// Regular Expressions library
#if __cplusplus >= MACRO_CPP11
#include <regex>
#endif

// Atomic Operations library
#if __cplusplus >= MACRO_CPP11
#include <atomic>
#endif

// Thread support library
#if __cplusplus >= MACRO_CPP11
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#endif
#if __cplusplus >= MACRO_CPP14
#include <shared_mutex>
#endif
#if __cplusplus >= MACRO_CPP20
#include <barrier>
#include <latch>
#include <semaphore>
#include <stop_token>
#endif


// C compatibility headers
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#if __cplusplus >= MACRO_CPP11
#include <cfenv>
#include <cinttypes>
#include <cstdint>
#include <cuchar>
#endif

// Empty C headers
#if (__cplusplus >= MACRO_CPP11) && (__cplusplus < MACRO_CPP17)
#include <ccomplex> // deprecated:C++17/removed:C++20
#include <complex.h>
#include <ctgmath>  // deprecated:C++17/removed:C++20
#include <tgmath.h>
#endif

// Special C compatibility headers
#if __cplusplus >= MACRO_CPP23
#include <stdatomic.h>
#endif

// Meaningless C headers
#include <iso646.h>
#if __cplusplus >= MACRO_CPP11
//#include <stdalign.h> // 環境による?
#include <stdbool.h>
#if __cplusplus < MACRO_CPP17
#include <cstdalign> // deprecated:C++17/removed:C++20
#include <cstdbool>  // deprecated:C++17/removed:C++20
#endif
#endif
#if __cplusplus < MACRO_CPP20
#include <ciso646> // removed:C++20
#endif
