
#ifndef BOOST_MPL_LAMBDA_FWD_HPP_INCLUDED
#define BOOST_MPL_LAMBDA_FWD_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: lambda_fwd.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#include "yaml-cpp/boost_mod/mpl/void_fwd.hpp"
#include "yaml-cpp/boost_mod/mpl/aux_/na.hpp"
#include "yaml-cpp/boost_mod/mpl/aux_/config/lambda.hpp"

#if !defined(BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT)

#   include "yaml-cpp/boost_mod/mpl/int.hpp"
#   include "yaml-cpp/boost_mod/mpl/aux_/lambda_arity_param.hpp"
#   include "yaml-cpp/boost_mod/mpl/aux_/template_arity_fwd.hpp"

namespace boost { namespace mpl {

template< 
      typename T = na
    , typename Tag = void_
    BOOST_MPL_AUX_LAMBDA_ARITY_PARAM(
          typename Arity = int_< aux::template_arity<T>::value >
        )
    >
struct lambda;

}}

#else // BOOST_MPL_CFG_NO_FULL_LAMBDA_SUPPORT

#   include "yaml-cpp/boost_mod/mpl/bool.hpp"

namespace boost { namespace mpl {

template< 
      typename T = na
    , typename Tag = void_
    , typename Protect = true_
    > 
struct lambda;

}}

#endif

#endif // BOOST_MPL_LAMBDA_FWD_HPP_INCLUDED
