/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    invoke.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_INVOKE_H
#define ZION_GUARD_INVOKE_H

#include <zion/returns.h>
#include <zion/tpl/gens.h>
#include <utility>
#include <tuple>

namespace zion {

namespace detail {

template<class T>
struct tuple_gens
: zion::tpl::gens<std::tuple_size<T>::value> {};

template<int N, class T>
auto forward_get(T && t) 
ZION_RETURNS(std::forward<decltype(std::get<N>(std::forward<T>(t)))>(std::get<N>(std::forward<T>(t))));

template<class F, class T, int ...N>
auto invoke_impl(F f, T && t, zion::tpl::seq<N...>) ZION_RETURNS(f(forward_get<N>(std::forward<T>(t))...));

}

template<class F, class Tuple>
auto invoke(F f, Tuple && t) ZION_RETURNS
(
    detail::invoke_impl(f, std::forward<Tuple>(t), typename detail::tuple_gens<Tuple>::type() ) 
)

}

#endif