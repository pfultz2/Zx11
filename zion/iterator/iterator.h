/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_ITERATOR_ITERATOR_H
#define ZION_GUARD_ITERATOR_ITERATOR_H

#include <iterator>
#include <zion/operators.h>
#include <zion/introspection.h>
#include <zion/requires.h>
#include <chaos/preprocessor.h>


namespace zion {

namespace detail {

    ZION_HAS_MEMBER(equal)
    ZION_HAS_MEMBER(dereference)
    ZION_HAS_MEMBER(increment)
    ZION_HAS_MEMBER(decrement)
    ZION_HAS_MEMBER(advance)
    ZION_HAS_MEMBER(distance_to)
    
    template<class It1, class It2>
    ZION_FUNCTION_REQUIRES(has_equal<It1, bool(It2)>)
    (bool) iterator_equal(const It1& it1, const It2& it2)
    {
        return it1.equal(it2);
    }
    
    template<class It1, class It2>
    ZION_FUNCTION_REQUIRES(not has_equal<It1, bool(It2)>)
    (bool) iterator_equal(const It1& it1, const It2& it2)
    {
        return &(*it1) == &(*it2);
    }

    // std::iterator<Category, Value, Distance, Pointer, Reference>
    
    template<class T, class Self>
    struct iterator_derived
    {
        typedef T type;
    };
    
    template<class Self>
    struct iterator_derived<void, Self>
    {
        typedef Self type;
    };
    
    struct iterator_access_tag
    {};
    
    template<
    class Iterator,
    class Category,
    class Traits,
    class Derived = void
    > struct iterator_wrapper
    {
    };
    
    // TODO: Add support for input and output iterators
    template<
    class Iterator,
    class Traits,
    class Derived
    > struct iterator_wrapper<Iterator, iterator_access_tag, Traits, Derived> : Traits
    {
        Iterator it;
        
        iterator_wrapper()
        {}
        
        template<class... T>
        iterator_wrapper(T &&... x) : it(std::forward<T>(x)...)
        {
        }
        
        typename Traits::reference operator*() const
        {
            return this->it.dereference();
        }
    
        typename Traits::pointer operator->() const
        {
            return &this->it.dereference();
        }
    
        template<class T>
        bool operator==(const T& x) const
        {
            return iterator_equal(it, x.it);
        }
        
        template<class T>
        bool operator!=(const T& x) const
        {
            return not iterator_equal(it, x.it);
        }
    
    };

#define ZION_ITERATOR_WRAPPER_TYPE(category, base) \
typename iterator_derived<Derived, iterator_wrapper<Iterator, category, Traits, Derived>>::type

#define ZION_ITERATOR_WRAPPER_OP(s, x, ...) \
x<__VA_ARGS__>
    
#define ZION_ITERATOR_WRAPPER(category, base_category, ...) \
        template< \
        class Iterator, \
        class Traits, \
        class Derived \
        > struct iterator_wrapper<Iterator, category, Traits, Derived> \
        : iterator_wrapper<Iterator, base_category, Traits, Derived>, \
        CHAOS_PP_EXPR_S(1)(CHAOS_PP_TUPLE_TRANSFORM_S(1, ZION_ITERATOR_WRAPPER_OP, (__VA_ARGS__), ZION_ITERATOR_WRAPPER_TYPE(category, base_category))) \
        { \
            typedef ZION_ITERATOR_WRAPPER_TYPE(category, base_category) derived; \
            typedef iterator_wrapper<Iterator, base_category, Traits, Derived> base; \
            \
            iterator_wrapper() \
            {} \
                \
            template<class... T> \
            iterator_wrapper(T &&... x) \
            : base(std::forward<T>(x)...) \
            {} \
            ZION_ITERATOR_WRAPPER_I
        
#define ZION_ITERATOR_WRAPPER_I(...) __VA_ARGS__ };

    ZION_ITERATOR_WRAPPER(std::forward_iterator_tag, iterator_access_tag, zion::incrementable)
    (
        derived& operator++()
        {
         this->it.increment();
         return static_cast<derived&>(*this);
        }
    )

    ZION_ITERATOR_WRAPPER(std::bidirectional_iterator_tag, std::forward_iterator_tag, zion::decrementable)
    (
        derived& operator--()
        {
            this->it.decrement();
            return static_cast<derived&>(*this);
        }
    )

    ZION_ITERATOR_WRAPPER(std::random_access_iterator_tag, std::bidirectional_iterator_tag, zion::less_than_comparable)
    (
    
        bool operator<(const iterator_wrapper& x) const
        {
            return this->it.distance_to(x.it) < 0;
        }
     
        derived& operator+=(typename Traits::difference_type x)
        {
            this->it.advance(x);
            return static_cast<derived&>(*this);
        }
     
        friend derived operator+(derived x, const derived& y)
        {
            return x += y;
        }
     
        friend typename Traits::difference_type operator-(const derived& x, const derived& y)
        {
            return x.distance_to(y);
        }
     
        friend derived operator+(derived x, const derived& y)
        {
            return x += y;
        }
     
        derived& operator-=(typename Traits::difference_type x)
        {
            this->it.advance(-x);
            return static_cast<derived&>(*this);
        }

        derived operator-(typename Traits::difference_type x) const
        {
            derived result(static_cast<const derived&>(*this));
            return result -= x;
        }
    )

    template<class Iterator>
    struct iterator_category
    : tpl::if_<has_advance<Iterator>, tpl::lazy<std::random_access_iterator_tag>>
    ::template else_if<has_decrement<Iterator>, tpl::lazy<std::bidirectional_iterator_tag>>
    ::template else_<tpl::lazy<std::forward_iterator_tag>>
    {};
    
    template<class Iterator, class Enable = void>
    struct iterator_distance
    {};
    
    template<class Iterator>
    struct iterator_distance<Iterator, ZION_CLASS_REQUIRES(has_distance_to<Iterator>)>
    {
        typedef decltype(std::declval<Iterator>().distance_to(std::declval<Iterator>())) type;
    };
    
    template<class Iterator>
    struct iterator_distance<Iterator, ZION_CLASS_REQUIRES(not has_distance_to<Iterator>)>
    {
        typedef std::ptrdiff_t type;
    };
    
    


    template<
    class Iterator,
    class Category=typename detail::iterator_category<Iterator>::type,
    class Reference=decltype(std::declval<Iterator>().dereference()), class Value=typename std::decay<Reference>::type>
    struct iterator_traits
    : std::iterator_traits
    <Category,
    Value,
    typename detail::iterator_distance<Iterator>::type,
    typename std::add_pointer<Value>::type,
    Reference>
    {};

}



template<class Iterator, class Category=typename detail::iterator_category<Iterator>::type>
struct iterator : detail::iterator_wrapper<Iterator, Category, detail::iterator_traits<Iterator, Category>>
{
    iterator()
    {}
    
    template<class... T>
    iterator(T &&... x) : detail::iterator_wrapper<Iterator, Category, detail::iterator_traits<Iterator, Category>>
    {}
};

}

#endif
