// proxydll.cpp
#include "stdafx.h"
#include "proxydll.h"
#include "../log/log.h"
#include "../main.h"

// global variables
myIDirect3DSwapChain9 * gl_pmyIDirect3DSwapChain9;
myIDirect3DDevice9 * gl_pmyIDirect3DDevice9;
myIDirect3D9 * gl_pmyIDirect3D9;
HINSTANCE gl_hOriginalDll;
HINSTANCE gl_hThisInstance;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{   
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: Start(hModule); InitInstance(hModule); break;
	    case DLL_PROCESS_DETACH: ExitInstance(); Close(); break;
        
        case DLL_THREAD_ATTACH:  break;
	    case DLL_THREAD_DETACH:  break;
	}
    return TRUE;
}

// Exported function (faking d3d9.dll's one-and-only export)
IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
	if (!gl_hOriginalDll) LoadOriginalDll(); // looking for the "right d3d9.dll"
	
	// Hooking IDirect3D Object from Original Library
	typedef IDirect3D9 *(WINAPI* D3D9_Type)(UINT SDKVersion);
	D3D9_Type D3DCreate9_fn = (D3D9_Type) GetProcAddress( gl_hOriginalDll, "Direct3DCreate9");
    
    // Debug
	if (!D3DCreate9_fn) 
    {
        Log::Debug("PROXYDLL: Pointer to original D3DCreate9 function not received ERROR ****");
        ExitProcess(0); // exit the hard way
    }
	
	// Request pointer from Original Dll. 
	IDirect3D9 *pIDirect3D9_orig = D3DCreate9_fn(SDKVersion);
	
	// Create my IDirect3D8 object and store pointer to original object there.
	// note: the object will delete itself once Ref count is zero (similar to COM objects)
	gl_pmyIDirect3D9 = new myIDirect3D9(pIDirect3D9_orig);
	
	// Return pointer to hooking Object instead of "real one"
	return (gl_pmyIDirect3D9);
}

void InitInstance(HANDLE hModule) 
{
	Log::Debug("PROXYDLL: InitInstance called.");
	
	// Initialisation
	gl_hOriginalDll = NULL;
	gl_hThisInstance = NULL;
	gl_pmyIDirect3D9 = NULL;
	gl_pmyIDirect3DDevice9 = NULL;
	gl_pmyIDirect3DSwapChain9 = NULL;
	
	// Storing Instance handle into global var
	gl_hThisInstance = (HINSTANCE)hModule;
}

void LoadOriginalDll(void)
{
    char buffer[MAX_PATH];
    
    // Getting path to system dir and to d3d9.dll
	GetSystemDirectoryA(buffer,MAX_PATH);

	// Append dll name
	strcat_s(buffer, MAX_PATH, "\\d3d9.dll");
	
	// try to load the system's d3d9.dll, if pointer empty
	if (!gl_hOriginalDll) gl_hOriginalDll = LoadLibraryA(buffer);

	// Debug
	if (!gl_hOriginalDll)
	{
		Log::Debug("PROXYDLL: Original d3d9.dll not loaded ERROR ****");
		ExitProcess(0); // exit the hard way
	}
}

void ExitInstance() 
{    
    Log::Debug("PROXYDLL: ExitInstance called.");
	
	// Release the system's d3d9.dll
	if (gl_hOriginalDll)
	{
		FreeLibrary(gl_hOriginalDll);
	    gl_hOriginalDll = NULL;  
	}
}

