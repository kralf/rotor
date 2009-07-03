elrob_point_6d_t = '''
  struct elrob_point_6d_t {
    double x;
    double y;
    double z;
    double phi;
    double theta;
    double psi;
  };
'''

import rotorc

options  = rotorc.BaseOptions() 
options.setInt( "BOOTSTRAP", "loggingLevel", 7 )
registry = rotorc.Registry.load( "CarmenRegistry", "path_follow", options, "lib" )
registry.registerType( elrob_point_6d_t )
s = registry.newStructure( "elrob_point_6d_t", None )