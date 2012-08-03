/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    identity.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_TPL_IDENTITY_H
#define ZION_GUARD_TPL_IDENTITY_H

namespace zion { namespace tpl {

template<class T>
struct identity
{
    typedef T type;
};

}}

#endif
