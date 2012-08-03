/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    requires.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZION_GUARD_REQUIRES_H
#define	ZION_GUARD_REQUIRES_H

#include <zion/tpl/fold.h>
#include <zion/tpl/identity.h>
#include <zion/tpl/integral_constant.h>
#include <zion/tpl/type_list.h>
#include <chaos/preprocessor.h>


namespace zion{

template <bool B, class T = void>
struct enable_if_c 
{
    typedef T type;
};

template <class T>
struct enable_if_c<false, T> {};

template <class Cond, class T = void> 
struct enable_if 
: enable_if_c<Cond::value, T> {};

namespace requires_detail{
struct not_tag {};

template<class Sequence>
struct requires_
{
    template<class X>
    struct not_state : X {};

    struct op
    {
        template<class X, class Y>
        struct apply
        : zion::tpl::bool_<X::type::value and Y::type::value> {};

        template<class X>
        struct apply<X, not_tag>
        : zion::tpl::identity<not_state<X> > {};

        template<class X, class Y>
        struct apply<not_state<X>, Y>
        : zion::tpl::bool_<X::type::value and not Y::type::value> {};
    };

    typedef typename zion::tpl::fold<Sequence, zion::tpl::bool_<true>, op>::type type;

    static const bool value = type::value;

};

}
}
#define ZION_DETAIL_REQUIRES_CLAUSE_KEYWORD_not ()

#define ZION_DETAIL_REQUIRES_CLAUSE_DISPATCH(tokened, x) \
CHAOS_PP_IIF(CHAOS_PP_IS_PAREN(tokened)) \
(CHAOS_PP_IDENTITY(zion::requires_detail::not_tag, CHAOS_PP_REM tokened), CHAOS_PP_IDENTITY(x))()
#define ZION_DETAIL_REQUIRES_CLAUSE_EACH(s, i, x) CHAOS_PP_COMMA_IF(i) ZION_DETAIL_REQUIRES_CLAUSE_DISPATCH(CHAOS_PP_CAT(ZION_DETAIL_REQUIRES_CLAUSE_KEYWORD_, x), x)

#define ZION_DETAIL_REQUIRES_CLAUSE(...) \
CHAOS_PP_EXPR(CHAOS_PP_TUPLE_FOR_EACH_I(ZION_DETAIL_REQUIRES_CLAUSE_EACH, (__VA_ARGS__)))

#define ZION_REQUIRES_CLAUSE(...) \
zion::requires_detail::requires_<zion::tpl::type_list<ZION_DETAIL_REQUIRES_CLAUSE(__VA_ARGS__)> >


#ifdef ZION_TEST
template<class A, class B>
struct requires_test
{
    typedef zion::tpl::bool_<true> type;
};
static_assert(ZION_REQUIRES_CLAUSE(requires_test<void, void>)::type::value, "Failed");
static_assert(ZION_REQUIRES_CLAUSE(zion::tpl::bool_<true>)::type::value, "Failed");
static_assert(ZION_REQUIRES_CLAUSE(zion::tpl::bool_<true>, zion::tpl::bool_<true>)::type::value, "Failed");
static_assert(ZION_REQUIRES_CLAUSE(zion::tpl::bool_<true>, not zion::tpl::bool_<false>)::type::value, "Failed");
static_assert(ZION_REQUIRES_CLAUSE(not zion::tpl::bool_<false>, zion::tpl::bool_<true>)::type::value, "Failed");
static_assert(ZION_REQUIRES_CLAUSE(not zion::tpl::bool_<false>)::type::value, "Failed");
#endif

 
#define ZION_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE(...) __VA_ARGS__>::type
#define ZION_FUNCTION_REQUIRES(...) typename zion::enable_if<ZION_REQUIRES_CLAUSE(__VA_ARGS__), ZION_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE

#define ZION_CLASS_REQUIRES(...) typename zion::enable_if<ZION_REQUIRES_CLAUSE(__VA_ARGS__)>::type

#define ZION_REQUIRES(...) class Zelda_Enable = typename zion::enable_if<ZION_REQUIRES_CLAUSE(__VA_ARGS__)>::type


#endif	/* ZION_REQUIRES_H */

