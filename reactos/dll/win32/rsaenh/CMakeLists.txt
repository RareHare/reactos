
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(rsaenh.dll rsaenh.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    aes.c
    des.c
    handle.c
    implglue.c
    md2.c
    mpi.c
    rc2.c
    rc4.c
    rsa.c
    rsaenh.c
    sha2.c
    rsrc.rc
    ${CMAKE_CURRENT_BINARY_DIR}/rsaenh.def)

add_library(rsaenh SHARED ${SOURCE})
set_module_type(rsaenh win32dll)
target_link_libraries(rsaenh uuid wine)
add_importlibs(rsaenh msvcrt crypt32 advapi32 kernel32 ntdll)
add_pch(rsaenh tomcrypt.h)
add_cd_file(TARGET rsaenh DESTINATION reactos/system32 FOR all)
