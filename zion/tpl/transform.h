/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    transform.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_TRANSFORM_H
#define ZION_GUARD_TPL_TRANSFORM_H

namespace zion { namespace tpl {

template<class TypeList, class MF>
struct transform {};

template<template<class...> class List, class MF, class... Args>
struct transform<List<Args...>, MF>
{
    typedef List<typename MF::template apply<Args>::type...> type;
};

}}

#endif
