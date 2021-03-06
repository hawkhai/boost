// Copyright Paul A. Bristow 2006-7.
// Copyright John Maddock 2006-7.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Test error handling mechanism produces the expected error messages.
// for example Error in function boost::math::test_function<float>(float, float, float): Domain Error evaluating function at 0

// Define some custom dummy error handlers that do nothing but throw,
// in order to check that they are otherwise undefined.
// The user MUST define them before they can be used.
//
struct user_defined_error{};

namespace boost{ namespace math{ namespace policies{

template <class T>
T user_domain_error(const char* , const char* , const T& )
{
   throw user_defined_error();
}

template <class T>
T user_pole_error(const char* , const char* , const T& )
{
   throw user_defined_error();
}

template <class T>
T user_overflow_error(const char* , const char* , const T& )
{
   throw user_defined_error();
}

template <class T>
T user_underflow_error(const char* , const char* , const T& )
{
   throw user_defined_error();
}

template <class T>
T user_denorm_error(const char* , const char* , const T& )
{
   throw user_defined_error();
}

template <class T>
T user_evaluation_error(const char* , const char* , const T& )
{
   throw user_defined_error();
}

}}} // namespaces

#include <boost/math/policies/policy.hpp>
#include <boost/math/policies/error_handling.hpp>
#include <boost/math/concepts/real_concept.hpp>
#include <boost/test/included/test_exec_monitor.hpp> // for test_main
//
// Define some policies:
//
using namespace boost::math::policies;
policy<
   domain_error<throw_on_error>,
   pole_error<throw_on_error>,
   overflow_error<throw_on_error>,
   underflow_error<throw_on_error>,
   denorm_error<throw_on_error>,
   evaluation_error<throw_on_error> > throw_policy;
policy<
   domain_error<errno_on_error>,
   pole_error<errno_on_error>,
   overflow_error<errno_on_error>,
   underflow_error<errno_on_error>,
   denorm_error<errno_on_error>,
   evaluation_error<errno_on_error> > errno_policy;
policy<
   domain_error<user_error>,
   pole_error<user_error>,
   overflow_error<user_error>,
   underflow_error<user_error>,
   denorm_error<user_error>,
   evaluation_error<user_error> > user_policy;
policy<> default_policy;

#define TEST_EXCEPTION(expression, exception)\
   BOOST_CHECK_THROW(expression, exception);\
   try{ expression; }catch(const exception& e){ std::cout << e.what() << std::endl; }

template <class T>
void test_error(T)
{
   const char* func = "boost::math::test_function<%1%>(%1%, %1%, %1%)";
   const char* msg1 = "Error while handling value %1%";
   const char* msg2 = "Error message goes here...";

   // Check that exception is thrown, catch and show the message, for example:
   // Error in function boost::math::test_function<float>(float, float, float): Error while handling value 0

   TEST_EXCEPTION(boost::math::policies::raise_domain_error(func, msg1, T(0.0), throw_policy), std::domain_error);
   TEST_EXCEPTION(boost::math::policies::raise_domain_error(func, 0, T(0.0), throw_policy), std::domain_error);
   TEST_EXCEPTION(boost::math::policies::raise_pole_error(func, msg1, T(0.0), throw_policy), std::domain_error);
   TEST_EXCEPTION(boost::math::policies::raise_pole_error(func, 0, T(0.0), throw_policy), std::domain_error);
   TEST_EXCEPTION(boost::math::policies::raise_overflow_error<T>(func, msg2, throw_policy), std::overflow_error);
   TEST_EXCEPTION(boost::math::policies::raise_overflow_error<T>(func, 0, throw_policy), std::overflow_error);
   TEST_EXCEPTION(boost::math::policies::raise_underflow_error<T>(func, msg2, throw_policy), std::underflow_error);
   TEST_EXCEPTION(boost::math::policies::raise_underflow_error<T>(func, 0, throw_policy), std::underflow_error);
   TEST_EXCEPTION(boost::math::policies::raise_denorm_error<T>(func, msg2, T(0), throw_policy), std::underflow_error);
   TEST_EXCEPTION(boost::math::policies::raise_denorm_error<T>(func, 0, T(0), throw_policy), std::underflow_error);
   TEST_EXCEPTION(boost::math::policies::raise_evaluation_error(func, msg1, T(1.25), throw_policy), boost::math::evaluation_error);
   TEST_EXCEPTION(boost::math::policies::raise_evaluation_error(func, 0, T(1.25), throw_policy), boost::math::evaluation_error);
   //
   // Now try user error handlers: these should all throw user_error():
   // - because by design these are undefined and must be defined by the user ;-)
   BOOST_CHECK_THROW(boost::math::policies::raise_domain_error(func, msg1, T(0.0), user_policy), user_defined_error);
   BOOST_CHECK_THROW(boost::math::policies::raise_pole_error(func, msg1, T(0.0), user_policy), user_defined_error);
   BOOST_CHECK_THROW(boost::math::policies::raise_overflow_error<T>(func, msg2, user_policy), user_defined_error);
   BOOST_CHECK_THROW(boost::math::policies::raise_underflow_error<T>(func, msg2, user_policy), user_defined_error);
   BOOST_CHECK_THROW(boost::math::policies::raise_denorm_error<T>(func, msg2, T(0), user_policy), user_defined_error);
   BOOST_CHECK_THROW(boost::math::policies::raise_evaluation_error(func, msg1, T(0.0), user_policy), user_defined_error);

}

int test_main(int, char* [])
{
   // Test error handling.
   // (Parameter value, arbitrarily zero, only communicates the floating point type FPT).
   test_error(0.0F); // Test float.
   test_error(0.0); // Test double.
   test_error(0.0L); // Test long double.
  test_error(boost::math::concepts::real_concept(0.0L)); // Test concepts.
   return 0;
} // int test_main(int, char* [])

/*

Autorun "i:\boost-06-05-03-1300\libs\math\test\Math_test\debug\test_error_handling.exe"
Running 1 test case...
Error in function boost::math::test_function<float>(float, float, float): Error while handling value 0
Error in function boost::math::test_function<float>(float, float, float): Domain Error evaluating function at 0
Error in function boost::math::test_function<float>(float, float, float): Error while handling value 0
Error in function boost::math::test_function<float>(float, float, float): Evaluation of function at pole 0
Error in function boost::math::test_function<float>(float, float, float): Error message goes here...
Error in function boost::math::test_function<float>(float, float, float): Overflow Error
Error in function boost::math::test_function<float>(float, float, float): Error message goes here...
Error in function boost::math::test_function<float>(float, float, float): Underflow Error
Error in function boost::math::test_function<float>(float, float, float): Error message goes here...
Error in function boost::math::test_function<float>(float, float, float): Denorm Error
Error in function boost::math::test_function<float>(float, float, float): Error while handling value 1.25
Error in function boost::math::test_function<float>(float, float, float): Internal Evaluation Error, best value so far was 1.25
Error in function boost::math::test_function<double>(double, double, double): Error while handling value 0
Error in function boost::math::test_function<double>(double, double, double): Domain Error evaluating function at 0
Error in function boost::math::test_function<double>(double, double, double): Error while handling value 0
Error in function boost::math::test_function<double>(double, double, double): Evaluation of function at pole 0
Error in function boost::math::test_function<double>(double, double, double): Error message goes here...
Error in function boost::math::test_function<double>(double, double, double): Overflow Error
Error in function boost::math::test_function<double>(double, double, double): Error message goes here...
Error in function boost::math::test_function<double>(double, double, double): Underflow Error
Error in function boost::math::test_function<double>(double, double, double): Error message goes here...
Error in function boost::math::test_function<double>(double, double, double): Denorm Error
Error in function boost::math::test_function<double>(double, double, double): Error while handling value 1.25
Error in function boost::math::test_function<double>(double, double, double): Internal Evaluation Error, best value so far was 1.25
Error in function boost::math::test_function<long double>(long double, long double, long double): Error while handling value 0
Error in function boost::math::test_function<long double>(long double, long double, long double): Domain Error evaluating function at 0
Error in function boost::math::test_function<long double>(long double, long double, long double): Error while handling value 0
Error in function boost::math::test_function<long double>(long double, long double, long double): Evaluation of function at pole 0
Error in function boost::math::test_function<long double>(long double, long double, long double): Error message goes here...
Error in function boost::math::test_function<long double>(long double, long double, long double): Overflow Error
Error in function boost::math::test_function<long double>(long double, long double, long double): Error message goes here...
Error in function boost::math::test_function<long double>(long double, long double, long double): Underflow Error
Error in function boost::math::test_function<long double>(long double, long double, long double): Error message goes here...
Error in function boost::math::test_function<long double>(long double, long double, long double): Denorm Error
Error in function boost::math::test_function<long double>(long double, long double, long double): Error while handling value 1.25
Error in function boost::math::test_function<long double>(long double, long double, long double): Internal Evaluation Error, best value so far was 1.25
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Error while handling value 0
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Domain Error evaluating function at 0
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Error while handling value 0
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Evaluation of function at pole 0
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Error message goes here...
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Overflow Error
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Error message goes here...
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Underflow Error
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Error message goes here...
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Denorm Error
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Error while handling value 1.25
Error in function boost::math::test_function<class boost::math::concepts::real_concept>(class boost::math::concepts::real_concept, class boost::math::concepts::real_concept, class boost::math::concepts::real_concept): Internal Evaluation Error, best value so far was 1.25
*** No errors detected

*/

