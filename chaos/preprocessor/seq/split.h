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
# ifndef CHAOS_PREPROCESSOR_SEQ_SPLIT_H
# define CHAOS_PREPROCESSOR_SEQ_SPLIT_H
#
# include <chaos/preprocessor/seq/fold_left.h>
# include <chaos/preprocessor/tuple/rem.h>
# include <chaos/preprocessor/tuple/core.h>
# include <chaos/preprocessor/cat.h>
# include <chaos/preprocessor/config.h>
# include <chaos/preprocessor/control/if.h>
# include <chaos/preprocessor/facilities/optional.h>
# include <chaos/preprocessor/facilities/empty.h>
# include <chaos/preprocessor/facilities/expand.h>
# include <chaos/preprocessor/punctuation/comma.h>
#
# /* CHAOS_PP_SEQ_SPLIT */
#
# define CHAOS_PP_SEQ_SPLIT(pred, ...) CHAOS_PP_SEQ_SPLIT_S(CHAOS_PP_STATE(), pred, __VA_ARGS__)
#
# /* CHAOS_PP_SEQ_SPLIT_S */
#
# define CHAOS_PP_SEQ_SPLIT_S(s, pred, ...) \
        CHAOS_IP_SEQ_SPLIT_M(CHAOS_PP_SEQ_FOLD_LEFT_S(s, CHAOS_IP_SEQ_SPLIT_OP, CHAOS_PP_NON_OPTIONAL(__VA_ARGS__), pred, CHAOS_IP_SEQ_SPLIT_TUPLE(__VA_ARGS__),,))
#
# define CHAOS_IP_SEQ_SPLIT_TUPLE(...) CHAOS_PP_IIF(CHAOS_PP_IS_OPTIONAL(__VA_ARGS__)) \
                                        ((CHAOS_PP_PACK_OPTIONAL(__VA_ARGS__)),)
#
# define CHAOS_IP_SEQ_SPLIT_INVOKE(m, args) m args
#
# define CHAOS_IP_SEQ_SPLIT_REM(tuple) CHAOS_PP_IIF(CHAOS_PP_TUPLE_IS_NIL(tuple))(, CHAOS_PP_COMMA() CHAOS_PP_REM tuple)
#
# define CHAOS_IP_SEQ_SPLIT_OP(s, x, pred, data, seq, elem) \
                CHAOS_PP_IF(CHAOS_IP_SEQ_SPLIT_INVOKE(pred, (s, x CHAOS_IP_SEQ_SPLIT_REM(data)))) \
                (CHAOS_IP_SEQ_SPLIT_OP_TRUE, CHAOS_IP_SEQ_SPLIT_OP_FALSE) \
                (x, pred, data, seq, elem)
#
# define CHAOS_IP_SEQ_SPLIT_OP_TRUE(x, pred, data, seq, elem) CHAOS_PP_IIF(CHAOS_IP_IS_PAREN(elem)) \
                                                            ( (pred, data, seq(elem),),\
                                                            (pred, data, seq,) )
#
# define CHAOS_IP_SEQ_SPLIT_OP_FALSE(x, pred, data, seq, elem) (pred, data, seq, elem (x))
#
# define CHAOS_IP_SEQ_SPLIT_M(pred, data, seq, elem) seq CHAOS_PP_IIF(CHAOS_PP_IS_PAREN(elem))((elem),)
#
# endif
