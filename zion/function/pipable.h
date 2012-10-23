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
#include <tuple>

namespace zion {

//Pipable
namespace details {
template<class F, class Sequence>
struct pipe_closure
{
    F f;
    Sequence seq;
    
    template<class S>
    pipe_closure(S && seq) : seq(std::forward<S>(seq)) {};
    
    template<class S>
    pipe_closure(F f, S && seq) : f(f), seq(std::forward<S>(seq)) {};

//    template<class A>
//    friend auto operator|(A && a, pipe_closure<F, Sequence> p) ZION_RETURNS
//    (zion::invoke(p.f, std::tuple_cat
//        (
//            std::forward_as_tuple(std::forward<A>(a)), 
//            std::forward<Sequence>(p.seq)
//        ))
//    );
};

    template<class A, class F, class Sequence>
    auto operator|(A && a, pipe_closure<F, Sequence> p) ZION_RETURNS
    (zion::invoke(p.f, std::tuple_cat
                  (
                   std::forward_as_tuple(std::forward<A>(a)),
                   std::forward<Sequence>(p.seq)
                   ))
     );

template<class F, class Seq>
auto make_pipe_closure(F f, Seq&& seq) ZION_RETURNS
(pipe_closure<F, Seq&&>(f, std::forward<Seq>(seq)));
}


template<class F>
struct pipable_adaptor : function_adaptor_base<F>
{
    pipable_adaptor() {}
    template<class X>
    pipable_adaptor(X f) : function_adaptor_base<F>(f) {}
    
    ZION_RETURNS_CLASS(function_adaptor_base<F>)

//    template<class... T>
//    auto operator()(T && ... x) ZION_RETURN_REQUIRES(is_callable<F(T&&...)>)
//    (ZION_THIS->get_function()(std::forward<T>(x)...));

    template<class... T>
    auto operator()(T && ... x) ZION_RETURNS
    (
        details::make_pipe_closure(ZION_THIS->get_function(), std::forward_as_tuple(std::forward<T>(x)...)) 
    );

//    template<class A>
//    friend auto operator|(A && x, pipable_adaptor<F> p) ZION_RETURNS
//    (p.get_function()(std::forward<A>(x)));
};

    template<class A, class F>
    auto operator|(A && x, pipable_adaptor<F> p) ZION_RETURNS
    (p.get_function()(std::forward<A>(x)));
}

#endif