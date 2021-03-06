//  (C) Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_UBLAS_TYPE_CHECK_EPSILON (type_traits<real_type>::type_sqrt (boost::math::tools::epsilon <real_type>()))
#define BOOST_UBLAS_TYPE_CHECK_MIN (type_traits<real_type>::type_sqrt ( boost::math::tools::min_value<real_type>()))

#include <boost/math/bindings/rr.hpp>
#include <boost/math/tools/remez.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/actor.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/test/included/test_exec_monitor.hpp> // for test_main

extern boost::math::ntl::RR f(const boost::math::ntl::RR& x, int variant);
extern void show_extra(
   const boost::math::tools::polynomial<boost::math::ntl::RR>& n, 
   const boost::math::tools::polynomial<boost::math::ntl::RR>& d, 
   const boost::math::ntl::RR& x_offset, 
   const boost::math::ntl::RR& y_offset, 
   int variant);

using namespace boost::spirit;

boost::math::ntl::RR a(0), b(1);   // range to optimise over
bool rel_error(true);
bool pin(false);
int orderN(3);
int orderD(1);
int target_precision(53);
int working_precision(250);
bool started(false);
int variant(0);
int skew(0);
int brake(50);
boost::math::ntl::RR x_offset(0), y_offset(0);
bool auto_offset_y;

boost::shared_ptr<boost::math::tools::remez_minimax<boost::math::ntl::RR> > p_remez;

boost::math::ntl::RR the_function(const boost::math::ntl::RR& val)
{
   return f(val + x_offset, variant) + y_offset;
}

void step_some(unsigned count)
{
   try{
      NTL::RR::SetPrecision(working_precision);
      if(!started)
      {
         //
         // If we have an automatic y-offset calculate it now:
         //
         if(auto_offset_y)
         {
            boost::math::ntl::RR fa, fb, fm;
            fa = f(a + x_offset, variant);
            fb = f(b + x_offset, variant);
            fm = f((a+b)/2 + x_offset, variant);
            y_offset = -(fa + fb + fm) / 3;
            NTL::RR::SetOutputPrecision(5);
            std::cout << "Setting auto-y-offset to " << y_offset << std::endl;
         }
         //
         // Truncate offsets to float precision:
         //
         x_offset = NTL::RoundToPrecision(x_offset.value(), 20);
         y_offset = NTL::RoundToPrecision(y_offset.value(), 20);
         //
         // Construct new Remez state machine:
         //
         p_remez.reset(new boost::math::tools::remez_minimax<boost::math::ntl::RR>(
            &the_function, 
            orderN, orderD, 
            a, b, 
            pin, 
            rel_error, 
            skew, 
            working_precision));
         std::cout << "Max error in interpolated form: " << std::setprecision(3) << std::scientific << boost::math::tools::real_cast<double>(p_remez->max_error()) << std::endl;
         //
         // Signal that we've started:
         //
         started = true;
      }
      unsigned i;
      for(i = 0; i < count; ++i)
      {
         std::cout << "Stepping..." << std::endl;
         p_remez->set_brake(brake);
         boost::math::ntl::RR r = p_remez->iterate();
         NTL::RR::SetOutputPrecision(3);
         std::cout 
            << "Maximum Deviation Found:                     " << std::setprecision(3) << std::scientific << boost::math::tools::real_cast<double>(p_remez->max_error()) << std::endl
            << "Expected Error Term:                         " << std::setprecision(3) << std::scientific << boost::math::tools::real_cast<double>(p_remez->error_term()) << std::endl
            << "Maximum Relative Change in Control Points:   " << std::setprecision(3) << std::scientific << boost::math::tools::real_cast<double>(r) << std::endl;
      }
   }
   catch(const std::exception& e)
   {
      std::cout << "Step failed with exception: " << e.what() << std::endl;
   }
}

void step(const char*, const char*)
{
   step_some(1);
}

void show(const char*, const char*)
{
   NTL::RR::SetPrecision(working_precision);
   if(started)
   {
      boost::math::tools::polynomial<boost::math::ntl::RR> n = p_remez->numerator();
      boost::math::tools::polynomial<boost::math::ntl::RR> d = p_remez->denominator();
      int prec = 2 + (target_precision * 3010LL)/10000;
      std::cout << std::scientific << std::setprecision(prec);
      NTL::RR::SetOutputPrecision(prec);
      boost::numeric::ublas::vector<boost::math::ntl::RR> v = p_remez->zero_points();
      
      std::cout << "  Zeros = {\n";
      unsigned i;
      for(i = 0; i < v.size(); ++i)
      {
         std::cout << "    " << v[i] << std::endl;
      }
      std::cout << "  }\n";

      v = p_remez->chebyshev_points();
      std::cout << "  Chebeshev Control Points = {\n";
      for(i = 0; i < v.size(); ++i)
      {
         std::cout << "    " << v[i] << std::endl;
      }
      std::cout << "  }\n";

      std::cout << "X offset: " << x_offset << std::endl;
      std::cout << "Y offset: " << y_offset << std::endl;

      std::cout << "P = {";
      for(i = 0; i < n.size(); ++i)
      {
         std::cout << "    " << n[i] << std::endl;
      }
      std::cout << "  }\n";

      std::cout << "Q = {";
      for(i = 0; i < d.size(); ++i)
      {
         std::cout << "    " << d[i] << std::endl;
      }
      std::cout << "  }\n";

      show_extra(n, d, x_offset, y_offset, variant);
   }
   else
   {
      std::cerr << "Nothing to display" << std::endl;
   }
}

void do_graph(unsigned points)
{
   NTL::RR::SetPrecision(working_precision);
   boost::math::ntl::RR step = (b - a) / (points - 1);
   boost::math::ntl::RR x = a;
   while(points > 1)
   {
      NTL::RR::SetOutputPrecision(10);
      std::cout << std::setprecision(10) << std::setw(30) << std::left 
         << boost::lexical_cast<std::string>(x) << the_function(x) << std::endl;
      --points;
      x += step;
   }
   std::cout << std::setprecision(10) << std::setw(30) << std::left 
      << boost::lexical_cast<std::string>(b) << the_function(b) << std::endl;
}

void graph(const char*, const char*)
{
   do_graph(3);
}

template <class T>
void do_test(T, const char* name)
{
   boost::math::ntl::RR::SetPrecision(working_precision);
   if(started)
   {
      //
      // We want to test the approximation at fixed precision:
      // either float, double or long double.  Begin by getting the
      // polynomials:
      //
      boost::math::tools::polynomial<T> n, d;
      n = p_remez->numerator();
      d = p_remez->denominator();
      //
      // We'll test at the Chebeshev control points which is where
      // (in theory) the largest deviation should occur.  For good
      // measure we'll test at the zeros as well:
      //
      boost::numeric::ublas::vector<boost::math::ntl::RR> 
         zeros(p_remez->zero_points()),
         cheb(p_remez->chebyshev_points());

      boost::math::ntl::RR max_error(0);

      //
      // Do the tests at the zeros:
      //
      std::cout << "Starting tests at " << name << " precision...\n";
      std::cout << "Absissa        Error\n";
      unsigned i;
      for(i = 0; i < zeros.size(); ++i)
      {
         boost::math::ntl::RR true_result = the_function(zeros[i]);
         T absissa = boost::math::tools::real_cast<T>(zeros[i]);
         boost::math::ntl::RR test_result = n.evaluate(absissa) / d.evaluate(absissa);
         boost::math::ntl::RR err;
         if(rel_error)
         {
            err = boost::math::tools::relative_error(test_result, true_result);
         }
         else
         {
            err = fabs(test_result - true_result);
         }
         if(err > max_error)
            max_error = err;
         std::cout << std::setprecision(6) << std::setw(15) << std::left << absissa
            << boost::math::tools::real_cast<T>(err) << std::endl;
      }
      //
      // Do the tests at the Chebeshev control points:
      //
      for(i = 0; i < cheb.size(); ++i)
      {
         boost::math::ntl::RR true_result = the_function(cheb[i]);
         T absissa = boost::math::tools::real_cast<T>(cheb[i]);
         boost::math::ntl::RR test_result = n.evaluate(absissa) / d.evaluate(absissa);
         boost::math::ntl::RR err;
         if(rel_error)
         {
            err = boost::math::tools::relative_error(test_result, true_result);
         }
         else
         {
            err = fabs(test_result - true_result);
         }
         if(err > max_error)
            max_error = err;
         std::cout << std::setprecision(6) << std::setw(15) << std::left << absissa
            << boost::math::tools::real_cast<T>(err) << std::endl;
      }
      std::cout << "Max error found: " << std::setprecision(6) << boost::math::tools::real_cast<T>(max_error) << std::endl;
   }
   else
   {
      std::cout << "Nothing to test: try converging an approximation first!!!" << std::endl;
   }
}

void test_float(const char*, const char*)
{
   do_test(float(0), "float");
}

void test_double(const char*, const char*)
{
   do_test(double(0), "double");
}

void test_long(const char*, const char*)
{
   do_test((long double)(0), "long double");
}

void test_all(const char*, const char*)
{
   do_test(float(0), "float");
   do_test(double(0), "double");
   do_test((long double)(0), "long double");
}

template <class T>
void do_test_n(T, const char* name, unsigned count)
{
   boost::math::ntl::RR::SetPrecision(working_precision);
   if(started)
   {
      //
      // We want to test the approximation at fixed precision:
      // either float, double or long double.  Begin by getting the
      // polynomials:
      //
      boost::math::tools::polynomial<T> n, d;
      n = p_remez->numerator();
      d = p_remez->denominator();

      boost::math::ntl::RR max_error(0);
      boost::math::ntl::RR step = (b - a) / count;

      //
      // Do the tests at the zeros:
      //
      std::cout << "Starting tests at " << name << " precision...\n";
      std::cout << "Absissa        Error\n";
      for(boost::math::ntl::RR x = a; x <= b; x += step)
      {
         boost::math::ntl::RR true_result = the_function(x);
         T absissa = boost::math::tools::real_cast<T>(x);
         boost::math::ntl::RR test_result = n.evaluate(absissa) / d.evaluate(absissa);
         boost::math::ntl::RR err;
         if(rel_error)
         {
            err = boost::math::tools::relative_error(test_result, true_result);
         }
         else
         {
            err = fabs(test_result - true_result);
         }
         if(err > max_error)
            max_error = err;
         std::cout << std::setprecision(6) << std::setw(15) << std::left << boost::math::tools::real_cast<double>(absissa)
            << (test_result < true_result ? "-" : "") << boost::math::tools::real_cast<double>(err) << std::endl;
      }
      std::cout << "Max error found: " << std::setprecision(6) << boost::math::tools::real_cast<T>(max_error) << std::endl;
   }
   else
   {
      std::cout << "Nothing to test: try converging an approximation first!!!" << std::endl;
   }
}

void test_n(unsigned n)
{
   do_test_n(boost::math::ntl::RR(), "boost::math::ntl::RR", n);
}

void test_float_n(unsigned n)
{
   do_test_n(float(0), "float", n);
}

void test_double_n(unsigned n)
{
   do_test_n(double(0), "double", n);
}

void test_long_n(unsigned n)
{
   do_test_n((long double)(0), "long double", n);
}

void rotate(const char*, const char*)
{
   if(p_remez)
   {
      p_remez->rotate();
   }
   else
   {
      std::cerr << "Nothing to rotate" << std::endl;
   }
}

void rescale(const char*, const char*)
{
   if(p_remez)
   {
      p_remez->rescale(a, b);
   }
   else
   {
      std::cerr << "Nothing to rescale" << std::endl;
   }
}

void graph_poly(const char*, const char*)
{
   int i = 50;
   boost::math::ntl::RR::SetPrecision(working_precision);
   if(started)
   {
      //
      // We want to test the approximation at fixed precision:
      // either float, double or long double.  Begin by getting the
      // polynomials:
      //
      boost::math::tools::polynomial<boost::math::ntl::RR> n, d;
      n = p_remez->numerator();
      d = p_remez->denominator();

      boost::math::ntl::RR max_error(0);
      boost::math::ntl::RR step = (b - a) / i;

      std::cout << "Evaluating Numerator...\n";
      boost::math::ntl::RR val;
      for(val = a; val <= b; val += step)
         std::cout << n.evaluate(val) << std::endl;
      std::cout << "Evaluating Denominator...\n";
      for(val = a; val <= b; val += step)
         std::cout << d.evaluate(val) << std::endl;
   }
   else
   {
      std::cout << "Nothing to test: try converging an approximation first!!!" << std::endl;
   }
}

int test_main(int, char* [])
{
   std::string line;
   while(std::getline(std::cin, line))
   {
      if(parse(line.c_str(), str_p("quit"), space_p).full)
         return 0;
      if(false == parse(line.c_str(), 
         (

            str_p("range")[assign_a(started, false)] && real_p[assign_a(a)] && real_p[assign_a(b)]
      ||
            str_p("relative")[assign_a(started, false)][assign_a(rel_error, true)]
      ||
            str_p("absolute")[assign_a(started, false)][assign_a(rel_error, false)]
      ||
            str_p("pin")[assign_a(started, false)] && str_p("true")[assign_a(pin, true)]
      ||
            str_p("pin")[assign_a(started, false)] && str_p("false")[assign_a(pin, false)]
      ||
            str_p("pin")[assign_a(started, false)] && str_p("1")[assign_a(pin, true)]
      ||
            str_p("pin")[assign_a(started, false)] && str_p("0")[assign_a(pin, false)]
      ||
            str_p("pin")[assign_a(started, false)][assign_a(pin, true)]
      ||
            str_p("order")[assign_a(started, false)] && uint_p[assign_a(orderN)] && uint_p[assign_a(orderD)]
      ||
            str_p("order")[assign_a(started, false)] && uint_p[assign_a(orderN)]
      ||
            str_p("target-precision") && uint_p[assign_a(target_precision)]
      ||
            str_p("working-precision")[assign_a(started, false)] && uint_p[assign_a(working_precision)]
      ||
            str_p("variant")[assign_a(started, false)] && int_p[assign_a(variant)]
      ||
            str_p("skew")[assign_a(started, false)] && int_p[assign_a(skew)]
      ||
            str_p("brake") && int_p[assign_a(brake)]
      ||
            str_p("step") && int_p[&step_some]
      ||
            str_p("step")[&step]
      ||
            str_p("poly")[&graph_poly]
      ||
            str_p("info")[&show]
      ||
            str_p("graph") && uint_p[&do_graph]
      ||
            str_p("graph")[&graph]
      ||
            str_p("x-offset") && real_p[assign_a(x_offset)]
      ||
            str_p("y-offset") && str_p("auto")[assign_a(auto_offset_y, true)]
      ||
            str_p("y-offset") && real_p[assign_a(y_offset)][assign_a(auto_offset_y, false)]
      ||
            str_p("test") && str_p("float") && uint_p[&test_float_n]
      ||
            str_p("test") && str_p("float")[&test_float]
      ||
            str_p("test") && str_p("double") && uint_p[&test_double_n]
      ||
            str_p("test") && str_p("double")[&test_double]
      ||
            str_p("test") && str_p("long") && uint_p[&test_long_n]
      ||
            str_p("test") && str_p("long")[&test_long]
      ||
            str_p("test") && str_p("all")[&test_all]
      ||
            str_p("test") && uint_p[&test_n]
      ||
            str_p("rotate")[&rotate]
      ||
            str_p("rescale") && real_p[assign_a(a)] && real_p[assign_a(b)] && epsilon_p[&rescale]

         ), space_p).full)
      {
         std::cout << "Unable to parse directive: \"" << line << "\"" << std::endl;
      }
      else
      {
         std::cout << "a                    = " << a << std::endl;
         std::cout << "b                    = " << b << std::endl;
         std::cout << "Relative Error       = " << rel_error << std::endl;
         std::cout << "Pin to Origin        = " << pin << std::endl;
         std::cout << "Order (Numerator)    = " << orderN << std::endl;
         std::cout << "Order (Denominator)  = " << orderD << std::endl;
         std::cout << "Target Precision     = " << target_precision << std::endl;
         std::cout << "Working Precision    = " << working_precision << std::endl;
         std::cout << "Variant              = " << variant << std::endl;
         std::cout << "Skew                 = " << skew << std::endl;
         std::cout << "Brake                = " << brake << std::endl;
         std::cout << "X Offset             = " << x_offset << std::endl;
         std::cout << "Y Offset             = " << y_offset << std::endl;
         std::cout << "Automatic Y Offset   = " << auto_offset_y << std::endl;
     }
   }
   return 0;
}
