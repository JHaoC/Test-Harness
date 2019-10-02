/////////////////////////////////////////////////////////////////////
// Tester.cpp - Prototype for Test Harness child Tester            //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#include "Tester.h"
#include "../Utilities/ITest/ITest.h"
#include "../Utilities/Executive/Executive.h"
#include "../Utilities/FileSystem/FileSystem.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>
#include <thread>
#include <functional>
#include <windows.h>

namespace Testing
{
	using namespace MsgPassingCommunication;
  //----< initialize paths and logger >------------------------------

  Tester::Tester(): testCom_(EndPoint("localhost", 8090), "child")
  {
 
	libPath("../SaveFile");
	name("child");
	EndPoint ip_port("localhost", 8090);
	childEp(ip_port);
	EndPoint ip_portM("localhost", 8080);
	motherEp(ip_portM);
    pLog(Utilities::Logger<0, Utilities::Lock>::getInstance());
    pLog()->setTerminator("");
	testCom_.setSendFilePath("../SendFile");
	testCom_.setSaveFilePath("../SaveFile");
	
  }
  //----< deallocate logger >----------------------------------------

  Tester::~Tester()
  {
    delete pLog();
  }

  //----< Promoting constructor>------------------------------
  Tester::Tester(const std::string & nm, const std::string & addrC, const int & portC, const std::string & addrM, const int & portM) : testCom_(EndPoint(addrC,portC), nm)
  {
	  libPath("../SaveFile");
	  name(nm);
	  EndPoint ip_port(addrC,portC);
	  childEp(ip_port);
	  EndPoint ip_portM(addrM, portM);
	  motherEp(ip_portM);
	  pLog(Utilities::Logger<0, Utilities::Lock>::getInstance());
	  pLog()->setTerminator("");
	  testCom_.setSendFilePath("../SendFile");
	  testCom_.setSaveFilePath("../SaveFile");
  }

  //----< Starting up com for child process>------------------------------
  void Tester::startUp()
  {
	  testCom_.start();
  }

  //----< Listen and grab TestRequest from mother processes >------------------------------
  void Tester::getTestrequest()
  {
	  MsgPassingCommunication::Message msg;
	  while (true)
	  {
		  msg = testCom_.getMessage();
		  pLog()->write( "\n  Child  recvd  message \"" + msg.name() + "\"");
		  if (msg.containsKey("sendingFile"))  // is this a file message?
		  {
			  if (msg.contentLength() == 0)
				  pLog()->write("\n  " + name() + " received file \"" + msg.file() + "\" from " + msg.name());
		  }
		  if (msg.command() == "stop")
			  break;
		  pLog()->write("\n  loaded TestRequest \"" + msg.name() + "\"");
		  pLog()->write("\n  " + msg.toString() + "\n");

		  if (msg.containsKey("testRequest") && msg.testRequest() != "") 
		  {
			  // add a log file
			  std::string path = testCom_.getSendFilePath();
			  std::string logfilename = msg.value("testLogFile");
			  std::string fileSpec = path +"\\"+ logfilename;
			  std::ofstream outfileStrm(fileSpec, std::ios::out);
			  if (outfileStrm.good())
				  pLog()->addStream(&outfileStrm);  

			  TestRequest tr = TestRequest::fromString(msg.testRequest());
			  if (loadLibraries(tr))
			  {
				  msg.attribute("TestResult", "passed");
			  }
			  else
			  {
				  msg.attribute("TestResult", "failed");
			  }
			  pLog()->removeStream(&outfileStrm);
			  msg.attribute("sendingFile",logfilename);
			  //make client port as destine and send msg back with the TestResult
			  msg.to(msg.from());
			  
			  // wrap in a thread and detached.
			  std::function<void()> tproc = [&]() { sentResult(msg); };
			  std::thread tget(tproc);
			  tget.detach();
		  }
	  }
  }

  //----< load libraries >---------------------------------------------
  /*
  * - Loads all libraries specified in a TestRequest from libPath.
  * - Uses DllLoader member, dllLoader_, to load dlls and extract
  *   their tests.
  */
  bool Tester::loadLibraries(TestRequest& tr)
  {
    std::string path = FileSystem::Path::getFullFileSpec(libPath());
    pLog()->write("\n  loading from \"" + path + "\"");

    std::vector<std::string> files;
    for (Dll dll : tr.request.valueRef())
    {
      files.push_back(dll);
    }
	bool res = true;
    for (auto file : files)
    {
      pLog()->write("\n    " + file);
      std::string fileSpec = path + "\\" + file;
	  if (dllLoader_.loadAndExtract(fileSpec))
	  {
		  pLog()->write("\n Loading succeed");
	  }
	  else { pLog()->write("\n Loading failed"); }
	  
	  if (!doTests())
	  {
		 res = false;
	  }
    }
	return res;
    //pLog()->write("\n");
	//pLog()->write("\n Loading finish");
  }

  //----< executes tests >--------------------------
  /*
  * - Executes tests held by DllLoader instance, 
  *   using TestUtilities infrastructure.
  */
  bool Tester::doTests()
  {
    using sPtr = std::shared_ptr<ITest>;
    Utilities::Executive te;
    for (ITest* test : dllLoader_.tests()) {
      // Host passes logger resource pointer to test
      // - test will use logger via this pointer
      test->acceptlogger(pLog.valueRef());
      sPtr pTest(test);
      te.registerTest(pTest, pTest->name());
    }
    bool result = te.doTests();
    return result;
  }

  //----< when que is empty sent sign to mother >--------------------------
  void Tester::signtoMother()
  {
	  EndPoint mother = motherEp();
	  MsgPassingCommunication::Message msg;
	  msg.name(name());
	  msg.to(mother);
	  msg.from(childEp());
	  msg.types("child");
	  std::string pt = libPath();
	  msg.path(pt);
	  testCom_.postMessage(msg);
	  std::cout << "\n Assign to Mother Success\n";
  }

  //----< sent Result to Client >--------------------------
  void Tester::sentResult(MsgPassingCommunication::Message msg)
  {
	  std::cout << "send Back "<< msg.toString()<<std::endl;
	  testCom_.postMessage(msg);
	  signtoMother();
  }
}

using namespace Testing;

int main(int argc, char* argv[])
//int main()
{
  std::cout << Utilities::sTitle("Testing Tester recive Testrequest from Mother and sent back to client");
  //
 /* Testing::TestRequest tr;
  tr.name("tr");
  tr.author("Junhao");
  tr.date(Utilities::DateTime().now());
  tr.request.valueRef().push_back("TestDriver1.dll");
  tr.request.valueRef().push_back("TestDrive.dll");
*/
  //Tester tester;

  //tester.loadLibraries(tr);
  //tester.doTests();
  ////----------------------------Dll problem.


  if (argc == 0)
  {
	  std::cout << " Create new Tester failed\n";
	  return 0;
  }

  //// test in Visual Studio
  //std::string name(argv[1]);
  //std::string addrC(argv[2]);
  //int portC = std::stoi(argv[3]);
  //std::string addrM(argv[4]);
  //int portM = std::stoi(argv[5]);

  //In the real 
  std::string name(argv[0]);
  std::string addrC(argv[1]);
  int portC = std::stoi(argv[2]);
  std::string addrM(argv[3]);
  int portM = std::stoi(argv[4]);

  Tester tester(name, addrC, portC, addrM, portM);
  tester.startUp();
  std::cout << "\n Assign to Mother\n";
  tester.signtoMother();

  std::cout << "\n listenning" << std::endl;

  std::function<void()> tproc = [&](){ tester.getTestrequest(); };
  std::thread tget(tproc);
  tget.join();
  
  Utilities::putline(2);
  getchar();
  getchar();
  return 0;

}

