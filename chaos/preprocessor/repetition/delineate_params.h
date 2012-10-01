# /* ********************************************************************
#  *                                                                    *
#  *    (C) Copyright Paul Mensonides 2003-2005.                        *
#  *                                                                    *
#  *    Distributed under the Boost Software License, Version 1.0.      *
#  *    (See accompanying file LICENSE).                                *
#  *                                                                    *
#  *    See http://chaos-pp.sourceforge.net for most recent version.    *
#  *                                                                    *
#  ******************************************************************** */
#
# ifndef CHAOS_PREPROCESSOR_DELINEATE_PARAMS_H
# define CHAOS_PREPROCESSOR_DELINEATE_PARAMS_H
#
# include <chaos/preprocessor/arithmetic/dec.h>
# include <chaos/preprocessor/cat.h>
# include <chaos/preprocessor/config.h>
# include <chaos/preprocessor/control/when.h>
# include <chaos/preprocessor/detection/is_binary.h>
# include <chaos/preprocessor/detection/is_variadic.h>
# include <chaos/preprocessor/lambda/ops.h>
# include <chaos/preprocessor/limits.h>
# include <chaos/preprocessor/recursion/basic.h>
# include <chaos/preprocessor/recursion/expr.h>
# include <chaos/preprocessor/tuple/rem.h>
# include <chaos/preprocessor/facilities/varn_cat.h>
#
#
# /* CHAOS_PP_DELINEATE_PARAMS */
#
# define CHAOS_PP_DELINEATE_PARAMS(count, sep, ...) CHAOS_PP_DELINEATE_PARAMS_BYPASS(CHAOS_PP_LIMIT_EXPR, count, sep, __VA_ARGS__)
# define CHAOS_PP_DELINEATE_PARAMS_ CHAOS_PP_LAMBDA(CHAOS_PP_DELINEATE_PARAMS_ID)()
#
# define CHAOS_PP_DELINEATE_PARAMS_ID() CHAOS_PP_DELINEATE_PARAMS
#
# /* CHAOS_PP_DELINEATE_PARAMS_BYPASS */
#
# define CHAOS_PP_DELINEATE_PARAMS_BYPASS(s, count, sep, ...) CHAOS_IP_DELINEATE_PARAMS_U(s, count, sep, __VA_ARGS__)
# define CHAOS_PP_DELINEATE_PARAMS_BYPASS_ CHAOS_PP_LAMBDA(CHAOS_PP_DELINEATE_PARAMS_BYPASS_ID)()
#
# define CHAOS_PP_DELINEATE_PARAMS_BYPASS_ID() CHAOS_PP_DELINEATE_PARAMS_BYPASS
#
# define CHAOS_IP_DELINEATE_PARAMS_U(s, count, sep, ...) \
    CHAOS_PP_EXPR_S(s)(CHAOS_IP_DELINEATE_PARAMS_I( \
        CHAOS_PP_OBSTRUCT(), CHAOS_PP_PREV(s), count, CHAOS_PP_EMPTY, sep, __VA_ARGS__ \
    )) \
    /**/
# define CHAOS_IP_DELINEATE_PARAMS_INDIRECT() CHAOS_IP_DELINEATE_PARAMS_I
# define CHAOS_IP_DELINEATE_PARAMS_I(_, s, count, sep, ss, ...) \
    CHAOS_PP_WHEN _(count)( \
        CHAOS_PP_EXPR_S(s) _(CHAOS_IP_DELINEATE_PARAMS_INDIRECT _()( \
            CHAOS_PP_OBSTRUCT _(), CHAOS_PP_PREV(s), CHAOS_PP_DEC(count), ss, ss, __VA_ARGS__ \
        )) \
        CHAOS_PP_VARN_CAT_S(s, CHAOS_PP_DEC(count), __VA_ARGS__) sep _()\
    ) \
    /**/
#
#
# endif
