/////////////////////////////////////////////////////////////////////
// Mother_TestHardness.cpp - Mother for Test Harness               //
// ver 1.0                                                         //
// Junhao Chen, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#include "Mother_TestHarness.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include <thread>
#include <functional>
#include <windows.h>
#include <iostream>
#include "../Process/Process.h"

namespace MotherSide
{
	using namespace MsgPassingCommunication;

	//----< initialize Ip and port of mother and mothercom >------------------------------
	MotherSide::Mother::Mother(): name_("mother"), motheripport_(EndPoint("localhost", 8090)), motherCom_(motheripport_, name_)
	{
		motherCom_.setSaveFilePath("../SaveFile"); //send files to children's folders
		motherCom_.setSendFilePath("../SendFile"); //take files by clients sending
		childTestCount_ = 2;
		childID_ = 1;
	}

	//----< Promoting constructor >------------------------------
	Mother::Mother(EndPoint & clpt, int num): name_("mother"), motheripport_(clpt), motherCom_(motheripport_, name_)
	{
		childTestCount_ = num;
		motherCom_.setSaveFilePath("../SaveFile"); //send files to children's folders
		motherCom_.setSendFilePath("../SendFile"); //take files by clients sending
		childID_ = 1;
	}

		
	//----< Start up Mother comm, testers and clients >------------------------------
	void Mother::startUp()
	{
		motherCom_.start();
		for (int i = 0; i < childTestCount_; i++)
		{
			createChildTester();
		}
		//createClient();	
	}

	//----<create a tester process  >------------------------------
	void Mother::createChildTester()
	{
		std::ostringstream name, cmdline;
		name << "ChildTester" << childID_;
		std::string addr = motheripport_.address;
		size_t portC = motheripport_.port + childID_;
		cmdline << name.str() << " " << addr << " " << portC << " " << addr << " " << motheripport_.port;
		std::cout << cmdline.str();
		Process tester;
		tester.title(name.str());
		tester.application("../Debug/TestHarnessChild.exe");
		tester.commandLine(cmdline.str());
		if (tester.create()) {
			std::cout << "\n succeed\n";
		}
		else
			std::cout << "\n failure \n";
		childID_++;
		std::cout << "\n   Create New ChildTester   \n";
	}

	//----<create a client process  >------------------------------
	void Mother::createClient()
	{
		std::ostringstream cmdline;
		std::string name = "Client";
		std::string addr = motheripport_.address;
		size_t portC = motheripport_.port + childID_;
		//cmdline << name << " " << addr << " " << motheripport_.port << " " << addr << " " << portC;
		Process clients;
		clients.title(name);
		clients.application("../Debug/Client.exe");
		//clients.commandLine(cmdline.str());
		clients.create();
		std::cout << "\n   Create Client   \n";
	}

	//----< fork a thread to listen and get msg >-----------
	void Mother::listen()
	{
		std::function<void()> tproc = [&]() { getmessages(); };
		recvthread = std::thread(tproc);
	}

	//----< grab messages >------------------------------
	void Mother::getmessages()
	{
		MsgPassingCommunication::Message msg;
		while (true)
		{
			msg = motherCom_.getMessage();
			if (msg.command() == "stop")
				break;
			putMsgtoQue(msg);
		}
	}

	//----< Check message and send to Ques>------------------------------
	void Mother::putMsgtoQue(Message & msg)
	{
		// Enter Child into ReadyQue
		if (msg.types() == "child")  // check where the msg come from
		{
			readyQue_.enQ(msg);
			std::cout << "\n  " + msg.name() + " arrived at mother \"\n";
		}
		// Note recieve files
		else if (msg.containsKey("sendingFile"))  // is this a file message?
		{
			if (msg.contentLength() == 0)
			{
				std::cout << "\n  " + name_ + " received file \"" + msg.file() + "\" from " + msg.name();
				//testRequestQue_.enQ(msg);
				
			}
		}
		// put normal command
		else
		{
			std::cout << "\n  " + name_ + " received file \"" + msg.toString();
			testRequestQue_.enQ(msg);
		}
	}

	//----< Set children's folder for sending>------------------------------
	void Mother::setchildpath(const Path & path)
	{
		motherCom_.setSaveFilePath(path);
	}

	//----< Set Mother saving files folder>------------------------------
	void Mother::setmotherpath(const Path & path)
	{
		motherCom_.setSendFilePath(path);
	}

	//----< Get Child path>------------------------------
	Path Mother::getchildpath()
	{
		return motherCom_.getSaveFilePath();
	}

	//----< Get Mother Path>------------------------------
	Path Mother::getmotherpath()
	{
		return motherCom_.getSendFilePath();
	}

	void Mother::wait()
	{
		if (recvthread.joinable())
			recvthread.join();
	}

	//----< Check TestRequestQue and ReadyQue>------------------------------
	void Mother::poolCheck()
	{
		while (true)
		{
			if (readyQue_.size() != 0)
			{
				if (testRequestQue_.size() != 0)
				{
					sentTestRequest();
				}
			}
		}
	}

	//----< Sent TestRequest to child >------------------------------
	void Mother::sentTestRequest()
	{
		Message testRq = testRequestQue_.deQ();
		Message child = readyQue_.deQ();
		testRq.to(child.from());
		/*if (child.path() != getchildpath()) {
			setchildpath(child.path());
		}*/
		motherCom_.postMessage(testRq);
		std::cout << "send TestRequest " + testRq.name() + " to " + child.name() << std::endl;
	}
}

using namespace MotherSide;

int main()
{
	std::cout << Utilities::sTitle("Testing Mother");

	Mother testharness;
	testharness.startUp();
	testharness.listen();
	testharness.poolCheck();
	testharness.wait();
	

	

}


