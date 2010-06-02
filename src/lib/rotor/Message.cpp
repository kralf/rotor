#include "Message.h"
#include "Structure.h"

using namespace Rotor;
using namespace std;

//------------------------------------------------------------------------------

Message::Message( const string & aname, Structure & adata )
  : _name( aname ), 
    _data( adata )
{
}

//------------------------------------------------------------------------------

const string &
Message::name() const
{
  return _name;
}

//------------------------------------------------------------------------------

const Structure &
Message::data() const
{
  return _data;
}

//------------------------------------------------------------------------------

Structure &
Message::data()
{
  return _data;
}
