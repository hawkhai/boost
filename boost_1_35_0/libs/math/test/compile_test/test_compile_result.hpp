//  Copyright John Maddock 2007.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//
// Note this header must NOT include any other headers, for its
// use to be meaningful (because we use it in tests designed to
// detect missing includes).
//

static const float f = 0;
static const double d = 0;
static const long double l = 0;
static const unsigned u = 0;
static const int i = 0;

//template <class T>
//inline void check_result_imp(T, T){}

inline void check_result_imp(float, float){}
inline void check_result_imp(double, double){}
inline void check_result_imp(long double, long double){}
inline void check_result_imp(int, int){}
inline void check_result_imp(bool, bool){}

template <class T1, class T2>
inline void check_result_imp(T1, T2)
{
   typedef int static_assertion[sizeof(T1) == 0xFFFF];
}

template <class T1, class T2>
inline void check_result(T2)
{
   T1 a = T1();
   T2 b = T2();
   return check_result_imp(a, b);
}


template <class Distribution>
struct DistributionConcept
{
   static void constraints()
   {
      typedef typename Distribution::value_type value_type;

      const Distribution& dist = DistributionConcept<Distribution>::get_object();

      value_type x = 0;
       // The result values are ignored in all these checks.
      check_result<value_type>(cdf(dist, x));
      check_result<value_type>(cdf(complement(dist, x)));
      check_result<value_type>(pdf(dist, x));
      check_result<value_type>(quantile(dist, x));
      check_result<value_type>(quantile(complement(dist, x)));
      check_result<value_type>(mean(dist));
      check_result<value_type>(mode(dist));
      check_result<value_type>(standard_deviation(dist));
      check_result<value_type>(variance(dist));
      check_result<value_type>(hazard(dist, x));
      check_result<value_type>(chf(dist, x));
      check_result<value_type>(coefficient_of_variation(dist));
      check_result<value_type>(skewness(dist));
      check_result<value_type>(kurtosis(dist));
      check_result<value_type>(kurtosis_excess(dist));
      check_result<value_type>(median(dist));
      //
      // we can't actually test that at std::pair is returned from these
      // because that would mean including some std lib headers....
      //
      range(dist);
      support(dist);

      check_result<value_type>(cdf(dist, f));
      check_result<value_type>(cdf(complement(dist, f)));
      check_result<value_type>(pdf(dist, f));
      check_result<value_type>(quantile(dist, f));
      check_result<value_type>(quantile(complement(dist, f)));
      check_result<value_type>(hazard(dist, f));
      check_result<value_type>(chf(dist, f));
      check_result<value_type>(cdf(dist, d));
      check_result<value_type>(cdf(complement(dist, d)));
      check_result<value_type>(pdf(dist, d));
      check_result<value_type>(quantile(dist, d));
      check_result<value_type>(quantile(complement(dist, d)));
      check_result<value_type>(hazard(dist, d));
      check_result<value_type>(chf(dist, d));
      check_result<value_type>(cdf(dist, l));
      check_result<value_type>(cdf(complement(dist, l)));
      check_result<value_type>(pdf(dist, l));
      check_result<value_type>(quantile(dist, l));
      check_result<value_type>(quantile(complement(dist, l)));
      check_result<value_type>(hazard(dist, l));
      check_result<value_type>(chf(dist, l));
      check_result<value_type>(cdf(dist, i));
      check_result<value_type>(cdf(complement(dist, i)));
      check_result<value_type>(pdf(dist, i));
      check_result<value_type>(quantile(dist, i));
      check_result<value_type>(quantile(complement(dist, i)));
      check_result<value_type>(hazard(dist, i));
      check_result<value_type>(chf(dist, i));
      unsigned long li = 1;
      check_result<value_type>(cdf(dist, li));
      check_result<value_type>(cdf(complement(dist, li)));
      check_result<value_type>(pdf(dist, li));
      check_result<value_type>(quantile(dist, li));
      check_result<value_type>(quantile(complement(dist, li)));
      check_result<value_type>(hazard(dist, li));
      check_result<value_type>(chf(dist, li));
   }
private:
   static Distribution* get_object_p();
   static Distribution& get_object()
   {
      // will never get called:
      return *get_object_p();
   }
}; // struct DistributionConcept

#ifndef BOOST_MATH_NO_LONG_DOUBLE_MATH_FUNCTIONS
#define TEST_DIST_FUNC(dist)\
   DistributionConcept< boost::math::dist##_distribution<float> >::constraints();\
   DistributionConcept< boost::math::dist##_distribution<double> >::constraints();\
   DistributionConcept< boost::math::dist##_distribution<long double> >::constraints();
#else
#define TEST_DIST_FUNC(dist)\
   DistributionConcept< boost::math::dist##_distribution<float> >::constraints();\
   DistributionConcept< boost::math::dist##_distribution<double> >::constraints();
#endif
