///////////////////////////////////////////////////////////////////////////////
// main.hpp
//
//  Copyright 2007 David Jenkins. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if defined(_MSC_VER)
//disbale warning C4996: 'std::xxx' was declared deprecated
# pragma warning(disable:4996)
#endif

#include <iostream>
#include <string> 
#include <map>
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>

///////////////////////////////////////////////////////////////////////////////
// Match all named numbers in a string and return their integer values
//
// For example, given the input string:
//      "one two sixty three thousand ninety five eleven"
// the program will output:
//      "one = 1"
//      "two = 2"
//      "sixty three thousand ninety five = 63095"
//      "eleven = 11"

void example1()
{
    using namespace boost::xpressive;
    using namespace boost::assign;

    // initialize the maps for named numbers
    std::map< std::string, int > ones_map = 
        map_list_of("one",1)("two",2)("three",3)("four",4)("five",5)
        ("six",6)("seven",7)("eight",8)("nine",9);

    std::map< std::string, int > teens_map = 
        map_list_of("ten",10)("eleven",11)("twelve",12)("thirteen",13)
        ("fourteen",14)("fifteen",15)("sixteen",16)("seventeen",17)
        ("eighteen",18)("nineteen",19);

    std::map< std::string, int > tens_map = 
        map_list_of("twenty",20)("thirty",30)("fourty",40)
        ("fifty",50)("sixty",60)("seventy",70)("eighty",80)("ninety",90);

    std::map< std::string, int > specials_map = 
        map_list_of("zero",0)("dozen",12)("score",20);

    // n is the integer result
    local<long> n(0);
    // temp stores intermediate values
    local<long> temp(0);

    // delimiters between words must be spaces, end-of-sequence, or punctuation
    sregex delim =
        +_s | eos | +punct;

    // initialize the regular expressions for named numbers
    sregex ones_rx =
        ( a1 = ones_map ) [ n += a1 ] >> delim;

    sregex tens_rx =
            ones_rx
        |
        (
            ( a1 = tens_map ) [ n += a1 ] >> delim
            >> !ones_rx
        )
        | 
        (   ( a1 = teens_map ) [ n += a1 ] >> delim
        );

    sregex hundreds_rx =
        (   ( tens_rx >> "hundred"  >> delim )
            [ n *= 100 ]
            >> !tens_rx
        ) 
        | tens_rx;

    sregex thousands_rx = 
        (   ( hundreds_rx >> "thousand" >> delim )
            [ temp += n * 1000, n = 0 ]
            >> !hundreds_rx
        )
        | hundreds_rx
        ;

    sregex millions_rx = 
        (   ( hundreds_rx >> "million" >> delim )
            [ temp += n * 1000000, n = 0 ]
            >> !thousands_rx
        )
        | thousands_rx;

    // Note: this uses two attribues, a1 and a2, and it uses
    // a default attribute value of 1 for a1. 
    sregex specials_rx =
        ( !((a1 = ones_map) >> delim) >> (a2 = specials_map) ) 
            [ n = (a1 | 1) * a2 ] 
        >> delim
        >> !("and" >> +_s >> ones_rx);

    sregex number_rx = 
        bow 
        >> 
        (   specials_rx
        | 
            millions_rx
            [n += temp, temp = 0 ] 
        );

    // this is the input string
    std::string str( "one two three eighteen twenty two "
        "nine hundred ninety nine twelve " 
        "eight hundred sixty three thousand ninety five "
        "sixty five hundred ten "
        "two million eight hundred sixty three thousand ninety five "
        "zero sixty five hundred thousand "
        "extra stuff -- skip me "
        "two dozen "
        "four score and seven ");

    // the results of iterating through the string are:
    //      one  = 1
    //      two  = 2
    //      three  = 3
    //      eighteen  = 18
    //      twenty two  = 22
    //      nine hundred ninety nine  = 999
    //      twelve  = 12
    //      eight hundred sixty three thousand ninety five  = 863095
    //      sixty five hundred ten  = 6510
    //      two million eight hundred sixty three thousand ninety five  = 2863095
    //      zero = 0
    //      sixty five hundred thousand = 6500000
    //      two dozen = 24
    //      four score and seven = 87
    sregex_token_iterator cur( str.begin(), str.end(), number_rx );
    sregex_token_iterator end;

    for( ; cur != end; ++cur )
    {
        std::cout << *cur << " = " << n.get() << '\n';
        n.get() = 0;
    }
    std::cout << '\n';
}

///////////////////////////////////////////////////////////////////////////////
// main
int main()
{
    std::cout << "\n\nExample 1:\n\n";
    example1();

    std::cout << "\n\n" << std::flush;

    return 0;
}
