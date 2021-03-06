// Copyright 2002-2008, Fernando Luis Cacciola Carballal.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Test program for "boost/utility/value_init.hpp"
//
// 21 Agu 2002 (Created) Fernando Cacciola
// 15 Jan 2008 (Added tests regarding compiler issues) Fernando Cacciola, Niels Dekker

#include <cstring>  // For memcmp.
#include <iostream>
#include <string>

#include "boost/utility/value_init.hpp"
#include <boost/shared_ptr.hpp>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "boost/test/minimal.hpp"

//
// Sample POD type
//
struct POD
{
  POD () : c(0), i(0), f(0) {}

  POD ( char c_, int i_, float f_ ) : c(c_), i(i_), f(f_) {}

  friend std::ostream& operator << ( std::ostream& os, POD const& pod )
    { return os << '(' << pod.c << ',' << pod.i << ',' << pod.f << ')' ; }

  friend bool operator == ( POD const& lhs, POD const& rhs )
    { return lhs.f == rhs.f && lhs.c == rhs.c && lhs.i == rhs.i ; }

  float f;
  char  c;
  int   i;
} ;

//
// Sample non POD type
//
struct NonPODBase
{
  virtual ~NonPODBase() {}
} ;
struct NonPOD : NonPODBase
{
  NonPOD () : id() {}
  NonPOD ( std::string const& id_) : id(id_) {}

  friend std::ostream& operator << ( std::ostream& os, NonPOD const& npod )
    { return os << '(' << npod.id << ')' ; }

  friend bool operator == ( NonPOD const& lhs, NonPOD const& rhs )
    { return lhs.id == rhs.id ; }

  std::string id ;
} ;

//
// Sample aggregate POD struct type
// Some compilers do not correctly value-initialize such a struct, for example:
// Borland C++ Report #51854, "Value-initialization: POD struct should be zero-initialized "
// http://qc.codegear.com/wc/qcmain.aspx?d=51854
//
struct AggregatePODStruct
{
  float f;
  char  c;
  int   i;
};

bool operator == ( AggregatePODStruct const& lhs, AggregatePODStruct const& rhs )
{ return lhs.f == rhs.f && lhs.c == rhs.c && lhs.i == rhs.i ; }

//
// An aggregate struct that contains an std::string and an int.
// Pavel Kuznetsov (MetaCommunications Engineering) used a struct like
// this to reproduce the Microsoft Visual C++ compiler bug, reported as
// Feedback ID 100744, "Value-initialization in new-expression"
// https://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=100744
//
struct StringAndInt
{
  std::string s;
  int i;
};

bool operator == ( StringAndInt const& lhs, StringAndInt const& rhs )
{ return lhs.s == rhs.s && lhs.i == rhs.i ; }


//
// A struct that has an explicit (user defined) destructor.
// Some compilers do not correctly value-initialize such a struct, for example:
// Microsoft Visual C++, Feedback ID 100744, "Value-initialization in new-expression"
// https://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=100744
//
struct StructWithDestructor
{
  int i;
  ~StructWithDestructor() {}
};

bool operator == ( StructWithDestructor const& lhs, StructWithDestructor const& rhs )
{ return lhs.i == rhs.i ; }


//
// A struct that has a virtual function.
// Some compilers do not correctly value-initialize such a struct either, for example:
// Microsoft Visual C++, Feedback ID 100744, "Value-initialization in new-expression"
// https://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=100744
//
struct StructWithVirtualFunction
{
  int i;
  virtual void VirtualFunction(); 
};

void StructWithVirtualFunction::VirtualFunction()
{
}

bool operator == ( StructWithVirtualFunction const& lhs, StructWithVirtualFunction const& rhs )
{ return lhs.i == rhs.i ; }


//
// A struct that is derived from an aggregate POD struct.
// Some compilers do not correctly value-initialize such a struct, for example:
// GCC Bugzilla Bug 30111,  "Value-initialization of POD base class doesn't initialize members",
// reported by Jonathan Wakely, http://gcc.gnu.org/bugzilla/show_bug.cgi?id=30111
//
struct DerivedFromAggregatePODStruct : AggregatePODStruct
{
  DerivedFromAggregatePODStruct() : AggregatePODStruct() {}
};

//
// A struct that wraps an aggregate POD struct as data member.
//
struct AggregatePODStructWrapper
{
  AggregatePODStructWrapper() : dataMember() {}
  AggregatePODStruct dataMember;
};

bool operator == ( AggregatePODStructWrapper const& lhs, AggregatePODStructWrapper const& rhs )
{ return lhs.dataMember == rhs.dataMember ; }

typedef unsigned char ArrayOfBytes[256];


//
// A struct that allows testing whether the appropriate copy functions are called.
//
struct CopyFunctionCallTester
{
  bool is_copy_constructed;
  bool is_assignment_called;

  CopyFunctionCallTester()
  : is_copy_constructed(false), is_assignment_called(false) {}

  CopyFunctionCallTester(const CopyFunctionCallTester & )
  : is_copy_constructed(true), is_assignment_called(false) {}

  CopyFunctionCallTester & operator=(const CopyFunctionCallTester & )
  {
    is_assignment_called = true ;
    return *this ;
  }
};


//
// This test function tests boost::value_initialized<T> for a specific type T.
// The first argument (y) is assumed have the value of a value-initialized object.
// Returns true on success.
//
template<class T>
bool test ( T const& y, T const& z )
{
  const boost::unit_test::counter_t counter_before_test = boost::minimal_test::errors_counter();
  boost::value_initialized<T> x ;
  BOOST_CHECK ( y == x ) ;
  BOOST_CHECK ( y == boost::get(x) ) ;
  static_cast<T&>(x) = z ;
  boost::get(x) = z ;
  BOOST_CHECK ( x == z ) ;

  boost::value_initialized<T> const x_c ;
  BOOST_CHECK ( y == x_c ) ;
  BOOST_CHECK ( y == boost::get(x_c) ) ;
  T& x_c_ref = x_c ;
  x_c_ref = z ;
  BOOST_CHECK ( x_c == z ) ;

  boost::value_initialized<T> const copy1 = x;
  BOOST_CHECK ( boost::get(copy1) == boost::get(x) ) ;

  boost::value_initialized<T> copy2;
  copy2 = x;
  BOOST_CHECK ( boost::get(copy2) == boost::get(x) ) ;
  
  boost::shared_ptr<boost::value_initialized<T> > ptr( new boost::value_initialized<T> );
  BOOST_CHECK ( y == *ptr ) ;

#if !BOOST_WORKAROUND(BOOST_MSVC, < 1300)
  boost::value_initialized<T const> cx ;
  BOOST_CHECK ( y == cx ) ;
  BOOST_CHECK ( y == boost::get(cx) ) ;

  boost::value_initialized<T const> const cx_c ;
  BOOST_CHECK ( y == cx_c ) ;
  BOOST_CHECK ( y == boost::get(cx_c) ) ;
#endif

  return boost::minimal_test::errors_counter() == counter_before_test ;
}

int test_main(int, char **)
{
  BOOST_CHECK ( test( 0,1234 ) ) ;
  BOOST_CHECK ( test( 0.0,12.34 ) ) ;
  BOOST_CHECK ( test( POD(0,0,0.0), POD('a',1234,56.78) ) ) ;
  BOOST_CHECK ( test( NonPOD( std::string() ), NonPOD( std::string("something") ) ) ) ;

  NonPOD NonPOD_object( std::string("NonPOD_object") );
  BOOST_CHECK ( test<NonPOD *>( 0, &NonPOD_object ) ) ;

  AggregatePODStruct zeroInitializedAggregatePODStruct = { 0.0f, '\0', 0 };
  AggregatePODStruct nonZeroInitializedAggregatePODStruct = { 1.25f, 'a', -1 };
  BOOST_CHECK ( test(zeroInitializedAggregatePODStruct, nonZeroInitializedAggregatePODStruct) );

  StringAndInt stringAndInt0;
  StringAndInt stringAndInt1;
  stringAndInt0.i = 0;
  stringAndInt1.i = 1;
  stringAndInt1.s = std::string("1");
  BOOST_CHECK ( test(stringAndInt0, stringAndInt1) );

  StructWithDestructor structWithDestructor0;
  StructWithDestructor structWithDestructor1;
  structWithDestructor0.i = 0;
  structWithDestructor1.i = 1;
  BOOST_CHECK ( test(structWithDestructor0, structWithDestructor1) );

  StructWithVirtualFunction structWithVirtualFunction0;
  StructWithVirtualFunction structWithVirtualFunction1;
  structWithVirtualFunction0.i = 0;
  structWithVirtualFunction1.i = 1;
  BOOST_CHECK ( test(structWithVirtualFunction0, structWithVirtualFunction1) );

  DerivedFromAggregatePODStruct derivedFromAggregatePODStruct0;
  DerivedFromAggregatePODStruct derivedFromAggregatePODStruct1;
  static_cast<AggregatePODStruct &>(derivedFromAggregatePODStruct0) = zeroInitializedAggregatePODStruct;
  static_cast<AggregatePODStruct &>(derivedFromAggregatePODStruct1) = nonZeroInitializedAggregatePODStruct;
  BOOST_CHECK ( test(derivedFromAggregatePODStruct0, derivedFromAggregatePODStruct1) );

  AggregatePODStructWrapper aggregatePODStructWrapper0;
  AggregatePODStructWrapper aggregatePODStructWrapper1;
  aggregatePODStructWrapper0.dataMember = zeroInitializedAggregatePODStruct;
  aggregatePODStructWrapper1.dataMember = nonZeroInitializedAggregatePODStruct;
  BOOST_CHECK ( test(aggregatePODStructWrapper0, aggregatePODStructWrapper1) );

  ArrayOfBytes zeroInitializedArrayOfBytes = { 0 };
  boost::value_initialized<ArrayOfBytes> valueInitializedArrayOfBytes;
  BOOST_CHECK (std::memcmp(get(valueInitializedArrayOfBytes), zeroInitializedArrayOfBytes, sizeof(ArrayOfBytes)) == 0);

  boost::value_initialized<CopyFunctionCallTester> copyFunctionCallTester1;
  BOOST_CHECK ( ! get(copyFunctionCallTester1).is_copy_constructed);
  BOOST_CHECK ( ! get(copyFunctionCallTester1).is_assignment_called);

  boost::value_initialized<CopyFunctionCallTester> copyFunctionCallTester2 = boost::value_initialized<CopyFunctionCallTester>(copyFunctionCallTester1);
  BOOST_CHECK ( get(copyFunctionCallTester2).is_copy_constructed);
  BOOST_CHECK ( ! get(copyFunctionCallTester2).is_assignment_called);

  boost::value_initialized<CopyFunctionCallTester> copyFunctionCallTester3;
  copyFunctionCallTester3 = boost::value_initialized<CopyFunctionCallTester>(copyFunctionCallTester1);
  BOOST_CHECK ( ! get(copyFunctionCallTester3).is_copy_constructed);
  BOOST_CHECK ( get(copyFunctionCallTester3).is_assignment_called);

  return 0;
}


unsigned int expected_failures = 0;

