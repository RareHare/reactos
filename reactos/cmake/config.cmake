
set(SARCH "pc" CACHE STRING
"Sub-architecture to build for. Specify one of:
 pc xbox")

set(OARCH "pentium" CACHE STRING
"Generate instructions for this CPU type. Specify one of:
 pentium, pentiumpro")

set(TUNE "i686" CACHE STRING
"Which CPU ReactOS should be optimized for.")

set(OPTIMIZE "1" CACHE STRING
"What level of optimization to use.
 0 = off
 1 = Default option, optimize for size (-Os) with some additional options
 2 = Optimize for size (-Os)
 3 = Optimize debugging experience (-Og)
 4 = Optimize (-O1)
 5 = Optimize even more (-O2)
 6 = Optimize yet more (-O3)
 7 = Disregard strict standards compliance (-Ofast)")

set(LTCG FALSE CACHE BOOL
"Whether to build with link-time code generation")

set(GDB FALSE CACHE BOOL
"Whether to compile for debugging with GDB.
If you don't use GDB, don't enable this.")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DBG FALSE CACHE BOOL
"Whether to compile for debugging.")
else()
    set(DBG TRUE CACHE BOOL
"Whether to compile for debugging.")
endif()

if(MSVC)
    set(KDBG FALSE CACHE BOOL
"Whether to compile in the integrated kernel debugger.")
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(_WINKD_ FALSE CACHE BOOL "Whether to compile with the KD protocol.")
    else()
        set(_WINKD_ TRUE CACHE BOOL "Whether to compile with the KD protocol.")
    endif()

else()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(KDBG FALSE CACHE BOOL "Whether to compile in the integrated kernel debugger.")
    else()
        set(KDBG TRUE CACHE BOOL "Whether to compile in the integrated kernel debugger.")
    endif()
    set(_WINKD_ FALSE CACHE BOOL "Whether to compile with the KD protocol.")
endif()

set(_ELF_ FALSE CACHE BOOL
"Whether to compile support for ELF files.
Do not enable unless you know what you're doing.")

set(NSWPAT FALSE CACHE BOOL
"Whether to compile apps/libs with features covered software patents or not.
If you live in a country where software patents are valid/apply, don't
enable this (except they/you purchased a license from the patent owner).
This settings is disabled (0) by default.")

set(BUILD_MP TRUE CACHE BOOL
"Whether to compile the multi processor versions for ntoskrnl and hal.")

set(GENERATE_DEPENDENCY_GRAPH FALSE CACHE BOOL
"Whether to create a graphml dependency of dlls.")

if(MSVC)

set(_PREFAST_ FALSE CACHE BOOL
"Whether to enable PREFAST while compiling.")
set(_VS_ANALYZE_ FALSE CACHE BOOL
"Whether to enable static analysis while compiling.")

else()

set(USE_PSEH3 FALSE CACHE BOOL
"Whether to use the new PSEH3 library (requires GCC 4.5 and newer).")

endif()
