/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    push_front.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_PUSH_FRONT_H
#define ZION_GUARD_TPL_PUSH_FRONT_H

namespace zion { namespace tpl {

template<class TypeList, class T>
struct push_front {};

template<template <typename...> class List, class Type, class... Args>
struct push_front<List<Args...>, Type>
{
    typedef List<Type, Args...> type;
};

}}

#endif
