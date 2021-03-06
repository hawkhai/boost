
// Copyright 2005-2007 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "./config.hpp"

#ifdef TEST_STD_INCLUDES
#  include <functional>
#else
#  include <boost/functional/hash.hpp>
#endif

#include <boost/detail/lightweight_test.hpp>

#include <cmath>
#include <boost/limits.hpp>

#include <iostream>

#if defined(BOOST_MSVC)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

template <class T>
void float_tests(char const* name, T* = 0)
{
    std::cerr<<"\n"
        <<"Testing " BOOST_STRINGIZE(HASH_NAMESPACE) "::hash<"<<name<<">\n"
        <<"\n"
        <<"std::numeric_limits<T>::digits = "
            <<std::numeric_limits<T>::digits<<"\n"
        <<"std::numeric_limits<int>::digits = "
            <<std::numeric_limits<int>::digits<<"\n"
        <<"std::numeric_limits<std::size_t>::digits = "
            <<std::numeric_limits<std::size_t>::digits<<"\n"
        <<"\n"
        ;

    HASH_NAMESPACE::hash<T> x1;

    T zero = 0;
    T minus_zero = (T) -1 * zero;

    BOOST_TEST(zero == minus_zero);
    BOOST_TEST(x1(zero) == x1(minus_zero));

    BOOST_TEST(x1(zero) == HASH_NAMESPACE::hash_value(zero));
    BOOST_TEST(x1(minus_zero) == HASH_NAMESPACE::hash_value(minus_zero));

    using namespace std;

// Doing anything with infinity causes borland to crash.
#if defined(__BORLANDC__)
    std::cerr<<"Not running infinity checks on Borland, as it causes it to crash.\n";
#else
    if(std::numeric_limits<T>::has_infinity) {
        T infinity = -log(zero);
        T infinity2 = (T) 1. / zero;
        T infinity3 = (T) -1. / minus_zero;
        T infinity4 = std::numeric_limits<T>::infinity();

        T minus_infinity = log(zero);
        T minus_infinity2 = (T) -1. / zero;
        T minus_infinity3 = (T) 1. / minus_zero;

        BOOST_TEST(x1(infinity) == HASH_NAMESPACE::hash_value(infinity));
        BOOST_TEST(x1(minus_infinity)
                == HASH_NAMESPACE::hash_value(minus_infinity));

        if(infinity == infinity2)
            BOOST_TEST(x1(infinity) == x1(infinity2));
        if(infinity == infinity3)
            BOOST_TEST(x1(infinity) == x1(infinity3));
        if(infinity == infinity4)
            BOOST_TEST(x1(infinity) == x1(infinity4));

        if(minus_infinity == minus_infinity2)
            BOOST_TEST(x1(minus_infinity) == x1(minus_infinity2));
        if(minus_infinity == minus_infinity3)
            BOOST_TEST(x1(minus_infinity) == x1(minus_infinity3));

        BOOST_TEST(infinity != minus_infinity);

        if(x1(infinity) == x1(minus_infinity)) {
            std::cerr<<"x1(infinity) == x1(-infinity) == "<<x1(infinity)<<"\n";
        }

        // This should really be 'has_denorm == denorm_present' but some
        // compilers don't have 'denorm_present'. See also a later use.
        if(std::numeric_limits<T>::has_denorm) {
            if(x1(std::numeric_limits<T>::denorm_min()) == x1(infinity)) {
                std::cerr<<"x1(denorm_min) == x1(infinity) == "<<x1(infinity)<<"\n";
            }
            if(x1(std::numeric_limits<T>::denorm_min()) == x1(minus_infinity)) {
                std::cerr<<"x1(denorm_min) == x1(-infinity) == "<<x1(minus_infinity)<<"\n";
            }
        }
        if(std::numeric_limits<T>::has_quiet_NaN) {
            if(x1(std::numeric_limits<T>::quiet_NaN()) == x1(infinity)) {
                std::cerr<<"x1(quiet_NaN) == x1(infinity) == "<<x1(infinity)<<"\n";
            }
            if(x1(std::numeric_limits<T>::quiet_NaN()) == x1(minus_infinity)) {
                std::cerr<<"x1(quiet_NaN) == x1(-infinity) == "<<x1(minus_infinity)<<"\n";
            }
        }
    }
#endif

    T max = (std::numeric_limits<T>::max)();
    T half_max = max / 2;
    T quarter_max = max / 4;
    T three_quarter_max = max - quarter_max;

    BOOST_TEST(x1(max) == HASH_NAMESPACE::hash_value(max));
    BOOST_TEST(x1(half_max) == HASH_NAMESPACE::hash_value(half_max));
    BOOST_TEST(x1(quarter_max) == HASH_NAMESPACE::hash_value(quarter_max));
    BOOST_TEST(x1(three_quarter_max) == HASH_NAMESPACE::hash_value(three_quarter_max));

    // The '!=' tests could legitimately fail, but with my hash it indicates a bug.
    BOOST_TEST(x1(max) == x1(max));
    BOOST_TEST(x1(max) != x1(quarter_max));
    BOOST_TEST(x1(max) != x1(half_max));
    BOOST_TEST(x1(max) != x1(three_quarter_max));
    BOOST_TEST(x1(quarter_max) == x1(quarter_max));
    BOOST_TEST(x1(quarter_max) != x1(half_max));
    BOOST_TEST(x1(quarter_max) != x1(three_quarter_max));
    BOOST_TEST(x1(half_max) == x1(half_max));
    BOOST_TEST(x1(half_max) != x1(three_quarter_max));
    BOOST_TEST(x1(three_quarter_max) == x1(three_quarter_max));

// Intel with gcc stdlib sometimes segfaults on calls to asin and acos.
#if !((defined(__INTEL_COMPILER) || defined(__ICL) || \
        defined(__ICC) || defined(__ECC)) && \
    (defined(__GLIBCPP__) || defined(__GLIBCXX__)))
    T v1 = asin((T) 1);
    T v2 = acos((T) 0);
    if(v1 == v2)
        BOOST_TEST(x1(v1) == x1(v2));
    BOOST_TEST(x1(v1) == HASH_NAMESPACE::hash_value(v1));
    BOOST_TEST(x1(v2) == HASH_NAMESPACE::hash_value(v2));
#endif

    BOOST_TEST(x1(std::numeric_limits<T>::epsilon()) ==
            HASH_NAMESPACE::hash_value(std::numeric_limits<T>::epsilon()));

    BOOST_TEST(std::numeric_limits<T>::epsilon() != (T) 0);
    if(x1(std::numeric_limits<T>::epsilon()) == x1((T) 0))
        std::cerr<<"x1(epsilon) == x1(0) == "<<x1((T) 0)<<"\n";

    BOOST_TEST(-std::numeric_limits<T>::epsilon() != (T) 0);
    if(x1(-std::numeric_limits<T>::epsilon()) == x1((T) 0))
        std::cerr<<"x1(-epsilon) == x1(0) == "<<x1((T) 0)<<"\n";

    BOOST_TEST((T) 1 + std::numeric_limits<T>::epsilon() != (T) 1);
    if(x1((T) 1 + std::numeric_limits<T>::epsilon()) == x1((T) 1))
        std::cerr<<"x1(1 + epsilon) == x1(1) == "<<x1((T) 1)<<"\n";

    BOOST_TEST((T) 1 - std::numeric_limits<T>::epsilon() != (T) 1);
    if(x1((T) 1 - std::numeric_limits<T>::epsilon()) == x1((T) 1))
        std::cerr<<"x1(1 - epsilon) == x1(1) == "<<x1((T) 1)<<"\n";

    BOOST_TEST((T) -1 + std::numeric_limits<T>::epsilon() != (T) -1);
    if(x1((T) -1 + std::numeric_limits<T>::epsilon()) == x1((T) -1))
        std::cerr<<"x1(-1 + epsilon) == x1(-1) == "<<x1((T) -1)<<"\n";

    BOOST_TEST((T) -1 - std::numeric_limits<T>::epsilon() != (T) -1);
    if(x1((T) -1 - std::numeric_limits<T>::epsilon()) == x1((T) -1))
        std::cerr<<"x1(-1 - epsilon) == x1(-1) == "<<x1((T) -1)<<"\n";

    // As before.
    if(std::numeric_limits<T>::has_denorm) {
        if(x1(std::numeric_limits<T>::denorm_min()) == x1(zero)) {
            std::cerr<<"x1(denorm_min) == x1(zero) == "<<x1(zero)<<"\n";
        }
#if !BOOST_WORKAROUND(__DECCXX_VER,<70190006)
        // The Tru64/CXX standard library prior to 7.1 contains a bug in the
        // specialization of std::numeric_limits::denorm_min() for long
        // doubles which causes this test to fail.
        if(x1(std::numeric_limits<T>::denorm_min()) !=
            HASH_NAMESPACE::hash_value(std::numeric_limits<T>::denorm_min()))
        {
            std::cerr<<"x1(std::numeric_limits<T>::denorm_min()) = "
                << x1(std::numeric_limits<T>::denorm_min())
                << "\nhash_value(std::numeric_limits<T>::denorm_min()) = "
                << HASH_NAMESPACE::hash_value(
                        std::numeric_limits<T>::denorm_min())
                << "\nx1(0) = "<<x1(0)<<"\n";
        }
#endif
    }

// NaN also causes borland to crash.
#if !defined(__BORLANDC__)
    if(std::numeric_limits<T>::has_quiet_NaN) {
        if(x1(std::numeric_limits<T>::quiet_NaN()) == x1(1.0)) {
            std::cerr<<"x1(quiet_NaN) == x1(1.0) == "<<x1(1.0)<<"\n";
        }
        BOOST_TEST(x1(std::numeric_limits<T>::quiet_NaN()) ==
            HASH_NAMESPACE::hash_value(std::numeric_limits<T>::quiet_NaN()));
    }
#endif
}

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif
