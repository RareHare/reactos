
add_definitions(-Dtzset=_tzset)

include_directories(include)

spec2def(dhcpcsvc.dll dhcpcsvc.spec ADD_IMPORTLIB)

list(APPEND SOURCE 
    dhcp/adapter.c
    dhcp/alloc.c
    dhcp/api.c
    dhcp/compat.c
    dhcp/dhclient.c
    dhcp/dispatch.c
    dhcp/hash.c
    dhcp/options.c
    dhcp/pipe.c
    dhcp/socket.c
    dhcp/tables.c
    dhcp/util.c
    dhcpcsvc.c
    dhcpcsvc.rc
    ${CMAKE_CURRENT_BINARY_DIR}/dhcpcsvc_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/dhcpcsvc.def)

add_library(dhcpcsvc SHARED ${SOURCE})

set_module_type(dhcpcsvc win32dll)

add_importlibs(dhcpcsvc ws2_32 iphlpapi advapi32 msvcrt kernel32 ntdll)


add_pch(dhcpcsvc include/rosdhcp.h)

add_cd_file(TARGET dhcpcsvc DESTINATION reactos/system32 FOR all)
