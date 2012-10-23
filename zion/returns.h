/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    returns.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_RETURNS_H
#define ZION_GUARD_RETURNS_H

#include <zion/requires.h>
#include <chaos/preprocessor.h>

// #define ZION_RETURNS(...) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

// This is a workaround for the defect report
#define ZION_RETURNS_RETURN(...) return ZION_RETURNS_RETURN_X(CHAOS_PP_WALL(__VA_ARGS__))
#define ZION_RETURNS_RETURN_X(...) __VA_ARGS__

#define ZION_RETURNS_DECLTYPE(...) decltype(ZION_RETURNS_DECLTYPE_CONTEXT(__VA_ARGS__))

#define ZION_RETURNS_DECLTYPE_CONTEXT(...) ZION_RETURNS_DECLTYPE_CONTEXT_X(CHAOS_PP_WALL(__VA_ARGS__))
#define ZION_RETURNS_DECLTYPE_CONTEXT_X(...) __VA_ARGS__

#define ZION_RETURNS_THIS() CHAOS_PP_IIF(CHAOS_PP_IS_NULLARY(ZION_RETURNS_DECLTYPE_CONTEXT(())))(this, zion_this())

#define ZION_THIS CHAOS_PP_RAIL(ZION_RETURNS_THIS)()

#define ZION_RETURNS_CLASS(...) static __VA_ARGS__* zion_this();

//#define ZION_RETURNS(...) \
//        noexcept(noexcept(decltype(__VA_ARGS__)(std::move(__VA_ARGS__)))) \
//        -> decltype(__VA_ARGS__) \
//        { return (__VA_ARGS__); } static_assert(true, "")


#define ZION_RETURNS(...) -> ZION_RETURNS_DECLTYPE(__VA_ARGS__) { ZION_RETURNS_RETURN(__VA_ARGS__); } static_assert(true, "")

#define ZION_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION(...) ZION_RETURNS_DECLTYPE(__VA_ARGS__)>::type { ZION_RETURNS_RETURN(__VA_ARGS__); } static_assert(true, "")
#define ZION_RETURN_REQUIRES(...) -> typename zion::tpl::if_<ZION_REQUIRES_CLAUSE(__VA_ARGS__), ZION_ERROR_RETURN_REQUIRES_NEEDS_AN_EXPRESSION

#endif