#pragma once
/////////////////////////////////////////////////////////////////
// Tester.h - test harness                                     //
// ver 2.0                                                     //
// Language:    C++, Visual Studio 2017                        //
// Application: Most Projects, CSE687 - Object Oriented Design //
// Author:      Junhao Chen                                    //
//              juchen@syr.edu                                 //
/////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a class, Tester:
*  - uses Loader to load Dll libraries and extract tests.
*  - provides pointer to its member logger to each test, then
*    executes it, using the infrastructure from TestUtilities.
*
* Required Files:
* ---------------
*  TestHardness.h TestHardness.cpp
*  Loader.h, Loader.cpp
*  ISingletonLogger.h, SingletonLogger.h
*  ITest.h, Exective.h
*  Properties.h, Properties.cpp
*  FileUtilities.h, FileUtilities.cpp
*  FileSystem.h, FileSystem.cpp
*  StringUtilities.h
*
*
* Maintenance History:
* --------------------
* ver 2.1 : 7 Nov 2018
* - implement comm 
* ver 2.0 : 5 Nov 2018
* - Copy stuff from Profreessor Jim Fawcett
* - Tester.cpp directly use his work
* ver 1.0 : 11 Oct 2018
* - first release
*
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#ifndef WIN32_LEAN_AND_MEAN  // prevents duplicate includes of core parts of windows.h in winsock2.h
#define WIN32_LEAN_AND_MEAN
#endif

#include "../TestRequest/TestRequest.h"
#include "../Utilities/Loader/Loader.h"
#include "../Utilities/SingletonLogger/ISingletonLogger.h"
#include "../Utilities/SingletonLogger/SingletonLogger.h"
#include "../Utilities/Properties/Properties.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <vector>
#include <string>

namespace Testing
{
  using namespace Utilities;
  using namespace MsgPassingCommunication;

  ///////////////////////////////////////////////////////////////////
  // Tester class
  // - Loads TestRequests from requestPath.
  //   Each TestRequest provides names of Dlls to load and test
  // - Loads Test Dynamic Link Libraries (Dlls) from libPath.
  // - Executes tests defined in each Dll.
  // - Each tests logs ouputs using Tester class's logger


  class Tester
  {
	public:
		Tester();
		// promoting construter, mother of server can create a child as her request
		Tester(const std::string & nm, const std::string & addr, const int & portC, const std::string & addrM, const int & portM);
		~Tester();
		Tester(const Tester& ts) = delete;
		Tester operator=(const Tester& ts) = delete;
	
		Property<EndPoint> childEp;
		Property<EndPoint> motherEp;
		Property<std::string> name;
		Property<std::string> libPath;
		Property<ILog*> pLog;

		void startUp();
		void getTestrequest();  //recieve testrequest and do test
		void signtoMother();    //send ready sign to mother 
		void sentResult(MsgPassingCommunication::Message msg);
		bool loadLibraries(TestRequest& tr);
		bool doTests();
		void setLibraryPath(std::string& path)
		{
			libPath(path);
		}
		void clear()
		{
			dllLoader_.clear();
		}
	private:
		Loader dllLoader_;
		Comm testCom_;
  };
}
