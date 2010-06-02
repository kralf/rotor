remake_include(../lib)

remake_add_directories(
  sandbox
  COMPONENT sandbox
  IF WITH_SANDBOX
)
remake_add_directories(
  test
  COMPONENT test
  IF WITH_TESTING
)
