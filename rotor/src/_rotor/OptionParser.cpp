#include "OptionParser.h"
#include <boost/spirit.hpp>

using namespace boost::spirit;
using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

struct OptionGrammar : public grammar<OptionGrammar>
{
  OptionGrammar( Options & options ) : grammar<OptionGrammar>(), _options( options ) {}

  struct captureOption
  {
    captureOption( const OptionGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      OptionGrammar & grammar = const_cast<OptionGrammar&>( _grammar );
      grammar._options.setString( grammar._section, grammar._option, grammar._value );
    }
    const OptionGrammar & _grammar;
  };

  struct captureSection
  {
    captureSection( const OptionGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      OptionGrammar & grammar = const_cast<OptionGrammar&>( _grammar );
      grammar._section = string( begin, end );
    }
    const OptionGrammar & _grammar;
  };

  struct captureOptionIdentifier
  {
    captureOptionIdentifier( const OptionGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      OptionGrammar & grammar = const_cast<OptionGrammar&>( _grammar );
      grammar._option = string( begin, end );
    }
    const OptionGrammar & _grammar;
  };

  struct captureOptionValue
  {
    captureOptionValue( const OptionGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      OptionGrammar & grammar = const_cast<OptionGrammar&>( _grammar );
      grammar._value = string( begin, end );
    }
    const OptionGrammar & _grammar;
  };

  template <typename SCANNER>
  struct definition
  {
    typedef rule< SCANNER > Rule;
    Rule lines, line, comment, sectionHeader, option, optionValue, identifier;
    
    Rule const &
    start() const 
    {
      return lines;
    }
    
    
    definition ( OptionGrammar const & self )
    {
      lines          = !( comment | sectionHeader ) 
                        >> *eol_p 
                        >> *( line >> *eol_p ) 
                        >> end_p;
      line           =    comment 
                        | sectionHeader 
                        | option[captureOption( self )] 
                        | +blank_p;
      comment        = *blank_p 
                        >> ';' 
                        >> +(anychar_p - eol_p );
      sectionHeader  = *blank_p 
                        >> ( '[' 
                          >> identifier[captureSection( self )]
                        >> ']' )
                        >> * blank_p;
      option         = *blank_p 
                        >> identifier[captureOptionIdentifier( self )]
                        >> *blank_p >> '=' 
                        >> *blank_p 
                        >> optionValue[captureOptionValue( self )];
                        ;
      optionValue    = +(anychar_p - eol_p );
      identifier     = ( alpha_p | '_' | '-' ) 
                        >> *( alnum_p | '_' | '-' );
    }
    
  };
  
  Options & _options;
  string    _section;
  string    _option;
  string    _value;
};

//------------------------------------------------------------------------------

OptionParser::OptionParser() 
{
}

//------------------------------------------------------------------------------

bool
OptionParser::parse( const string & input, Options & options )
{
  OptionGrammar grammar( options );
  parse_info<> result = boost::spirit::parse( input.c_str(), grammar );
  if ( ! result.full ) {
    _error = string( "Parsing error around: " ) + result.stop;
  }
  return result.full;
}

//------------------------------------------------------------------------------

const string & 
OptionParser::error() const
{
  return _error;
}
