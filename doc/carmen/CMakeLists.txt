remake_project_get(DOCUMENTATION_DESTINATION)
remake_add_documentation(
  DOXYGEN Doxyfile.remake
  INPUT ${REMAKE_PROJECT_SOURCE_DIR}/lib/carmen
  PATTERNS *.h
  INSTALL ${DOCUMENTATION_DESTINATION}/carmen
)
