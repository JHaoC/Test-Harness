#pragma once
/////////////////////////////////////////////////////////////////
// Mother_TestHardness - Mother of Test Hardness               //
// ver 2.0                                                     //
// Language:    C++, Visual Studio 2017                        //
// Application: Most Projects, CSE687 - Object Oriented Design //
// Author:      Junhao Chen                                    //
//              juchen@syr.edu                                 //
/////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a class, Mother:
*  - uses Process pool to control TestRequests.
*  - Recieve msg from Child TestHardness and Clients.
*  - Send TestRequests to Child TestHardness to run tests.
*  
*
* Required Files:
* ---------------
*  TestRequest.h, TestRequest.cpp
*  FileUtilities.h, FileUtilities.cpp
*  FileSystem.h, FileSystem.cpp
*  
*  Comm.h, Comm.cpp
*  StringUtilities.h
*
* Maintenance History:
* --------------------
* ver 2.0 : 10 Nov 2018
* - reset mother method and remove sendthread
* ver 1.0 : 8 Nov 2018
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
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../Process/Process.h"
#include <vector>
#include <string>

namespace MotherSide
{
	//using namespace Utilities;

	///////////////////////////////////////////////////////////////////
	// Mother class
	// - Create child processes 
	//   Each Child provides name, IPAddr and Port of Dlls.
	// - Listen messages from clients and children 
	// - Send msg to childs
	// - Shupdown system

	using namespace MsgPassingCommunication;

	class Mother
	{
	public:
		Mother();
		Mother(EndPoint & clpt, int num);
		~Mother(){ }
		Mother(const Mother& m) = delete;
		Mother operator=(const Mother& c) = delete;

		void startUp();  // start up server, server's com and new processes testers and clients 
		void createChildTester();  //create A children tester process
		void createClient();       //create A client process
		void sentTestRequest();
		void getmessages();
		void listen(); //wrap getmessages into recvthread and continuely get messeges
		void poolCheck(); //continuely send messeges to children if there is a message and an ready tester
		void putMsgtoQue(Message & msg);
		void setchildpath(const Path & path);
		void setmotherpath(const Path & path);
		Path getchildpath();
		Path getmotherpath();
		void wait(); // join the recvthread

		
	private:
		std::string name_;
		EndPoint motheripport_;
		MsgPassingCommunication::Comm motherCom_;
		BlockingQueue<Message> testRequestQue_;
		BlockingQueue<Message> readyQue_;
		int childTestCount_;
		int childID_;
		std::thread recvthread;
	};
}
