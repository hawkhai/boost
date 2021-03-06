
// Copyright John Maddock 2007.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/policies/policy.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/test/included/test_exec_monitor.hpp> // for test_main

template <class P1, class P2>
bool check_same(const P1&, const P2&)
{
   if(!boost::is_same<P1, P2>::value)
   {
      std::cout << "P1 = " << typeid(P1).name() << std::endl;
      std::cout << "P2 = " << typeid(P2).name() << std::endl;
   }
   return boost::is_same<P1, P2>::value;
}


int test_main(int, char* [])
{
   using namespace boost::math::policies;
   using namespace boost;

   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::domain_error_type, domain_error<ignore_error> >::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::pole_error_type, pole_error<BOOST_MATH_POLE_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::overflow_error_type, overflow_error<ignore_error> >::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::underflow_error_type, underflow_error<BOOST_MATH_UNDERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::denorm_error_type, denorm_error<BOOST_MATH_DENORM_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::evaluation_error_type, evaluation_error<BOOST_MATH_EVALUATION_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::precision_type, policy<>::precision_type>::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::promote_float_type, policy<>::promote_float_type>::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::promote_double_type, policy<>::promote_double_type>::value));
   BOOST_CHECK((is_same<policy<domain_error<ignore_error>, overflow_error<ignore_error> >::discrete_quantile_type, policy<>::discrete_quantile_type>::value));
   
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::domain_error_type, domain_error<ignore_error> >::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::pole_error_type, pole_error<BOOST_MATH_POLE_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::overflow_error_type, overflow_error<BOOST_MATH_OVERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::underflow_error_type, underflow_error<BOOST_MATH_UNDERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::denorm_error_type, denorm_error<throw_on_error> >::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::evaluation_error_type, evaluation_error<BOOST_MATH_EVALUATION_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::precision_type, policy<>::precision_type>::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::promote_float_type, policy<>::promote_float_type>::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::promote_double_type, policy<>::promote_double_type>::value));
   BOOST_CHECK((is_same<policy<denorm_error<throw_on_error>, domain_error<ignore_error> >::discrete_quantile_type, policy<>::discrete_quantile_type>::value));
   
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::domain_error_type, domain_error<ignore_error> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::pole_error_type, pole_error<BOOST_MATH_POLE_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::overflow_error_type, overflow_error<BOOST_MATH_OVERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::underflow_error_type, underflow_error<BOOST_MATH_UNDERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::denorm_error_type, denorm_error<throw_on_error> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::evaluation_error_type, evaluation_error<BOOST_MATH_EVALUATION_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::precision_type, digits2<20> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::promote_float_type, promote_float<false> >::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::promote_double_type, policy<>::promote_double_type>::value));
   BOOST_CHECK((is_same<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::discrete_quantile_type, discrete_quantile<integer_round_down> >::value));
   
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::domain_error_type, domain_error<ignore_error> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::pole_error_type, pole_error<BOOST_MATH_POLE_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::overflow_error_type, overflow_error<BOOST_MATH_OVERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::underflow_error_type, underflow_error<BOOST_MATH_UNDERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::denorm_error_type, denorm_error<throw_on_error> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::evaluation_error_type, evaluation_error<BOOST_MATH_EVALUATION_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::precision_type, digits2<20> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::promote_float_type, promote_float<false> >::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::promote_double_type, policy<>::promote_double_type>::value));
   BOOST_CHECK((is_same<normalise<policy<digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> > >::type::discrete_quantile_type, discrete_quantile<integer_round_down> >::value));
   
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::type::domain_error_type, domain_error<ignore_error> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error> >::type::pole_error_type, pole_error<BOOST_MATH_POLE_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::overflow_error_type, overflow_error<BOOST_MATH_OVERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::underflow_error_type, underflow_error<BOOST_MATH_UNDERFLOW_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::denorm_error_type, denorm_error<throw_on_error> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::evaluation_error_type, evaluation_error<BOOST_MATH_EVALUATION_ERROR_POLICY> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::precision_type, digits2<20> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::promote_float_type, promote_float<false> >::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::promote_double_type, policy<>::promote_double_type>::value));
   BOOST_CHECK((is_same<normalise<policy<>, digits2<20>, promote_float<false>, discrete_quantile<integer_round_down>, denorm_error<throw_on_error>, domain_error<ignore_error>  >::type::discrete_quantile_type, discrete_quantile<integer_round_down> >::value));

   BOOST_CHECK(check_same(make_policy(), policy<>()));
   BOOST_CHECK(check_same(make_policy(denorm_error<ignore_error>()), normalise<policy<denorm_error<ignore_error> > >::type()));
   BOOST_CHECK(check_same(make_policy(digits2<20>()), normalise<policy<digits2<20> > >::type()));
   BOOST_CHECK(check_same(make_policy(promote_float<false>()), normalise<policy<promote_float<false> > >::type()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>()), normalise<policy<domain_error<ignore_error> > >::type()));
   BOOST_CHECK(check_same(make_policy(pole_error<ignore_error>()), normalise<policy<pole_error<ignore_error> > >::type()));

   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>()), policy<domain_error<ignore_error> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>()), policy<domain_error<ignore_error>, pole_error<ignore_error> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>(), evaluation_error<ignore_error>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error>, evaluation_error<ignore_error> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>(), evaluation_error<ignore_error>(), digits2<10>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error>, evaluation_error<ignore_error>, digits2<10> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>(), evaluation_error<ignore_error>(), digits10<5>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error>, evaluation_error<ignore_error>, digits2<19> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>(), evaluation_error<ignore_error>(), digits2<10>(), promote_float<false>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error>, evaluation_error<ignore_error>, digits2<10>, promote_float<false> >()));
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>(), evaluation_error<ignore_error>(), digits2<10>(), promote_float<false>(), promote_double<false>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error>, evaluation_error<ignore_error>, digits2<10>, promote_float<false>, promote_double<false> >()));
   BOOST_CHECK(check_same(make_policy(domain_error<ignore_error>(), pole_error<ignore_error>(), overflow_error<ignore_error>(), underflow_error<throw_on_error>(), denorm_error<throw_on_error>(), evaluation_error<ignore_error>(), digits2<10>(), promote_float<false>(), promote_double<false>(), discrete_quantile<integer_round_down>()), policy<domain_error<ignore_error>, pole_error<ignore_error>, overflow_error<ignore_error>, underflow_error<throw_on_error>, denorm_error<throw_on_error>, evaluation_error<ignore_error>, digits2<10>, promote_float<false>, promote_double<false>, discrete_quantile<integer_round_down> >()));
#endif
   return 0;
} // int test_main(int, char* [])



