remake_project_option(WITH_TESTING "testing binaries" ON)
remake_project_option(WITH_SANDBOX "sandbox utilities" ON)
remake_project_option(WITH_GUI "graphical user interface (GUI)" ON)

remake_find_package(Boost COMPONENTS system)

remake_add_directories(lib)
remake_add_directories(bin)
