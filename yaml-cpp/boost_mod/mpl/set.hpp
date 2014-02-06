
#ifndef BOOST_MPL_SET_HPP_INCLUDED
#define BOOST_MPL_SET_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: set.hpp 49267 2008-10-11 06:19:02Z agurtovoy $
// $Date: 2008-10-10 23:19:02 -0700 (Fri, 10 Oct 2008) $
// $Revision: 49267 $

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "yaml-cpp/boost_mod/mpl/limits/set.hpp"
#   include "yaml-cpp/boost_mod/mpl/aux_/na.hpp"
#   include "yaml-cpp/boost_mod/mpl/aux_/config/preprocessor.hpp"

#   include "yaml-cpp/boost_mod/preprocessor/inc.hpp"
#   include "yaml-cpp/boost_mod/preprocessor/cat.hpp"
#   include "yaml-cpp/boost_mod/preprocessor/stringize.hpp"

#if !defined(BOOST_NEEDS_TOKEN_PASTING_OP_FOR_TOKENS_JUXTAPOSING)
#   define AUX778076_SET_HEADER \
    BOOST_PP_CAT(set, BOOST_MPL_LIMIT_SET_SIZE).hpp \
    /**/
#else
#   define AUX778076_SET_HEADER \
    BOOST_PP_CAT(set, BOOST_MPL_LIMIT_SET_SIZE)##.hpp \
    /**/
#endif

#   include BOOST_PP_STRINGIZE(boost_mod/mpl/set/AUX778076_SET_HEADER)
#   undef AUX778076_SET_HEADER
#endif

#include "yaml-cpp/boost_mod/mpl/aux_/config/use_preprocessed.hpp"

#if !defined(BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS) \
    && !defined(BOOST_MPL_PREPROCESSING_MODE)

#   define BOOST_MPL_PREPROCESSED_HEADER set.hpp
#   include "yaml-cpp/boost_mod/mpl/aux_/include_preprocessed.hpp"

#else

#   include "yaml-cpp/boost_mod/mpl/limits/set.hpp"

#   define AUX778076_SEQUENCE_NAME set
#   define AUX778076_SEQUENCE_LIMIT BOOST_MPL_LIMIT_SET_SIZE
#   include "yaml-cpp/boost_mod/mpl/aux_/sequence_wrapper.hpp"

#endif // BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#endif // BOOST_MPL_SET_HPP_INCLUDED
