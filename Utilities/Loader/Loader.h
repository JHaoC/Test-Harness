#pragma once
///////////////////////////////////////////////////////////////////////
// Loader.h : Test-Driver-DLL loader                                 //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Test hardness, CSE687 - Object Oriented Design       //
// Author:      Junhao Chen                                          //
//              juchen@syr.edu                                       //
// Original:    Ammar Salman, EECS Department, Syracuse University   //
//              313/788-4694, hoplite.90@hotmail.com                 //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package loads Test-Driver as DLLs following the ITests protocole 
*
*  The protocol forces all TestDrivers to have an implementation of ITests
*  interface which holds a collection of ITest pointers. This package will
*  attempt to load the DLL, get an instance of ITests* and retrieve the
*  std::vector<ITest*> contained within the DLL.
*
* Required Files:
* ---------------
*   ITest.h
*   Loader.h, Loader.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 07 Oct 2018
* - first release
* - taken from AMMARS_DLLLOADER/DllLoader package
*
* Notes:
* ------
* - declare all methods in the header file with inline
* - nothing else is needed since this will load the DLLs at RUNTIME.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/


#include "../ITest/ITest.h"
#include <vector>
#include <windows.h>

namespace Utilities
{
	
	//----<  define loader to load DLL >----------
	class Loader
	{
	public:
		Loader();
		~Loader();
		bool loadAndExtract(const std::string& dll); 
		const std::vector<ITest*>& tests() const; 
		void clear();  // unloads the DLL (if loaded) and clears all extracted tests

	private:
		std::vector<ITest*> extractedTests;  // contains the set of tests extracted from the TestDriver DLL
		HMODULE hmod;  // handle to the loaded DLL
	};


}


