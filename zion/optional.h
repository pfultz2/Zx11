/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    optional.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_OPTIONAL_H
#define ZION_GUARD_OPTIONAL_H

#include <utility>

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

template<class T>
class optional
{
    details::optional_storage<T> storage;
    T * data;

    optional() : data(0)
    {}

    template<class... Ts>
    optional(Ts && ... x)
    {
        this->storage.init(std::forward<Ts>(x)...);
        this->data = this->storage.get();
    }

    optional(const optional& rhs)
    {
        if (rhs.data == 0) data = 0;
        else
        {
            this->init(*rhs.data);
        }
    }

    // TODO: Move constructor


};


}

#endif