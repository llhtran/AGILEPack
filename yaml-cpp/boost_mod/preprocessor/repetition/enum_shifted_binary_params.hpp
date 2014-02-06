# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2005.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef BOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_BINARY_PARAMS_HPP
# define BOOST_PREPROCESSOR_REPETITION_ENUM_SHIFTED_BINARY_PARAMS_HPP
#
# include "yaml-cpp/boost_mod/preprocessor/arithmetic/dec.hpp"
# include "yaml-cpp/boost_mod/preprocessor/arithmetic/inc.hpp"
# include "yaml-cpp/boost_mod/preprocessor/cat.hpp"
# include "yaml-cpp/boost_mod/preprocessor/config/config.hpp"
# include "yaml-cpp/boost_mod/preprocessor/punctuation/comma_if.hpp"
# include "yaml-cpp/boost_mod/preprocessor/repetition/repeat.hpp"
# include "yaml-cpp/boost_mod/preprocessor/tuple/elem.hpp"
# include "yaml-cpp/boost_mod/preprocessor/tuple/rem.hpp"
#
# /* BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(count, p1, p2) BOOST_PP_REPEAT(BOOST_PP_DEC(count), BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# else
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS(count, p1, p2) BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_I(count, p1, p2)
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_I(count, p1, p2) BOOST_PP_REPEAT(BOOST_PP_DEC(count), BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# endif
#
# if BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_STRICT()
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M(z, n, pp) BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_IM(z, n, BOOST_PP_TUPLE_REM_2 pp)
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_IM(z, n, im) BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_I(z, n, im)
# else
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M(z, n, pp) BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_I(z, n, BOOST_PP_TUPLE_ELEM(2, 0, pp), BOOST_PP_TUPLE_ELEM(2, 1, pp))
# endif
#
# define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M_I(z, n, p1, p2) BOOST_PP_COMMA_IF(n) BOOST_PP_CAT(p1, BOOST_PP_INC(n)) BOOST_PP_CAT(p2, BOOST_PP_INC(n))
#
# /* BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z */
#
# if ~BOOST_PP_CONFIG_FLAGS() & BOOST_PP_CONFIG_EDG()
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z(z, count, p1, p2) BOOST_PP_REPEAT_ ## z(BOOST_PP_DEC(count), BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# else
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z(z, count, p1, p2) BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z_I(z, count, p1, p2)
#    define BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_Z_I(z, count, p1, p2) BOOST_PP_REPEAT_ ## z(BOOST_PP_DEC(count), BOOST_PP_ENUM_SHIFTED_BINARY_PARAMS_M, (p1, p2))
# endif
#
# endif
