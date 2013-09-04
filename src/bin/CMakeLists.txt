remake_include(../lib)

remake_add_directories(
  example
  COMPONENT example
  IF CARMEN_CORE_FOUND
)
remake_add_directories(
  test
  COMPONENT test
)
