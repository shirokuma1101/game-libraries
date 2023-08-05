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
 * @note "https://en.cppreference.com/w/cpp/header" reference. (2023-05-21)
 */

// Concepts library
#if CPP_VERSION >= MACRO_CPP20
#include <concepts>
#endif

// Coroutines library
#if CPP_VERSION >= MACRO_CPP20
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
#if CPP_VERSION >= MACRO_CPP11
#include <chrono>
#include <initializer_list>
#include <tuple>
#include <type_traits>
#include <typeindex>
#endif
#if CPP_VERSION >= MACRO_CPP17
#include <any>
#include <optional>
#include <variant>
#endif
#if CPP_VERSION >= MACRO_CPP20
#include <compare>
#include <source_location>
#include <version>
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <expected>
#endif

// Dynamic memory management
#include <memory>
#include <new>
#if CPP_VERSION >= MACRO_CPP11
#include <scoped_allocator>
#endif
#if CPP_VERSION >= MACRO_CPP17
#include <memory_resource>
#endif

// Numeric limits
#include <cfloat>
#include <climits>
#include <limits>
#if CPP_VERSION >= MACRO_CPP11
#include <cinttypes>
#include <cstdint>
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <stdfloat>
#endif

// Error handling
#include <cassert>
#include <cerrno>
#include <exception>
#include <stdexcept>
#if CPP_VERSION >= MACRO_CPP11
#include <system_error>
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <stacktrace>
#endif

// Strings library
#include <cctype>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <string>
#if CPP_VERSION >= MACRO_CPP11
#include <cuchar>
#endif
#if CPP_VERSION >= MACRO_CPP17
#include <charconv>
#include <string_view>
#endif
#if CPP_VERSION >= MACRO_CPP20
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
#if CPP_VERSION >= MACRO_CPP11
#include <array>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>
#endif
#if CPP_VERSION >= MACRO_CPP20
#include <span>
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <flat_map>
#include <flat_set>
#include <mdspan>
#endif

// Iterators library
#include <iterator>

// Ranges library
#if CPP_VERSION >= MACRO_CPP20
#include <ranges>
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <generator>
#endif

// Algorithms library
#include <algorithm>
#if CPP_VERSION >= MACRO_CPP17
#include <execution>
#endif

// Numerics library
#include <cmath>
#include <complex>
#include <numeric>
#include <valarray>
#if CPP_VERSION >= MACRO_CPP11
#include <cfenv>
#include <random>
#include <ratio>
#endif
#if CPP_VERSION >= MACRO_CPP20
#include <bit>
#include <numbers>
#endif

// Localization library
#include <clocale>
#include <locale>
#if CPP_VERSION >= MACRO_CPP11 && CPP_VERSION < MACRO_CPP17
#include <codecvt> // deprecated in C++17
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
#if CPP_VERSION < MACRO_CPP98
#include <strstream> // deprecated in C++98
#endif
#if CPP_VERSION >= MACRO_CPP20
#include <syncstream>
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <print>
#include <spanstream>
#endif

// Filesystem library
#if CPP_VERSION >= MACRO_CPP17
#include <filesystem>
#endif

// Regular Expressions library
#if CPP_VERSION >= MACRO_CPP11
#include <regex>
#endif

// Atomic Operations library
#if CPP_VERSION >= MACRO_CPP11
#include <atomic>
#endif

// Thread support library
#if CPP_VERSION >= MACRO_CPP11
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#endif
#if CPP_VERSION >= MACRO_CPP14
#include <shared_mutex>
#endif
#if CPP_VERSION >= MACRO_CPP20
#include <barrier>
#include <latch>
#include <semaphore>
#include <stop_token>
#endif

// C compatibility headers
#include <assert.h> // Behaves same as <cassert>
#include <ctype.h>  // Behaves as if each name from <cctype> is placed in global namespace
#include <errno.h>  // Behaves same as <cerrno>
#include <float.h>  // Behaves same as <cfloat>
#include <limits.h> // Behaves same as <climits>
#include <locale.h> // Behaves as if each name from <clocale> is placed in global namespace
#include <math.h>   // Behaves as if each name from <cmath> is placed in global namespace, except for names of mathematical special functions
#include <setjmp.h> // Behaves as if each name from <csetjmp> is placed in global namespace
#include <signal.h> // Behaves as if each name from <csignal> is placed in global namespace
#include <stdarg.h> // Behaves as if each name from <cstdarg> is placed in global namespace
#include <stddef.h> // Behaves as if each name from <cstddef> is placed in global namespace, except for names of std::byte and related functions
#include <stdio.h>  // Behaves as if each name from <cstdio> is placed in global namespace
#include <stdlib.h> // Behaves as if each name from <cstdlib> is placed in global namespace
#include <string.h> // Behaves as if each name from <cstring> is placed in global namespace
#include <time.h>   // Behaves as if each name from <ctime> is placed in global namespace
#include <wchar.h>  // Behaves as if each name from <cwchar> is placed in global namespace
#include <wctype.h> // Behaves as if each name from <cwctype> is placed in global namespace
#if CPP_VERSION >= MACRO_CPP11
#include <fenv.h>     // Behaves as if each name from <cfenv> is placed in global namespace
#include <inttypes.h> // Behaves as if each name from <cinttypes> is placed in global namespace
#include <stdint.h>   // Behaves as if each name from <cstdint> is placed in global namespace
#include <uchar.h>    // Behaves as if each name from <cuchar> is placed in global namespace
#endif

// Special C compatibility headers
#if CPP_VERSION >= MACRO_CPP11
//#include <complex.h>
//#include <tgmath.h>
#endif
#if CPP_VERSION >= MACRO_CPP11 && CPP_VERSION < MACRO_CPP17
#include <ccomplex> // deprecated in C++17 / removed in C++20
#include <ctgmath> // deprecated in C++17 / removed in C++20
#endif
#if CPP_VERSION >= MACRO_CPP23
#include <stdatomic.h>
#endif

// Meaningless C headers
#include <iso646.h>
#if CPP_VERSION >= MACRO_CPP11
//#include <stdalign.h>
//#include <stdbool.h>
#endif
#if CPP_VERSION >= MACRO_CPP11 && CPP_VERSION < MACRO_CPP17
#include <cstdalign> // deprecated in C++17 / removed in C++20
#include <cstdbool> // deprecated in C++17 / removed in C++20
#endif
#if CPP_VERSION < MACRO_CPP20
#include <ciso646> // removed in C++20
#endif
