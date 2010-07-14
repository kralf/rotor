from server import Server
from client import Client

def server( parameterFile ):
  module = Server( parameterFile )
  module.join()

def client( name ):
  module = Client( name )
  module.join()
