
#ifndef BOOST_MPL_SET_SET20_C_HPP_INCLUDED
#define BOOST_MPL_SET_SET20_C_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: set20_c.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "yaml-cpp/boost_mod/mpl/set/set10_c.hpp"
#   include "yaml-cpp/boost_mod/mpl/set/set20.hpp"
#endif

#include "yaml-cpp/boost_mod/mpl/aux_/config/use_preprocessed.hpp"

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER set20_c.hpp
#   include "yaml-cpp/boost_mod/mpl/set/aux_/include_preprocessed.hpp"

#else

#   include "yaml-cpp/boost_mod/preprocessor/iterate.hpp"

namespace boost { namespace mpl {

#   define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(11, 20, "yaml-cpp/boost_mod/mpl/set/aux_/numbered_c.hpp"))
#   include BOOST_PP_ITERATE()

}}

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS

#endif // BOOST_MPL_SET_SET20_C_HPP_INCLUDED
