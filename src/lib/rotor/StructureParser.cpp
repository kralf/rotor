#include "StructureParser.h"
#include "StructureParserImpl.ipp"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

StructureParser::StructureParser()
  : _impl( new StructureParserImpl )
{
}

//------------------------------------------------------------------------------

StructureParser::~StructureParser()
{
  delete _impl;
}

//------------------------------------------------------------------------------

bool
StructureParser::parse( const string & declarationString )
{
  return _impl->parse( declarationString );
}

//------------------------------------------------------------------------------

const MemberDefinitions & 
StructureParser::members() const
{
  return _impl->members();
}

//------------------------------------------------------------------------------

const std::string & 
StructureParser::name() const
{
  return _impl->name();
}

//------------------------------------------------------------------------------

const std::string & 
StructureParser::error() const
{
  return _impl->error();
}
