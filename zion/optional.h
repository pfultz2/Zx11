/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    optional.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_OPTIONAL_H
#define ZION_GUARD_OPTIONAL_H

#include <utility>
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

}

static std::nullptr_t none;

template<class T, ZION_REQUIRES(std::is_copy_constructible<T>, not std::is_reference<T>)>
class optional
{
    details::optional_storage<T> storage;
    T * data;
    
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
    
    void assign(std::nullptr_t)
    {
        if (this->data != 0)
        {
            this->data->~T();
        }
    }
    
    template<ZION_REQUIRES(std::is_assignable<T, T>)>
    void assign(T x)
    {
        using std::swap;
        swap(*this->data, x);
    }
    
    template<ZION_REQUIRES(not std::is_assignable<T, T>)>
    void assign(const T& x)
    {
        this->assign(none);
        this->construct(x);
    }
    
    template<ZION_REQUIRES(not std::is_assignable<T, T>)>
    void assign(T&& x)
    {
        this->assign(none);
        this->construct(std::move(x));
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
    
    const T& get() const
    {
        assert(this->data != 0);
        return *this->data;
    }
    
    T& get()
    {
        assert(this->data != 0);
        return *this->data;
    }
    
    const T& operator*() const
    {
        return this->get();
    }
    
    T& operator*()
    {
        return this->get();
    }
    
    const T* operator->() const
    {
        return this->data;
    }
    
    T* operator->()
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