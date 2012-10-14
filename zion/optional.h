/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    optional.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_OPTIONAL_H
#define ZION_GUARD_OPTIONAL_H

#include <utility>
#include <functional>
#include <zion/requires.h>
#include <type_traits>
#include <cassert>

namespace zion {

namespace details {

template<class T>
struct optional_storage
{
    char data[sizeof(T)];

    T* get() const
    {
        return reinterpret_cast<T*>(data);
    }

    template<class... Ts>
    void init(Ts && ... x)
    {
        new (data) T(std::forward<Ts>(x)...);
    }
};

template<class T>
struct optional_type
{
    typedef T type;
};

template<class T>
struct optional_type<std::reference_wrapper<T>>
{
    typedef T type;
};

}

static std::nullptr_t none;

template<class T, ZION_REQUIRES(std::is_copy_constructible<T>, not std::is_reference<T>)>
class optional
{
    details::optional_storage<T> storage;
    T * data;
    
    typedef typename details::optional_type<T>::type value_type;
    
public:

    optional() : data(0)
    {}
    
    optional(std::nullptr_t) : data(0)
    {}

    template<class... Ts>
    optional(Ts && ... x)
    {
        this->storage.init(std::forward<Ts>(x)...);
        this->data = this->storage.get();
    }
    
    template<class... Ts>
    void construct(Ts &&... x)
    {
        this->storage.init(std::forward<Ts>(x)...);
        this->data = this->storage.get();
    }

    optional(const optional& rhs)
    {
        if (rhs.data == 0) data = 0;
        else
        {
            this->construct(*rhs.data);
        }
    }
    
    optional(optional&& rhs)
    {
        if (rhs.data == 0) data = 0;
        else
        {
            this->construct(std::move(*rhs.data));
        }
    }
    
    ~optional()
    {
        this->assign(none);
    }
    
    void assign(std::nullptr_t)
    {
        if (this->data != 0)
        {
            this->data->~T();
        }
    }
    
    template<class X, ZION_REQUIRES(std::is_assignable<X, X>)>
    void assign(X x)
    {
        using std::swap;
        swap(*this->data, x);
    }
    
    template<class X, ZION_REQUIRES(not std::is_assignable<X, X>)>
    void assign(X&& x)
    {
        this->assign(none);
        this->construct(std::forward<X>(x));
    }
    
    void assign(const optional& rhs)
    {
        if (this != &rhs)
        {
            if (rhs.data == 0) this->assign(none);
            else this->assign(*rhs.data);
        }
    }
    
    optional& operator=(optional rhs)
    {
        this->assign(rhs);
        return *this;
    }
    
    template<class X>
    optional& operator= (X&& rhs)
    {
        this->assign(rhs);
        return *this;
    }
    
    const value_type& get() const
    {
        assert(this->data != 0);
        return *this->data;
    }
    
    value_type& get()
    {
        assert(this->data != 0);
        return *this->data;
    }
    
    bool empty() const
    {
        return this->data != 0;
    }
    
    value_type* begin()
    {
        if (not this->empty()) return std::addressof(this->get());
        else return 0;
    }
    
    value_type* end()
    {
        if (not this->empty()) return std::addressof(this->get()) + 1;
        else return 0;
    }
    
    const value_type* begin() const
    {
        if (not this->empty()) return std::addressof(this->get());
        else return 0;
    }
    
    const value_type* end() const
    {
        if (not this->empty()) return std::addressof(this->get()) + 1;
        else return 0;
    }
    
    // Operators
    
    const value_type& operator*() const
    {
        return this->get();
    }
    
    value_type& operator*()
    {
        return this->get();
    }
    
    const value_type* operator->() const
    {
        return this->data;
    }
    
    value_type* operator->()
    {
        return this->data;
    }
    
    explicit operator bool() const
    {
        return this->data != 0;
    }

};

template<class T>
inline T&& operator| (optional<T> && x, T&& y)
{
    if (x) return x.get();
    else return y;
}

template<class T>
inline bool operator != ( optional<T> const& x, std::nullptr_t )
{
    return x;
}

template<class T>
inline bool operator != ( std::nullptr_t, optional<T> const& x )
{
    return x;
}

template<class T>
inline bool operator == ( optional<T> const& x, std::nullptr_t )
{
    return !x;
}

template<class T>
inline bool operator == ( std::nullptr_t, optional<T> const& x )
{
    return !x;
}


}

#endif