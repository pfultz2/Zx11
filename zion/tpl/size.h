/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    size.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_SIZE_H
#define ZION_GUARD_TPL_SIZE_H

#include <zion/tpl/integral_constant.h>

namespace zion { namespace tpl {

template<class TypeList>
struct size {};

template<template <typename...> class List, class... Args>
struct size<List<Args...>> 
: int_<sizeof... (Args)> {};

}}

#endif
