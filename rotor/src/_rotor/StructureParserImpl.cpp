#include "StructureParserImpl.h"
#include <boost/spirit.hpp>
#include <iostream>

using namespace std;
using namespace boost::spirit;
using namespace Rotor;

//------------------------------------------------------------------------------

struct StructureGrammar : public grammar<StructureGrammar>
{
  StructureGrammar( string & name, MemberDefinitions & members )
    : grammar<StructureGrammar>(),
      _name( name ),
      _members( members )
  {
  }

  struct captureName
  {
    captureName( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._name = string( begin, end );
    }
    const StructureGrammar & _grammar;
  };

  struct captureMemberName
  {
    captureMemberName( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._memberName = string( begin, end );
    }
    const StructureGrammar & _grammar;
  };

  struct captureType
  {
    captureType( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char * begin, const char * end ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._type = string( begin, end );
    }
    const StructureGrammar & _grammar;
  };

  struct captureCardinality
  {
    captureCardinality( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( int value ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._cardinality = value;
    }
    const StructureGrammar & _grammar;
  };

  struct captureNormal
  {
    captureNormal( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char *, const char * ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._members.resize( grammar._members.size() + 1 );
      MemberDefinition & member = grammar._members[grammar._members.size() - 1];
      member.name        = grammar._memberName;
      member.type        = grammar._type;
      member.cardinality = ONE;
    }
    const StructureGrammar & _grammar;
  };

  struct captureFixed
  {
    captureFixed( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char *, const char * ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._members.resize( grammar._members.size() + 1 );
      MemberDefinition & member = grammar._members[grammar._members.size() - 1];
      member.name        = grammar._memberName;
      member.type        = grammar._type;
      member.cardinality = grammar._cardinality;
    }
    const StructureGrammar & _grammar;
  };

  struct captureVariable
  {
    captureVariable( const StructureGrammar & grammar ) : _grammar( grammar ) {}
    void operator()( const char *, const char * ) const
    {
      StructureGrammar & grammar = const_cast<StructureGrammar&>( _grammar );
      grammar._members.resize( grammar._members.size() + 1 );
      MemberDefinition & member = grammar._members[grammar._members.size() - 1];
      member.name        = grammar._memberName;
      member.type        = grammar._type;
      member.cardinality = VARIABLE;
    }
    const StructureGrammar & _grammar;
  };

  template <typename SCANNER>
  struct definition
  {
    typedef rule< SCANNER > Rule;
    Rule  grammar, structure, identifier,
          declaration, typeDefinition,
          normalType, fixedArrayType, variableArrayType, integer;

    Rule const &
    start() const
    {
      return grammar;
    }


    definition ( StructureGrammar const & self )
    {
      grammar           = *space_p
                          >> structure
                          >> *space_p
                          >>';'
                          >> *space_p
                          >> end_p;
      structure         = "struct"
                          >> +space_p
                          >> identifier [captureName( self )]
                          >> *space_p
                          >> '{'
                          >> *declaration
                          >> '}';
      identifier        = ( alpha_p | '_' ) >> *( alnum_p | '_' );
      declaration       = *space_p
                          >> typeDefinition
                          >> *space_p;
      typeDefinition    =   normalType        [captureNormal( self )]
                          | fixedArrayType    [captureFixed( self )]
                          | variableArrayType [captureVariable( self )];
      normalType        = identifier    [captureType( self )]
                          >> +space_p
                          >> identifier [captureMemberName( self )]
                          >> *space_p
                          >> ';';
      fixedArrayType    = identifier    [captureType( self )]
                          >> +space_p
                          >> identifier [captureMemberName( self )]
                          >> *space_p
                          >> '['
                          >> *space_p
                          >> int_p      [captureCardinality( self )]
                          >> *space_p
                          >> ']'
                          >> *space_p
                          >> ';';
      variableArrayType = identifier    [captureType( self )]
                          >> *space_p
                          >> '*'
                          >> *space_p
                          >> identifier [captureMemberName( self )]
                          >> *space_p
                          >> ';';
    }

  };

  string            & _name;
  string              _memberName;
  string              _type;
  int                 _cardinality;
  MemberDefinitions & _members;
};

//------------------------------------------------------------------------------

StructureParserImpl::StructureParserImpl()
{
}

//------------------------------------------------------------------------------

bool
StructureParserImpl::parse( const string & declarationString )
{
  StructureGrammar grammar( _name, _members );
  parse_info<> result = boost::spirit::parse( declarationString.c_str(), grammar );
  if ( ! result.full ) {
    _error = string( "Parsing error around: " ) + result.stop;
    return false;
  } else {
    return true;
  }

}

//------------------------------------------------------------------------------

const MemberDefinitions &
StructureParserImpl::members() const
{
  return _members;
}

//------------------------------------------------------------------------------

const std::string &
StructureParserImpl::name() const
{
  return _name;
}

//------------------------------------------------------------------------------

const std::string &
StructureParserImpl::error() const
{
  return _error;
}
