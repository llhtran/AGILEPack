
#ifndef BOOST_MPL_COUNT_IF_HPP_INCLUDED
#define BOOST_MPL_COUNT_IF_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2002
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: count_if.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include "yaml-cpp/boost_mod/mpl/fold.hpp"
#include "yaml-cpp/boost_mod/mpl/next.hpp"
#include "yaml-cpp/boost_mod/mpl/integral_c.hpp"
#include "yaml-cpp/boost_mod/mpl/identity.hpp"
#include "yaml-cpp/boost_mod/mpl/eval_if.hpp"
#include "yaml-cpp/boost_mod/mpl/apply.hpp"
#include "yaml-cpp/boost_mod/mpl/aux_/msvc_eti_base.hpp"
#include "yaml-cpp/boost_mod/mpl/aux_/na_spec.hpp"
#include "yaml-cpp/boost_mod/mpl/aux_/lambda_support.hpp"
#include "yaml-cpp/boost_mod/mpl/aux_/config/forwarding.hpp"

namespace boost { namespace mpl {

namespace aux {

template< typename Predicate >
struct next_if
{
    template<
          typename N
        , typename T
        >
    struct apply
#if !defined(BOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : eval_if<
              typename apply1<Predicate,T>::type
            , next<N>
            , identity<N>
            >
    {
#else
    {
        typedef typename eval_if<
              typename apply1<Predicate,T>::type
            , next<N>
            , identity<N>
            >::type type;
#endif
    };
};

} // namespace aux


template<
      typename BOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename BOOST_MPL_AUX_NA_PARAM(Predicate)
    >
struct count_if
    : aux::msvc_eti_base< typename fold<
          Sequence
        , integral_c<unsigned long,0>
        , protect< aux::next_if<Predicate> >
        >::type >
{
    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,count_if,(Sequence,Predicate))
};

BOOST_MPL_AUX_NA_SPEC(2, count_if)

}}

#endif // BOOST_MPL_COUNT_IF_HPP_INCLUDED
