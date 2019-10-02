///////////////////////////////////////////////////////////////////////
// Executor.cpp - facilities for single-user test harness            //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Junhao Chen                                          //
//              juchen@syr.edu                                       //
// Original:    Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include "Executor.h"
#include "../CodeUtilities/CodeUtilities.h"

#ifdef TEST_TESTEXECUTOR

//std::ostream& out = std::cout;

#include <iostream>
#include <iomanip>

using namespace Utilities;

int main() {
	
	Cosmetic cos;

	std::cout << "\n  Testing Executor Package";
	std::cout << "\n ===============================\n";

	Executor tester;
	Utilities::Loader loader;

	std::cout << "\n\n    Loading '../Debug/TestDrive.dll' which is a testDriver...";

	bool loaded = loader.loadAndExtract("../Debug/TestDrive.dll");
	if (!loaded) {
		std::cout << " failed to load dll or failed to load tests.\n    Terminating...\n";
		return -1;
	}

	std::cout << " successfully loaded";
	std::cout << "\n    Extracted tests:";
	for (auto test : loader.tests()) {
		std::cout << "\n      Test name: " << std::setw(30) << std::left << 
		test->name() << " - Author: " << std::setw(20) << test->author();
		std::cout << "\n      Executing test...";
		std::shared_ptr<ITest> ptest(test);
		
		//pass the logger pTest to test;
		tester.execute(ptest,ptest->name());
	}
	return 0;
}
#endif // TEST_TESTEXECUTOR

