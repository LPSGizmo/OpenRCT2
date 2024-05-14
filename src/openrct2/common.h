/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#undef M_PI

// Ignore isatty warning on WIN32
#ifndef _CRT_NONSTDC_NO_WARNINGS
#    define _CRT_NONSTDC_NO_WARNINGS
#endif

#ifdef _MSC_VER
#    include <ctime>
#endif

#include "Diagnostic.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

using colour_t = uint8_t;

// Gets the name of a symbol as a C string
#define nameof(symbol) #symbol

#define OPENRCT2_MASTER_SERVER_URL "https://servers.openrct2.io"

// Time (represented as number of 100-nanosecond intervals since 0001-01-01T00:00:00Z)
using datetime64 = uint64_t;

constexpr datetime64 DATETIME64_MIN = 0;

// Money is stored as a multiple of 0.10.
using money16 = fixed16_1dp;
using money32 = fixed32_1dp;
using money64 = fixed64_1dp;

// For a user defined floating point literal, the parameter type must be a
// `long double` which is problematic on ppc64el, as the architecture uses a
// pair of `doubles` to represent that type. This cannot be converted to a
// `constexpr`. As a workaround, statically cast the `long double` down to a
// `double`. All of the uses of _GBP constants fit just fine, and if anyone
// really tries to use a gigantic constant that can't fit in a double, they are
// probably going to be breaking other things anyways.
// For more details, see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=26374
constexpr money64 operator"" _GBP(long double money) noexcept
{
    return static_cast<double>(money) * 10;
}

constexpr money64 ToMoney64FromGBP(int32_t money) noexcept
{
    return money * 10;
}

constexpr money64 ToMoney64FromGBP(int64_t money) noexcept
{
    return money * 10;
}

constexpr money64 ToMoney64FromGBP(double money) noexcept
{
    return money * 10;
}

constexpr money16 kMoney16Undefined = static_cast<money16>(static_cast<uint16_t>(0xFFFF));
constexpr money32 kMoney32Undefined = static_cast<money32>(0x80000000);
constexpr money64 kMoney64Undefined = static_cast<money64>(0x8000000000000000);

constexpr money16 ToMoney16(money64 value)
{
    return value == kMoney64Undefined ? kMoney16Undefined : value;
}

constexpr money64 ToMoney64(money32 value)
{
    return value == kMoney32Undefined ? kMoney64Undefined : value;
}

constexpr money64 ToMoney64(money16 value)
{
    return value == kMoney16Undefined ? kMoney64Undefined : value;
}

using StringId = uint16_t;

#define abstract = 0

#if defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#    define OPENRCT2_X86
#elif defined(_MSC_VER) && (_MSC_VER >= 1500) && (defined(_M_X64) || defined(_M_IX86)) // VS2008
#    define OPENRCT2_X86
#endif

#if defined(__i386__) || defined(_M_IX86)
#    define PLATFORM_X86
#endif

#if defined(__LP64__) || defined(_WIN64)
#    define PLATFORM_64BIT
#else
#    define PLATFORM_32BIT
#endif

// C99's restrict keywords guarantees the pointer in question, for the whole of its lifetime,
// will be the only way to access a given memory region. In other words: there is no other pointer
// aliasing the same memory area. Using it lets compiler generate better code. If your compiler
// does not support it, feel free to drop it, at some performance hit.
#ifdef _MSC_VER
#    define RESTRICT __restrict
#else
#    define RESTRICT __restrict__
#endif

#define assert_struct_size(x, y) static_assert(sizeof(x) == (y), "Improper struct size")

#ifdef PLATFORM_X86
#    ifndef FASTCALL
#        ifdef __GNUC__
#            define FASTCALL __attribute__((fastcall))
#        elif defined(_MSC_VER)
#            define FASTCALL __fastcall
#        else
#            pragma message "Not using fastcall calling convention, please check your compiler support"
#            define FASTCALL
#        endif
#    endif // FASTCALL
#else      // PLATFORM_X86
#    define FASTCALL
#endif // PLATFORM_X86
