/*
 * PROJECT:     Ports installer library
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        dll/win32/msports/msports.c
 * PURPOSE:     Library main function
 * COPYRIGHT:   Copyright 2011 Eric Kohl
 */

#include <windows.h>
#include <wine/debug.h>


WINE_DEFAULT_DEBUG_CHANNEL(msports);


BOOL
WINAPI
DllMain(HINSTANCE hinstDll,
        DWORD dwReason,
        LPVOID reserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            TRACE("DLL_PROCESS_ATTACH\n");
            DisableThreadLibraryCalls(hinstDll);
            break;
    }

    return TRUE;
}

/* EOF */