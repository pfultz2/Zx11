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
# ifndef CHAOS_PREPROCESSOR_REPETITION_X_FOR_AUX_H
# define CHAOS_PREPROCESSOR_REPETITION_X_FOR_AUX_H
#
# include <chaos/preprocessor/comparison/not_equal.h>
# include <chaos/preprocessor/config.h>
# include <chaos/preprocessor/control/iif.h>
# include <chaos/preprocessor/control/inline_when.h>
# include <chaos/preprocessor/detection/is_unary.h>
# include <chaos/preprocessor/detection/is_variadic.h>
# include <chaos/preprocessor/lambda/call.h>
# include <chaos/preprocessor/lambda/ops.h>
# include <chaos/preprocessor/punctuation/comma.h>
# include <chaos/preprocessor/recursion/buffer.h>
# include <chaos/preprocessor/recursion/expr.h>
# include <chaos/preprocessor/recursion/phase.h>
# include <chaos/preprocessor/seq/core.h>
# include <chaos/preprocessor/tuple/eat.h>
# include <chaos/preprocessor/tuple/rem.h>
#
# /* CHAOS_PP_FOR_AUX_X */
#
# if CHAOS_PP_VARIADICS
#    define CHAOS_PP_FOR_AUX_X(size, pred, op, macro, data, ...) CHAOS_PP_FOR_AUX_X_S(CHAOS_PP_STATE(), size, pred, op, macro, data, __VA_ARGS__)
#    define CHAOS_PP_FOR_AUX_X_ CHAOS_PP_LAMBDA(CHAOS_PP_FOR_AUX_X_ID)()
# else
#    define CHAOS_PP_FOR_AUX_X(size, pred, op, macro, data, state) CHAOS_PP_FOR_AUX_X_S(CHAOS_PP_STATE(), size, pred, op, macro, data, state)
# endif
#
# define CHAOS_PP_FOR_AUX_X_ID() CHAOS_PP_FOR_AUX_X
#
# /* CHAOS_PP_FOR_AUX_X_S */
#
# if CHAOS_PP_VARIADICS
#    define CHAOS_PP_FOR_AUX_X_S(s, size, pred, op, macro, data, ...) \
        CHAOS_IP_FOR_AUX_X_U(s, size, pred, op, macro, CHAOS_PP_INLINE_WHEN(CHAOS_PP_IS_VARIADIC(data))(CHAOS_PP_REM)(data), (__VA_ARGS__)) \
        /**/
#    define CHAOS_PP_FOR_AUX_X_S_ CHAOS_PP_LAMBDA(CHAOS_PP_FOR_AUX_X_S_ID)()
# else
#    define CHAOS_PP_FOR_AUX_X_S(s, size, pred, op, macro, data, state) \
        CHAOS_IP_FOR_AUX_X_U(s, size, pred, op, macro, CHAOS_PP_INLINE_WHEN(CHAOS_PP_IS_UNARY(data))(CHAOS_PP_REM)(data), (state)) \
        /**/
# endif
#
# define CHAOS_PP_FOR_AUX_X_S_ID() CHAOS_PP_FOR_AUX_X_S
#
# define CHAOS_IP_FOR_AUX_X_U(s, size, pred, op, macro, data, ps) \
    CHAOS_IP_FOR_AUX_X_I( \
        CHAOS_PP_NEXT(s), (CHAOS_PP_NEXT(s)), CHAOS_PP_FIXED_S(s, size), 0, \
        pred, CHAOS_PP_CALL(pred), op, CHAOS_PP_CALL(op), macro, CHAOS_PP_CALL(macro), data, ps \
    ) \
    /**/
# define CHAOS_IP_FOR_AUX_X_INDIRECT() CHAOS_IP_FOR_AUX_X_I
# define CHAOS_IP_FOR_AUX_X_I(s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    CHAOS_PP_IIF(CHAOS_PP_NOT_EQUAL(s, fix))( \
        CHAOS_PP_IIF(CHAOS_PP_NOT_EQUAL(s, CHAOS_PP_PREV(fix)))( \
            CHAOS_IP_FOR_AUX_X_II, CHAOS_IP_FOR_AUX_X_ ## mode \
        ), \
        CHAOS_IP_FOR_AUX_X_III \
    )(CHAOS_PP_PHASE(0), s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    /**/
# define CHAOS_IP_FOR_AUX_X_II(_, s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    _(1, CHAOS_PP_EXPR_S)(s)( \
        CHAOS_IP_FOR_AUX_X_ ## mode(_, s, (CHAOS_PP_NEXT(s)) jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    ) \
    /**/
# define CHAOS_IP_FOR_AUX_X_0(_, s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    _(1, CHAOS_PP_IIF)(_(0, _p)()(s, pred, CHAOS_PP_REM data _(0, CHAOS_PP_COMMA)() CHAOS_PP_REM ps))( \
        _(1, CHAOS_PP_EXPR_S)(s), CHAOS_PP_EAT \
    )( \
        _(1, CHAOS_IP_FOR_AUX_X_INDIRECT)()( \
            CHAOS_PP_NEXT(s), jump, fix, 1, pred, _p, op, _o, macro, _m, data, ps \
        ) \
    ) \
    /**/
# define CHAOS_IP_FOR_AUX_X_1(_, s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    _(0, _m)()(s, macro, CHAOS_PP_REM data _(0, CHAOS_PP_COMMA)() CHAOS_PP_REM ps) \
    _(1, CHAOS_PP_EXPR_S)(s)(_(1, CHAOS_IP_FOR_AUX_X_INDIRECT)()( \
        CHAOS_PP_NEXT(s), jump, fix, 0, pred, _p, op, _o, macro, _m, data, (_(0, _o)()(s, op, CHAOS_PP_REM data _(0, CHAOS_PP_COMMA)() CHAOS_PP_REM ps)) \
    )) \
    /**/
# define CHAOS_IP_FOR_AUX_X_III(_, s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    CHAOS_IP_FOR_AUX_X_IV(CHAOS_PP_SEQ_HEAD(jump), CHAOS_PP_SEQ_TAIL(jump), fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    /**/
# define CHAOS_IP_FOR_AUX_X_IV(s, jump, fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    CHAOS_PP_IIF(CHAOS_PP_NOT_EQUAL(s, CHAOS_PP_PREV(fix)))( \
        CHAOS_IP_FOR_AUX_X_II, CHAOS_IP_FOR_AUX_X_ ## mode \
    )(CHAOS_PP_PHASE(1), s, CHAOS_PP_IIF(CHAOS_PP_SEQ_IS_NIL(jump))((s), jump), fix, mode, pred, _p, op, _o, macro, _m, data, ps) \
    /**/
#
# endif
