/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pop_front.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_POP_FRONT_H
#define ZION_GUARD_TPL_POP_FRONT_H

namespace zion { namespace tpl {

template<class List>
struct pop_front {};

template<template <typename...> class List, class Head, class... Args>
struct pop_front<List<Head, Args...>>
{
    typedef List<Args...> type;
};

}}

#endif
