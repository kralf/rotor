remake_add_documentation(
  DOXYGEN Doxyfile.remake
  INPUT ${REMAKE_PROJECT_SOURCE_DIR}/lib/rotor
    ${REMAKE_PROJECT_SOURCE_DIR}/lib/broadcast
  PATTERNS *.h
)
