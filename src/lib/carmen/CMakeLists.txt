remake_find_library(carmen-ipc ipc.h PACKAGE CARMEN)

remake_add_library(carmen_registry LINK rotor ${CARMEN_IPC_LIBRARY})
remake_add_headers()
