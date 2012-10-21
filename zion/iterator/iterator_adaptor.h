/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    iterator_adaptor.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_ITERATOR_ITERATOR_ADAPTOR_H
#define ZION_GUARD_ITERATOR_ITERATOR_ADAPTOR_H

#include <zion/iterator/iterator.h>
#include <zion/returns.h>

namespace zion {

namespace detail {

    template<class Adaptor, class Base>
    auto iterator_dereference(const Adaptor& a, const Base& b)
    ZION_RETURN_REQUIRES(has_dereference<const Adaptor, void()>)
    (a.dereference());
    
    template<class Adaptor, class Base>
    auto iterator_dereference(const Adaptor& a, const Base& b)
    ZION_RETURN_REQUIRES(not has_dereference<const Adaptor, void()>)
    (*b);
    
    template<class Adaptor, class Base, class T>
    ZION_FUNCTION_REQUIRES(has_equal<Adaptor, bool(T)>)
    (bool) iterator_equal(const Adaptor& a, const Base& b, const T& x)
    {
        return a.equal(x);
    }
    
    template<class Adaptor, class Base, class T>
    ZION_FUNCTION_REQUIRES(not has_equal<Adaptor, bool(T)>)
    (bool) iterator_equal(const Adaptor& a, const Base& b, const T& x)
    {
        return b == x.base();
    }
    
    template<class Adaptor, class Base>
    ZION_FUNCTION_REQUIRES(has_increment<Adaptor, void()>)
    (void) iterator_increment(Adaptor& a, Base& b)
    {
        a.increment();
    }
    
    template<class Adaptor, class Base>
    ZION_FUNCTION_REQUIRES(not has_increment<Adaptor, void()>)
    (void) iterator_increment(Adaptor& a, Base& b)
    {
        ++b;
    }
    
    template<class Adaptor, class Base>
    ZION_FUNCTION_REQUIRES(has_decrement<Adaptor, void()>)
    (void) iterator_decrement(Adaptor& a, Base& b)
    {
        a.decrement();
    }
    
    template<class Adaptor, class Base>
    ZION_FUNCTION_REQUIRES(not has_decrement<Adaptor, void()>)
    (void) iterator_decrement(Adaptor& a, Base& b)
    {
        --b;
    }
    
    template<class Adaptor, class Base, class T>
    ZION_FUNCTION_REQUIRES(has_advance<Adaptor, void(T)>)
    (void) iterator_advance(Adaptor& a, Base& b, T x)
    {
        a.advance(x);
    }
    
    template<class Adaptor, class Base, class T>
    ZION_FUNCTION_REQUIRES(not has_advance<Adaptor, void(T)>)
    (void) iterator_advance(Adaptor& a, Base& b, T x)
    {
        a += x;
    }
    
    template<class Iterator, class Category>
    struct iterator_adaptor_wrapper
    {
        Iterator it;
        
        template<class... T>
        iterator_adaptor_wrapper(T &&... x) : it(std::forward<T>(x)...)
        {}
    
        bool equal(const iterator_adaptor_wrapper& x) const
        {
            return iterator_equal(it, it.base(), x.it);
        }
        
        auto dereference() const ZION_RETURNS(iterator_dereference(it, it->base()));
    
        void increment()
        {
            iterator_increment(it, it.base());
        }
    
        template<ZION_REQUIRES(std::is_base_of<std::bidirectional_iterator_tag, Category>)>
        void decrement()
        {
            iterator_increment(it, it.base());
        }
        
        template<class T, ZION_REQUIRES(std::is_base_of<std::random_access_iterator_tag, Category>)>
        void advance(T x)
        {
            iterator_advance(it, it.base(), x);
        }
        
        // distance_to
    };
    
}

template<class Iterator, class Category=typename tpl::identity<decltype(std::declval<Iterator>().base())>::type::iterator_category>
struct iterator_adaptor : iterator<detail::iterator_adaptor_wrapper<Iterator, Category>>
{
    template<class... T>
    iterator_adaptor(T &&... x) : iterator<detail::iterator_adaptor_wrapper<Iterator, Category>>(std::forward<T>(x)...)
    {}
};

}

#endif
