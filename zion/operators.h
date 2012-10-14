/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    operators.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZION_GUARD_ZION_OPERATORS_H
#define ZION_GUARD_ZION_OPERATORS_H

namespace zion {

namespace detail {
template <typename T> class empty_base {};
}
    
template <class T, class U = void, class B = detail::empty_base<T> >
struct less_than_comparable : B
{
    template<class A> struct operator_rebase { typedef less_than_comparable<T, U, A> type; };
    friend bool operator<=(const T& x, const U& y) { return !static_cast<bool>(x > y); }
    friend bool operator>=(const T& x, const U& y) { return !static_cast<bool>(x < y); }
    friend bool operator>(const U& x, const T& y)  { return y < x; }
    friend bool operator<(const U& x, const T& y)  { return y > x; }
    friend bool operator<=(const U& x, const T& y) { return !static_cast<bool>(y < x); }
    friend bool operator>=(const U& x, const T& y) { return !static_cast<bool>(y > x); }
};

template <class T, class B>
struct less_than_comparable<T, void, B> : B
{
    template<class A> struct operator_rebase { typedef less_than_comparable<T, void, A> type; };
    friend bool operator>(const T& x, const T& y)  { return y < x; }
    friend bool operator<=(const T& x, const T& y) { return !static_cast<bool>(y < x); }
    friend bool operator>=(const T& x, const T& y) { return !static_cast<bool>(x < y); }
};

template <class T, class U = void, class B = detail::empty_base<T> >
struct equality_comparable : B
{
    template<class A> struct operator_rebase { typedef equality_comparable<T, U, A> type; };
    friend bool operator==(const U& y, const T& x) { return x == y; }
    friend bool operator!=(const U& y, const T& x) { return !static_cast<bool>(x == y); }
    friend bool operator!=(const T& y, const U& x) { return !static_cast<bool>(y == x); }
};

template <class T, class B>
struct equality_comparable<T, void, B> : B
{
    template<class A> struct operator_rebase { typedef equality_comparable<T, void, A> type; };
    friend bool operator!=(const T& x, const T& y) { return !static_cast<bool>(x == y); }
};
        
#define ZION_BINARY_OPERATOR_COMMUTATIVE( NAME, OP )                         \
template <class T, class U = void, class B = detail::empty_base<T> >        \
struct NAME : B                                                            \
{                                                                             \
template<class A> struct operator_rebase { typedef NAME<T, U, A> type; };          \
friend T operator OP( const T& lhs, const U& rhs )                          \
{ T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
friend T operator OP( const U& lhs, const T& rhs )                          \
{ T nrv( rhs ); nrv OP##= lhs; return nrv; }                              \
};                                                                            \
\
template <class T, class B >                 \
struct NAME<T, void, B> : B                                                            \
{                                                                             \
template<class A> struct operator_rebase { typedef NAME<T, void, A> type; };          \
friend T operator OP( const T& lhs, const T& rhs )                          \
{ T nrv( lhs ); nrv OP##= rhs; return nrv; }                              \
};

                
                
#define ZION_BINARY_OPERATOR_NON_COMMUTATIVE( NAME, OP )               \
template <class T, class U = void, class B = detail::empty_base<T> >        \
struct NAME : B                                                            \
{                                                                       \
template<class A> struct operator_rebase { typedef NAME<T, U, A> type; };          \
friend T operator OP( const T& lhs, const U& rhs )                    \
{ T nrv( lhs ); nrv OP##= rhs; return nrv; }                        \
};                                                                      \
\
template <class T, class U, class B = detail::empty_base<T> >  \
struct NAME##_left : B                                   \
{                                                                       \
template<class A> struct operator_rebase { typedef NAME<T, U, A> type; };          \
friend T operator OP( const U& lhs, const T& rhs )                    \
{ T nrv( lhs ); nrv OP##= rhs; return nrv; }                        \
};                                                                      \
\
template <class T, class B >                 \
struct NAME<T, void, B> : B                                                            \
{                                                                       \
template<class A> struct operator_rebase { typedef NAME<T, void, A> type; };          \
friend T operator OP( const T& lhs, const T& rhs )                    \
{ T nrv( lhs ); nrv OP##= rhs; return nrv; }                        \
};
                
ZION_BINARY_OPERATOR_COMMUTATIVE( multipliable, * )
ZION_BINARY_OPERATOR_COMMUTATIVE( addable, + )
ZION_BINARY_OPERATOR_NON_COMMUTATIVE( subtractable, - )
ZION_BINARY_OPERATOR_NON_COMMUTATIVE( dividable, / )
ZION_BINARY_OPERATOR_NON_COMMUTATIVE( modable, % )
ZION_BINARY_OPERATOR_COMMUTATIVE( xorable, ^ )
ZION_BINARY_OPERATOR_COMMUTATIVE( andable, & )
ZION_BINARY_OPERATOR_COMMUTATIVE( orable, | )
ZION_BINARY_OPERATOR_NON_COMMUTATIVE( left_shiftable, << )
ZION_BINARY_OPERATOR_NON_COMMUTATIVE( right_shiftable, >> )

template <class T, class B = detail::empty_base<T> >
struct incrementable : B
{
    template<class A> struct operator_rebase { typedef incrementable<T, A> type; };
    friend T operator++(T& x, int)
    {
        T nrv(x);
        ++x;
        return nrv;
    }
};

template <class T, class B = detail::empty_base<T> >
struct decrementable : B
{
    template<class A> struct operator_rebase { typedef decrementable<T, A> type; };
    friend T operator--(T& x, int)
    {
        T nrv(x);
        --x;
        return nrv;
    }
};

template<class T, class... Ts>
struct operators
: T::template operator_rebase<operators<Ts...> >::type
{};

template<class T>
struct operators<T>
: T
{};


}

#endif
