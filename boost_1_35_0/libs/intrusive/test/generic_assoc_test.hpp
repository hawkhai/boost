/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2007.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#include <vector> //vector
#include <algorithm> //sort, random_shuffle
#include <boost/intrusive/detail/config_begin.hpp>
#include "common_functors.hpp"
#include <boost/intrusive/options.hpp>
#include <boost/detail/lightweight_test.hpp>
#include "test_macros.hpp"
#include "test_container.hpp"

namespace boost{
namespace intrusive{
namespace test{

template<class T>
struct has_splay
{
   static const bool value = false;
};

template<class T>
struct has_rebalance
{
   static const bool value = false;
};

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
struct test_generic_assoc 
{
   typedef typename ValueTraits::value_type value_type;
   static void test_all(std::vector<value_type>& values);
   static void test_clone(std::vector<value_type>& values);
   static void test_insert_erase_burst();
   static void test_container_from_end(std::vector<value_type>& values);
   static void test_splay_up(std::vector<value_type>& values);
   static void test_splay_up(std::vector<value_type>& values, boost::intrusive::detail::true_type);
   static void test_splay_up(std::vector<value_type>& values, boost::intrusive::detail::false_type);
   static void test_splay_down(std::vector<value_type>& values);
   static void test_splay_down(std::vector<value_type>& values, boost::intrusive::detail::true_type);
   static void test_splay_down(std::vector<value_type>& values, boost::intrusive::detail::false_type);
   static void test_rebalance(std::vector<value_type>& values);
   static void test_rebalance(std::vector<value_type>& values, boost::intrusive::detail::true_type);
   static void test_rebalance(std::vector<value_type>& values, boost::intrusive::detail::false_type);
};

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_insert_erase_burst()
{
   typedef typename ValueTraits::value_type value_type;

   std::vector<value_type> values;
   const int MaxValues = 100;
   for(int i = 0; i != MaxValues; ++i){
      values.push_back(value_type(i));
   }

   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef typename assoc_type::iterator iterator;

   //First ordered insertions
   assoc_type testset (&values[0], &values[0] + values.size());
   TEST_INTRUSIVE_SEQUENCE_EXPECTED(testset, testset.begin());

   //Ordered erasure
   iterator it(testset.begin()), itend(testset.end());
   for(int i = 0; it != itend; ++i){
      BOOST_TEST(&*it == &values[i]);
      it = testset.erase(it);
   }

   BOOST_TEST(testset.empty());

   //Now random insertions
   std::random_shuffle(values.begin(), values.end());
   testset.insert(&values[0], &values[0] + values.size());
   std::sort(values.begin(), values.end());
   TEST_INTRUSIVE_SEQUENCE_EXPECTED(testset, testset.begin());

   //Random erasure
   std::random_shuffle(values.begin(), values.end());
   for(int i = 0; i != MaxValues; ++i){
      it = testset.erase(testset.iterator_to(values[i]));
   }

   BOOST_TEST(testset.empty());
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_all(std::vector<typename ValueTraits::value_type>& values)
{
   test_clone(values);
   test_container_from_end(values);
   test_splay_up(values);
   test_splay_down(values);
   test_rebalance(values);
   test_insert_erase_burst();
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>
   ::test_clone(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type value_type;
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;

   assoc_type testset1 (&values[0], &values[0] + values.size());
   assoc_type testset2;

   testset2.clone_from(testset1, test::new_cloner<value_type>(), test::delete_disposer<value_type>());
   BOOST_TEST (testset2 == testset1);
   testset2.clear_and_dispose(test::delete_disposer<value_type>());
   BOOST_TEST (testset2.empty());
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>
   ::test_container_from_end(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ValueTraits::value_type value_type;
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   assoc_type testset (&values[0], &values[0] + values.size());
   BOOST_TEST (testset == assoc_type::container_from_end_iterator(testset.end()));
   BOOST_TEST (testset == assoc_type::container_from_end_iterator(testset.cend()));
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_splay_up
(std::vector<typename ValueTraits::value_type>& values, boost::intrusive::detail::true_type)
{
   typedef typename ValueTraits::value_type value_type;
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef typename assoc_type::iterator iterator;
   typedef std::vector<value_type> orig_set_t;
   std::size_t num_values;
   orig_set_t original_testset;
   {
      assoc_type testset (values.begin(), values.end());
      num_values = testset.size();
      original_testset.insert(original_testset.end(), testset.begin(), testset.end());
   }

   for(std::size_t i = 0; i != num_values; ++i){
      assoc_type testset (values.begin(), values.end());
      {
         iterator it = testset.begin();
         for(std::size_t j = 0; j != i; ++j, ++it){}
         testset.splay_up(it);
      }
      BOOST_TEST (testset.size() == num_values);
      iterator it = testset.begin();
      for( typename orig_set_t::const_iterator origit    = original_testset.begin()
         , origitend = original_testset.end()
         ; origit != origitend
         ; ++origit, ++it){
         BOOST_TEST(*origit == *it);
      }
   }
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_splay_up
(std::vector<typename ValueTraits::value_type>&, boost::intrusive::detail::false_type)
{}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_splay_up
(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef typename detail::remove_const<assoc_type>::type Type;
   typedef detail::bool_<has_splay<Type>::value> enabler;
   test_splay_up(values, enabler());
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_splay_down
(std::vector<typename ValueTraits::value_type>& values, boost::intrusive::detail::true_type)
{
   typedef typename ValueTraits::value_type value_type;
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef typename assoc_type::iterator iterator;
   typedef std::vector<value_type> orig_set_t;
   std::size_t num_values;
   orig_set_t original_testset;
   {
      assoc_type testset (values.begin(), values.end());
      num_values = testset.size();
      original_testset.insert(original_testset.end(), testset.begin(), testset.end());
   }

   for(std::size_t i = 0; i != num_values; ++i){
      assoc_type testset (values.begin(), values.end());
      BOOST_TEST(testset.size() == num_values);
      {
         iterator it = testset.begin();
         for(std::size_t j = 0; j != i; ++j, ++it){}
         BOOST_TEST(*it == *testset.splay_down(*it));
      }
      BOOST_TEST (testset.size() == num_values);
      iterator it = testset.begin();
      for( typename orig_set_t::const_iterator origit    = original_testset.begin()
         , origitend = original_testset.end()
         ; origit != origitend
         ; ++origit, ++it){
         BOOST_TEST(*origit == *it);
      }
   }
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_splay_down
(std::vector<typename ValueTraits::value_type>&, boost::intrusive::detail::false_type)
{}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_splay_down
(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef typename detail::remove_const<assoc_type>::type Type;
   typedef detail::bool_<has_splay<Type>::value> enabler;
   test_splay_down(values, enabler());
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_rebalance
(std::vector<typename ValueTraits::value_type>& values, boost::intrusive::detail::true_type)
{
   typedef typename ValueTraits::value_type value_type;
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef std::vector<value_type> orig_set_t;
   typedef typename orig_set_t::iterator iterator_t;
   std::size_t num_values;
   orig_set_t original_testset;
   {
      assoc_type testset (values.begin(), values.end());
      num_values = testset.size();
      original_testset.insert(original_testset.end(), testset.begin(), testset.end());
   }
   {
      assoc_type testset(values.begin(), values.end());
      testset.rebalance();
      iterator_t it = original_testset.begin();
      TEST_INTRUSIVE_SEQUENCE_EXPECTED(original_testset, testset.begin());
   }

   {
      std::size_t numdata;
      {
         assoc_type testset(values.begin(), values.end());
         numdata = testset.size();
      }

      for(int i = 0; i != (int)numdata; ++i){
         assoc_type testset(values.begin(), values.end());
         typename assoc_type::iterator it = testset.begin();
         for(int j = 0; j  != i; ++j)  ++it;
         testset.rebalance_subtree(it);
         TEST_INTRUSIVE_SEQUENCE_EXPECTED(original_testset, testset.begin());
      }
   }
}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_rebalance
(std::vector<typename ValueTraits::value_type>&, boost::intrusive::detail::false_type)
{}

template<class ValueTraits, template <class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none, class = ::boost::intrusive::none> class ContainerDefiner>
void test_generic_assoc<ValueTraits, ContainerDefiner>::test_rebalance
(std::vector<typename ValueTraits::value_type>& values)
{
   typedef typename ContainerDefiner
      < value_type
      , value_traits<ValueTraits>
      , constant_time_size<value_type::constant_time_size>
      >::type assoc_type;
   typedef typename detail::remove_const<assoc_type>::type Type;
   typedef detail::bool_<has_rebalance<Type>::value> enabler;
   test_rebalance(values, enabler());
}

}}}   //namespace boost::intrusive::test

#include <boost/intrusive/detail/config_end.hpp>
