/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    adaptor.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_FUNCTION_ADAPTOR_H
#define ZION_GUARD_FUNCTION_ADAPTOR_H

#include <zion/requires.h>
#include <type_traits>
    
namespace zion {

//Optimization for stateless functions
template<class F, class Enable = void>
struct function_adaptor_base
{
    F f;
    function_adaptor_base() {};
    template<class X>
    function_adaptor_base(X f) : f(f) {};

    F get_function() const
    {
        return f;
    }
};

template<class F>
struct function_adaptor_base<F, ZION_CLASS_REQUIRES(std::is_empty<F>)>
{
    function_adaptor_base() {};
    template<class X>
    function_adaptor_base(X) {};

    F get_function() const
    {
        return F();
    }
};

}

#endif