/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    is_callable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_IS_CALLABLE_H
#define ZION_GUARD_IS_CALLABLE_H

#include <utility>
#include <zion/tpl/integral_constant.h>


template<class F>
struct is_callable {};

template<class F, class... Args>
struct is_callable<F(Args...)>
{
    typedef char yes; 
    typedef long no; 
    template<class T> 
    struct selector {}; 

    template<class U> 
    static yes check( selector<decltype( std::declval<U>()(std::declval<Args>()...) )> * ); 

    template<class U> 
    static no check(...); 
    
    static const bool value = sizeof(check<F>(0)) == sizeof(yes); 

    typedef zion::tpl::bool_<value> type; 
};

#endif