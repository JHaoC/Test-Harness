///////////////////////////////////////////////////////////////////////
// TestUtilitiesDemo.cpp - facilities for single-user test harness   //
// ver 2.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include "Executive.h"
#include "../CodeUtilities/CodeUtilities.h"

#ifdef TEST_EXECUTIVE

#include "../Loader/Loader.h"
#include <iostream>
#include <iomanip>

using namespace Utilities;

int main()
{
  Cosmetic cos;

  std::cout << "\n  Testing Executive Package";
  std::cout << "\n ===============================\n";
  
  Executive ex;
  Path load_path = "../Debug/TestDrive.dll";

  ILog* pLog = Logger<0, Utilities::Lock>::getInstance();
  Message msg = "\n\n    Loading " + load_path + " which is a testDriver...";
  pLog->write(msg);

  Loader loader;
  if (!loader.loadAndExtract(load_path)) {
	  pLog->write(" failed to load dll or failed to load tests.\n    Terminating...\n");
	  return 0;
  }
  using sPtr = std::shared_ptr<ITest>;
  for (ITest* test : loader.tests()) {
	  test->acceptlogger(pLog);
	  sPtr pTest(test);
	  ex.registerTest(pTest, pTest->name());
  }
  bool result = ex.doTests();
  return result;
}
#endif
