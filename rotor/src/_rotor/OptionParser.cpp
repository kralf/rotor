#include "OptionParser.h"
#include <Hapy/Parser.h>
#include <iostream>
#include <algorithm>

using namespace Hapy;
using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

OptionParser::OptionParser() 
{
  _lines          = !( _comment | _sectionHeader ) >> *(eol_r >> _line ) >> *eol_r >> end_r;
  _line           = _comment | _sectionHeader | _option | *space_r;
  _comment        = *space_r >> ';' >> +(anychar_r - eol_r );
  _sectionHeader  = *space_r >> '[' >> _identifier >> ']' >> * space_r;
  _option         = *space_r >> _identifier >> *space_r >> '=' >> *space_r >> _optionValue;
  _optionValue    = +(anychar_r - eol_r );
  _identifier     = ( alpha_r | '_' | '-' ) >> *( alnum_r | '_' | '-' );
  
  _lines.verbatim( true );
  
  _identifier.committed( true );

  _identifier.leaf( true );
  _comment.leaf( true );
  _optionValue.leaf( true );
}

//------------------------------------------------------------------------------

bool
OptionParser::parse( const string & input, Options & options )
{
  Parser parser;
  parser.grammar( _lines );
  _error = "";
  if ( ! parser.parse( input ) ) {
    _error = parser.result().location();
    return false;
  }

  const Pree & pree = parser.result().pree;

  string section;
  string option;
  string value;

  if ( pree[0].count() == 1 ) {
    if ( pree[0][0][0].rid() == _sectionHeader.id() ) {
      section = pree[0][0][0][2].image();
    }
  }
  
  for ( size_t i = 0; i < pree[1].count(); i++ ) {
    if ( pree[1][i][1][0].rid() == _sectionHeader.id() ) {
      section = pree[1][i][1][0][2].image();
    }
    if ( pree[1][i][1][0].rid() == _option.id() ) {
      option = pree[1][i][1][0][1].image();
      value  = pree[1][i][1][0][5].image();
      options.setString( section, option, value );
    }
  }

  return true;
}

//------------------------------------------------------------------------------

const string & 
OptionParser::error() const
{
  return _error;
}
