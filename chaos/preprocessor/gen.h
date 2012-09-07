# /* ********************************************************************
#  *                                                                    *
#  *    (C) Copyright Paul Fultz II 2012.                               *
#  *                                                                    *
#  *    Distributed under the Boost Software License, Version 1.0.      *
#  *    (See accompanying file LICENSE).                                *
#  *                                                                    *
#  *    See http://chaos-pp.sourceforge.net for most recent version.    *
#  *                                                                    *
#  ******************************************************************** */
#
# ifndef CHAOS_PREPROCESSOR_GEN_H
# define CHAOS_PREPROCESSOR_GEN_H
#
# include <chaos/preprocessor/config.h>
# include <chaos/preprocessor/lambda/ops.h>
# include <chaos/preprocessor/control/if.h>
# include <chaos/preprocessor/recursion/basic.h>
# include <chaos/preprocessor/recursion/expr.h>
# include <chaos/preprocessor/tuple/eat.h>
#
#
# /* CHAOS_PP_VARN_CAT */
#
# define CHAOS_PP_VARN_CAT(n, ...) CHAOS_PP_VARN_CAT_S(CHAOS_PP_STATE(), n, __VA_ARGS__)
#
# define CHAOS_PP_VARN_CAT_S(s, n, ...) CHAOS_IP_VARN_CAT_U(CHAOS_PP_NEXT(s), n, __VA_ARGS__, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT, CHAOS_PP_INTERCEPT)
#
# define CHAOS_PP_VARN_CAT_S_ID() CHAOS_PP_VARN_CAT_S
#
# define CHAOS_IP_VARN_CAT_U(...) CHAOS_IP_VARN_CAT_I(__VA_ARGS__)
#
# define CHAOS_IP_VARN_CAT_I(s, n, a, b, c, d, e, f, g, h, ...) \
        CHAOS_IP_VARN_CAT_E(s, a, n) \
        CHAOS_IP_VARN_CAT_E(s, b, n) \
        CHAOS_IP_VARN_CAT_E(s, c, n) \
        CHAOS_IP_VARN_CAT_E(s, d, n) \
        CHAOS_IP_VARN_CAT_E(s, e, n) \
        CHAOS_IP_VARN_CAT_E(s, f, n) \
        CHAOS_IP_VARN_CAT_E(s, g, n) \
        CHAOS_IP_VARN_CAT_E(s, h, n)
#
# define CHAOS_IP_VARN_CAT_E(s, a, n) CHAOS_PP_IIF(CHAOS_PP_IS_PAREN(a))(CHAOS_IP_VARN_CAT_E_PAREN, CHAOS_IP_VARN_CAT_E_TOKEN)(s, a, n)
# define CHAOS_IP_VARN_CAT_E_PAREN(s, a, n) ( CHAOS_PP_DEFER(CHAOS_PP_EXPR_S(s))(CHAOS_PP_DEFER(CHAOS_PP_VARN_CAT_S_ID)()(s, n, CHAOS_PP_REM a)) )
# define CHAOS_IP_VARN_CAT_E_TOKEN(s, a, n) a ## n
#
# /* CHAOS_PP_PARAMS */
#
# define CHAOS_PP_PARAMS(n, ...) CHAOS_PP_EXPR(CHAOS_PP_ENUM(n, CHAOS_IP_PARAMS_E, __VA_ARGS__))
# define CHAOS_PP_PARAMS_S(s, n, ...) CHAOS_PP_EXPR_S(s)(CHAOS_PP_ENUM(n, CHAOS_PP_VARN_CAT_S, __VA_ARGS__))
#
#



# endif