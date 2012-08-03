/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    or.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_OR_H
#define ZION_GUARD_TPL_OR_H

#include <zion/tpl/fold.h>
#include <zion/tpl/integral_constant.h>

namespace zion { namespace tpl {

namespace details{
struct or_op
{
    template<class X, class Y>
    struct apply
    : bool_<X::type::value or Y::type::value> {};
};
}

template<class... Ts>
struct or_
: fold<or_<Ts...>, bool_<true>, details::or_op > {};


}}

#endif
