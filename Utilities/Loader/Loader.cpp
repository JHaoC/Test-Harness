////////////////////////////////////////////////////////////////////////////
// DllLoader.cpp : implementation and testing for DllLoader.h             //
// ver 1.0                                                                //
//                                                                        //
// Application : OOD F18 Project 2 Help                                   //
// Platform    : VS17 Community - Windows 10 Professional x64             //
// Author      : Ammar Salman, EECS Department, Syracuse University       //
//               313/788-4694, hoplite.90@hotmail.com                     //
////////////////////////////////////////////////////////////////////////////

#include "Loader.h"

using namespace Utilities;

//----<  help function to convert std::string to std::wstring >----------
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//----<  declare constructor of loader >----------
Loader::Loader() {}

//----<  declare destructor >----------
Loader::~Loader()
{
	clear(); // to make sure the DLL is unloaded
}

//------<  accepts a DLL path and attempts to load it and extract its tests >----------
bool Loader::loadAndExtract(const std::string & dll)
{
	hmod = LoadLibrary(s2ws(dll).c_str()); // load DLL
	if (!hmod) return false;  // check if DLL was properly loaded

	// check if the DLL contains get_ITests() function ...
	// in other words, check if the DLL complies with the protocol

	typedef ITests* (*gITs)(); 

	gITs get_ITests = (gITs)GetProcAddress(hmod, "get_ITests");
	if (!get_ITests) return false;

	// execute get_ITests() to get the collection of tests
	ITests* tests = get_ITests();
	if (tests) 
	{ // check if tests is nullptr before attempting to get its tests collection
		extractedTests=tests->tests();
		return true;
	}

	// this will only be reached if get_ITests() returned nullptr in which case the 
	// TestDriver DLL contains no tests to execute.
	return false;
}

const std::vector<ITest*>& Loader::tests() const
{
	return extractedTests;
}

void Loader::clear()
{
	if (hmod) FreeLibrary(hmod);
	extractedTests.clear();
}



#ifdef TEST_LOADER

//using namespace Utilities;

#include <iostream>
#include <iomanip>

int main() {
	std::cout << "\n  Testing DLL Loader";
	std::cout << "\n ====================";

	Loader loader;

	std::cout << "\n\n    Loading '../Debug/TestDrive.dll' which is a testDriver...";

	bool loaded = loader.loadAndExtract("../Debug/TestDrive.dll");
	if (!loaded) {
		std::cout << " failed to load dll or failed to load tests.\n    Terminating...\n";
		return -1; 
	}

	std::cout << " successfully loaded";
	std::cout << "\n    Extracted tests:";
	for (auto test : loader.tests()) {
		std::cout << "\n      Test name: " << std::setw(30) << std::left << test->name() << " - Author: " << std::setw(20) << test->author();
		std::cout << "\n      Executing test...";
		bool res = test->test();
		std::cout << "\n\n      Test passed: " << std::to_string(res);
	}


	//cout << "\n\n    Loading '../Debug/TestDriver2.dll' which is a multiple-tests TestDriver ...";
	//loader.clear();

	//loaded = loader.loadAndExtract("../Debug/TestDriver2.dll");
	//if (!loaded) {
	//	cout << " failed to load dll or failed to load tests.\n    Terminating...\n";
	//	return -1;
	//}

	//cout << " successfully loaded";
	//cout << "\n    Extracted tests:";
	//for (auto test : loader.tests()) {
	//	cout << "\n      Test name: " << setw(30) << left << test->name() << " - Author: " << setw(20) << test->author();
	//	//cout << "\n      Executing test...";
	//	//bool res = test->test();
	//	//cout << "\n\n      Test passed: " << to_string(res);
	//}

	std::cout << "\n\n";
	return 0;
}

#endif