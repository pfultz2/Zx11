/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pipable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_FUNCTION_PIPABLE_H
#define ZION_GUARD_FUNCTION_PIPABLE_H

#include <zion/function/adaptor.h>
#include <zion/is_callable.h>
#include <zion/returns.h>
#include <zion/invoke.h>
#include <functional>

namespace zion {

template<class T>
ZION_FUNCTION_REQUIRES(std::is_rvalue_reference<T&&>)
(T) auto_ref(T&& x)
{
    return std::forward<T>(x);
}

template<class T>
ZION_FUNCTION_REQUIRES(not std::is_rvalue_reference<T&&>)
(std::reference_wrapper<typename std::remove_reference<T>::type>) auto_ref(T&& x)
{
    return std::reference_wrapper<typename std::remove_reference<T>::type>(x);
}

//Pipable
namespace details {
template<class F>
struct pipe_closure
{
    F f;
    
    template<class X>
    pipe_closure(X f) : f(f) {};

};

    template<class A, class F, class Sequence>
    auto operator|(A && a, const pipe_closure<F>& p) ZION_RETURNS
    (p.f(std::forward<A>(a)));

template<class F>
auto make_pipe_closure(F f) ZION_RETURNS
(pipe_closure<F>(f));
}


template<class F>
struct pipable_adaptor : function_adaptor_base<F>
{
    pipable_adaptor() {}
    template<class X>
    pipable_adaptor(X f) : function_adaptor_base<F>(f) {}
    
    ZION_RETURNS_CLASS(function_adaptor_base<F>)

    template<class... T>
    auto operator()(T && ... x) ZION_RETURN_REQUIRES(is_callable<F(T&&...)>)
    (ZION_THIS->get_function()(std::forward<T>(x)...));

    template<class... T>
    auto operator()(T && ... x) ZION_RETURN_REQUIRES(not is_callable<F(T&&...)>)
    (
        details::make_pipe_closure(ZION_THIS->get_function(), std::bind(std::placeholders::_1, auto_ref(std::forward<T>(x))...) ) 
    );
};

    template<class A, class F>
    auto operator|(A && x, pipable_adaptor<F> p) ZION_RETURNS
    (p.get_function()(std::forward<A>(x)));
}

#endif