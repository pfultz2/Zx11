/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    lazy.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_LAZY_H
#define ZION_GUARD_TPL_LAZY_H

namespace zion { namespace tpl {

template<class T>
struct lazy : T {};

template<class T>
struct eval
{
    typedef T type;
};

template<class T>
struct eval<lazy<T> >
: lazy<T> {};

}}

#endif
