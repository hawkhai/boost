//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/math/concepts/real_concept.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/tools/test.hpp>
#include "functor.hpp"
#include <boost/array.hpp>

#include "handle_test_result.hpp"

//
// DESCRIPTION:
// ~~~~~~~~~~~~
//
// This file tests the function binomial_coefficient<T>.  
// The accuracy tests
// use values generated with NTL::RR at 1000-bit precision
// and our generic versions of these function.
//
// Note that when this file is first run on a new platform many of
// these tests will fail: the default accuracy is 1 epsilon which
// is too tight for most platforms.  In this situation you will 
// need to cast a human eye over the error rates reported and make
// a judgement as to whether they are acceptable.  Either way please
// report the results to the Boost mailing list.  Acceptable rates of
// error are marked up below as a series of regular expressions that
// identify the compiler/stdlib/platform/data-type/test-data/test-function
// along with the maximum expected peek and RMS mean errors for that
// test.
//

void expected_results()
{
   //
   // Define the max and mean errors expected for
   // various compilers and platforms.
   //
   const char* largest_type;
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   if(boost::math::policies::digits<double, boost::math::policies::policy<> >() == boost::math::policies::digits<long double, boost::math::policies::policy<> >())
   {
      largest_type = "(long\\s+)?double";
   }
   else
   {
      largest_type = "long double";
   }
#else
   largest_type = "(long\\s+)?double";
#endif
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      largest_type,                  // test type(s)
      ".*large.*",                   // test data group
      ".*", 100, 20);                 // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      ".*large.*",                   // test data group
      ".*", 200, 100);               // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      "real_concept",                // test type(s)
      ".*",                          // test data group
      ".*", 100, 30);                 // test function
   add_expected_result(
      ".*",                          // compiler
      ".*",                          // stdlib
      ".*",                          // platform
      ".*",                          // test type(s)
      ".*",                          // test data group
      ".*", 2, 1);                   // test function

   //
   // Finish off by printing out the compiler/stdlib/platform names,
   // we do this to make it easier to mark up expected error rates.
   //
   std::cout << "Tests run with " << BOOST_COMPILER << ", " 
      << BOOST_STDLIB << ", " << BOOST_PLATFORM << std::endl;
}

template <class T>
T binomial_wrapper(T n, T k)
{
   return boost::math::binomial_coefficient<T>(
      boost::math::tools::real_cast<unsigned>(n),
      boost::math::tools::real_cast<unsigned>(k));
}

template <class T>
void test_binomial(T, const char* type_name)
{
   using namespace std;

   typedef T (*func_t)(T, T);
#if defined(BOOST_MATH_NO_DEDUCED_FUNCTION_POINTERS)
   func_t f = &binomial_wrapper<T>;
#else
   func_t f = &binomial_wrapper;
#endif

#include "binomial_data.ipp"

   boost::math::tools::test_result<T> result = boost::math::tools::test(
      binomial_data, 
      bind_func(f, 0, 1), 
      extract_result(2));

   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
      "Test results for small arguments and type " << type_name << std::endl << std::endl;
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
   handle_test_result(result, binomial_data[result.worst()], result.worst(), type_name, "binomial_coefficient", "Binomials: small arguments");
   std::cout << std::endl;

#include "binomial_large_data.ipp"

   result = boost::math::tools::test(
      binomial_large_data, 
      bind_func(f, 0, 1), 
      extract_result(2));

   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
      "Test results for large arguments and type " << type_name << std::endl << std::endl;
   std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
   handle_test_result(result, binomial_large_data[result.worst()], result.worst(), type_name, "binomial_coefficient", "Binomials: large arguments");
   std::cout << std::endl;
}

template <class T>
void test_spots(T, const char* name)
{
   T tolerance = boost::math::tools::epsilon<T>() * 50 * 100;  // 50 eps as a percentage
   if(!std::numeric_limits<T>::is_specialized)
      tolerance *= 10;  // beta function not so accurate without lanczos support

   std::cout << "Testing spot checks for type " << name << " with tolerance " << tolerance << "%\n";

   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 0), static_cast<T>(1));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 1), static_cast<T>(20));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 2), static_cast<T>(190));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 3), static_cast<T>(1140));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 20), static_cast<T>(1));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 19), static_cast<T>(20));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 18), static_cast<T>(190));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 17), static_cast<T>(1140));
   BOOST_CHECK_EQUAL(boost::math::binomial_coefficient<T>(20, 10), static_cast<T>(184756L));

   BOOST_CHECK_CLOSE(boost::math::binomial_coefficient<T>(100, 5), static_cast<T>(7.528752e7L), tolerance);
   BOOST_CHECK_CLOSE(boost::math::binomial_coefficient<T>(100, 81), static_cast<T>(1.323415729392122674e20L), tolerance);

   BOOST_CHECK_CLOSE(boost::math::binomial_coefficient<T>(300, 3), static_cast<T>(4.45510e6L), tolerance);
   BOOST_CHECK_CLOSE(boost::math::binomial_coefficient<T>(300, 7), static_cast<T>(4.043855956140000e13L), tolerance);
   BOOST_CHECK_CLOSE(boost::math::binomial_coefficient<T>(300, 290), static_cast<T>(1.3983202332417017700000000e18L), tolerance);
   BOOST_CHECK_CLOSE(boost::math::binomial_coefficient<T>(300, 275), static_cast<T>(1.953265141442868389822364184842211512000000e36L), tolerance);
}

int test_main(int, char* [])
{
   expected_results();
   BOOST_MATH_CONTROL_FP;

   test_spots(1.0F, "float");
   test_spots(1.0, "double");
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_spots(1.0L, "long double");
   test_spots(boost::math::concepts::real_concept(), "real_concept");
#endif

   test_binomial(1.0F, "float");
   test_binomial(1.0, "double");
#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
   test_binomial(1.0L, "long double");
#ifndef BOOST_MATH_NO_REAL_CONCEPT_TESTS
   test_binomial(boost::math::concepts::real_concept(), "real_concept");
#endif
#else
   std::cout << "<note>The long double tests have been disabled on this platform "
      "either because the long double overloads of the usual math functions are "
      "not available at all, or because they are too inaccurate for these tests "
      "to pass.</note>" << std::cout;
#endif
   return 0;
}



