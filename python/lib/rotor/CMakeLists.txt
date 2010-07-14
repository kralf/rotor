remake_include(../../../src/lib)

remake_python_swig(DEPENDS ../../../src/lib/rotor/*.h)
remake_python_package(RECURSE)
