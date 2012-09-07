/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_RETURNS_H
#define ZION_GUARD_RETURNS_H

#include <zion/requires.h>

#define ZION_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

// #define ZION_RETURNS(...) \
//         noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__)))) \
//         -> decltype(__VA_ARGS__) \
//         { return (__VA_ARGS__); }

#define ZION_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION(...) decltype(__VA_ARGS__)>::type { return __VA_ARGS__; }
#define ZION_RETURN_REQUIRES(...) -> typename zion::tpl::if_<ZION_REQUIRES_CLAUSE(__VA_ARGS__), ZION_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION

#endif