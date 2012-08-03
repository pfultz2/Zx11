/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    not.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_NOT_H
#define ZION_GUARD_TPL_NOT_H

#include <zion/tpl/integral_constant.h>

namespace zion { namespace tpl {

template<class B>
struct not_
: bool_<not B::type::value> {};


}}

#endif
