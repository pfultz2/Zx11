/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    poly.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_FUNCTION_POLY_H
#define ZION_GUARD_FUNCTION_POLY_H

#include <zion/function/adaptor.h>
#include <zion/requires.h>
#include <zion/returns.h>
#include <zion/is_callable.h>
#include <type_traits>

namespace zion {

namespace details {

    template<class F1, class F2, class Enable = void>
    struct poly_kernel_base
    {
        F1 f1;
        F2 f2;
        poly_kernel_base() {}
        
        template<class A, class B>
        poly_kernel_base(A f1, B f2) : f1(f1), f2(f2)
        {}
        
        F1 get_function1() const
        {
            return f1;
        }
        F2 get_function2() const
        {
            return f2;
        }
    };
    
    template<class F1, class F2>
    struct poly_kernel_base<F1, F2, ZION_CLASS_REQUIRES(std::is_empty<F1>, std::is_empty<F2>)>
    {
        poly_kernel_base() {}
        
        template<class A, class B>
        poly_kernel_base(A, B)
        {}
        
        F1 get_function1() const
        {
            return F1();
        }
        F2 get_function2() const
        {
            return F2();
        }
    };
    
    template<class F1, class F2>
    struct poly_kernel : poly_kernel_base<F1, F2>
    {
        poly_kernel() {}
        
        template<class A, class B>
        poly_kernel(A f1, B f2) : poly_kernel_base<F1, F2>(f1, f2)
        {}
        
        ZION_RETURNS_CLASS(poly_kernel_base<F1, F2>)
        
        template<class... T>
        auto operator()(T &&... x) const ZION_RETURN_REQUIRES(zion::is_callable<F1(T&&...)>)
        (ZION_THIS->get_function1()(std::forward<T>(x)...));
        
        template<class... T>
        auto operator()(T &&... x) const ZION_RETURN_REQUIRES(not zion::is_callable<F1(T&&...)>)
        (ZION_THIS->get_function2()(std::forward<T>(x)...));
        
    };
    
    
}

template<class... Fs>
struct poly_adaptor;

template<class F, class... Fs>
struct poly_adaptor<F, Fs...> : details::poly_kernel<F, poly_adaptor<Fs...>>
{
    poly_adaptor() {}
    
    template<class X, class... Xs>
    poly_adaptor(X&& x, Xs&&... xs) : details::poly_kernel<F, poly_adaptor<Fs...>>(std::forward<X>(x), poly_adaptor<Fs...>(std::forward<Xs>(xs)...))
    {}
};

template<>
struct poly_adaptor<>
{
    poly_adaptor() {}
};

template<class... F>
poly_adaptor<F...> poly(F&&... f)
{
    return poly_adaptor<F...>(std::forward<F>(f)...);
}

}

#endif
