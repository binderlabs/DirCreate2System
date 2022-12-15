// dll_spawn_cmd.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "dll_spawn_cmd.h"


// This is an example of an exported variable
DLLSPAWNCMD_API int ndllspawncmd=0;

// This is an example of an exported function.
DLLSPAWNCMD_API int fndllspawncmd(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cdllspawncmd::Cdllspawncmd()
{
    return;
}
