///////////////////////////////////////////////////////////////////////
// Process.cpp - class used to start process                         //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018         //
///////////////////////////////////////////////////////////////////////

#include "Process.h"
#include <iostream>

#ifdef TEST_PROCESS

int main()
{
  std::cout << "\n  Demonstrating code pop-up in notepad";
  std::cout << "\n ======================================";

  Process p;
  p.title("test application");
  std::string mother = "../Debug/Mother_TestHardness.exe";
  p.application(mother);


  //std::string cmdLine = "/A ../Process/Process.h";
  //p.commandLine(cmdLine);

  std::cout << "\n  starting process: \"" << mother << "\"";
  //std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
  p.create();


  Process p1;
  p1.title("TestHardness_Child");
  p1.application("../Debug/TestHarnessCore.exe");
  p1.create();

  Process c1;
  c1.title("Client");
  c1.application("../Debug/Client.exe");
  c1.create();

  //CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
  //p.setCallBackProcessing(callback);
  //p.registerCallback();

  std::cout << "\n  after OnExit";
  std::cout.flush();
  char ch;
  std::cin.read(&ch,1);
  return 0;
}

#endif
