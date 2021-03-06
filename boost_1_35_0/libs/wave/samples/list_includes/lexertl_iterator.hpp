/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    Definition of the lexertl based lexer iterator 
    
    http://www.boost.org/

    Copyright (c) 2001-2008 Hartmut Kaiser. Distributed under the Boost 
    Software License, Version 1.0. (See accompanying file 
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_WAVE_LEXERTL_ITERATOR_HPP_INCLUDED)
#define BOOST_WAVE_LEXERTL_ITERATOR_HPP_INCLUDED

#include <string>

#include <boost/assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/spirit/iterator/multi_pass.hpp>

#include <boost/wave/language_support.hpp>
#include <boost/wave/util/file_position.hpp>
#include <boost/wave/util/functor_input.hpp>

#include "lexertl_interface.hpp"

#if 0 != __COMO_VERSION__
#define BOOST_WAVE_EOF_PREFIX static
#else
#define BOOST_WAVE_EOF_PREFIX 
#endif

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace wave { namespace cpplexer { namespace lexertl 
{

///////////////////////////////////////////////////////////////////////////////
namespace impl {

///////////////////////////////////////////////////////////////////////////////
//  
//  lex_iterator_functor_shim
//
///////////////////////////////////////////////////////////////////////////////

template <typename TokenT> 
class iterator_functor_shim 
{
public:
    template <typename IteratorT>
    iterator_functor_shim(IteratorT const &first, IteratorT const &last, 
            typename TokenT::position_type const &pos, 
            wave::language_support language)
    :   functor_ptr(lexertl_input_interface<TokenT>
            ::new_lexer(first, last, pos, language)) 
#if 0 != __DECCXX_VER
      , eof()
#endif // 0 != __DECCXX_VER
    {}

// interface to the boost::spirit::multi_pass_policies::functor_input policy
    typedef TokenT result_type;

    BOOST_WAVE_EOF_PREFIX result_type const eof;
    
    result_type operator()() 
    { 
        BOOST_ASSERT(0 != functor_ptr.get());
        return functor_ptr->get(); 
    }
    void set_position(typename TokenT::position_type const &pos)
    {
        BOOST_ASSERT(0 != functor_ptr.get());
        functor_ptr->set_position(pos);
    }
    
#if BOOST_WAVE_SUPPORT_PRAGMA_ONCE != 0
    bool has_include_guards(std::string& guard_name) const
    {
        BOOST_ASSERT(0 != functor_ptr.get());
        return functor_ptr->has_include_guards(guard_name);
    }
#endif    

private:
    boost::shared_ptr<lex_input_interface<TokenT> > functor_ptr;
};

#if 0 != __COMO_VERSION__
///////////////////////////////////////////////////////////////////////////////
//  eof token
template <typename TokenT>
typename iterator_functor_shim<TokenT>::result_type const
    iterator_functor_shim<TokenT>::eof = 
        typename iterator_functor_shim<TokenT>::result_type();
#endif // 0 != __COMO_VERSION__

///////////////////////////////////////////////////////////////////////////////
}   // namespace impl

///////////////////////////////////////////////////////////////////////////////
//  
//  lexertl_iterator
//
//      A generic C++ lexer interface class, which allows to plug in different
//      lexer implementations (template parameter LexT). The following 
//      requirement apply:
//
//          - the lexer type should have a function implemented, which returnes
//            the next lexed token from the input stream:
//                typename LexT::token_type get();
//          - at the end of the input stream this function should return the
//            eof token equivalent
//          - the lexer should implement a constructor taking two iterators
//            pointing to the beginning and the end of the input stream and
//            a third parameter containing the name of the parsed input file,
//            the 4th parameter contains the information about the mode the 
//            preprocessor is used in (C99/C++ mode etc.)
//
///////////////////////////////////////////////////////////////////////////////

template <typename TokenT>
class lex_iterator 
:   public boost::spirit::multi_pass<
        impl::iterator_functor_shim<TokenT>,
        boost::wave::util::functor_input
    >
{
    typedef impl::iterator_functor_shim<TokenT> input_policy_type;
    typedef 
        boost::spirit::multi_pass<input_policy_type, 
                boost::wave::util::functor_input>
        base_type;
    typedef lex_iterator<TokenT> self_type;
    
public:
    typedef TokenT token_type;
    
    lex_iterator()
    {}
    
    template <typename IteratorT>
    lex_iterator(IteratorT const &first, IteratorT const &last, 
            typename TokenT::position_type const &pos, 
            boost::wave::language_support language)
    :   base_type(input_policy_type(first, last, pos, language))
    {}

    void set_position(typename TokenT::position_type const &pos)
    {
        typedef typename TokenT::position_type position_type;
        
    // set the new position in the current token
    token_type& currtoken = base_type::get_input();
    position_type currpos = currtoken.get_position();

        currpos.set_file(pos.get_file());
        currpos.set_line(pos.get_line());
        currtoken.set_position(currpos);
        
    // set the new position for future tokens as well
        if (token_type::string_type::npos != 
            currtoken.get_value().find_first_of('\n'))
        {
            currpos.set_line(pos.get_line() + 1);
        }
        base_type::get_functor().set_position(currpos);
    }
    
#if BOOST_WAVE_SUPPORT_PRAGMA_ONCE != 0
    // Return, whether the current file has include guards. This function 
    // returns meaningful results only if the file was scanned completely.
    // For now we always return false, but this can be fixed easily later on.
    bool has_include_guards(std::string& guard_name) const
    {
        return base_type::get_functor().has_include_guards(guard_name);
    }
#endif    
};

///////////////////////////////////////////////////////////////////////////////
}}}}   // namespace boost::wave::cpplexer::lexertl

#undef BOOST_WAVE_EOF_PREFIX

#endif // !defined(BOOST_WAVE_LEXERTL_ITERATOR_HPP_INCLUDED)
