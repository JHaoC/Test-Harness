/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Prototype for OOD Project #4               //
// ver 1.1                                                         //
// Junhao Chen                                                     //
// Original from:                                                  //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines one class MainWindow that provides tabs:
 * - Find Libs: Navigate through local directory to find files for testing
 *   - Shows local directories and files
 *   - Can navigate by double clicking directories
 *   - Can select files by selecting.  That displays selections in a popup window.
 * - Request Tests: Show Test requests selected by users
 * - Show Results:  Show tests result pass or fail and give a name of log file.
 * 
 * Required Files:
 * ---------------
 * MainWindow.xaml, MainWindow.xaml.cs
 * SelectionWindow.xaml, SelectionWindow.xaml.cs
 * 
 * Maintenance History:
 * --------------------
 * ver 1.1 : 5 Dec 2018
 * - add add commondlines access.
 * ver 1.0 : 29 Nov 2018
 * - first release.
 * - Copy from Jim Fawcett.
 * 
 * 
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO;
using System.Threading;
using MsgPassingCommunication;

namespace PrototypeGui_OOD_Pr4
{
    ///////////////////////////////////////////////////////////////////
    // MainWindow class
    // - Provides navigation and selection to find libraries to test.
    // - Creates a popup window to handle selections.

    public partial class MainWindow : Window
    {
        public string Path { get; set; }
        List<string> SelectedFiles { get; set; } = new List<string>();
        public Dictionary<string, List<string>> TestRequests { get; set; } = new Dictionary<string,List<string>>();
        public SelectionWindow Swin { get; set; } = null;
        bool unselecting = false;
       
        private Translater translater;
        private CsEndPoint endPoint_;
        private Thread rcvThrd = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_ 
            = new Dictionary<string, Action<CsMessage>>();
        private string Clientname { get; set; }
        private string Author { get; set; }
        


        public MainWindow()
        {
            InitializeComponent();
        }

        //----< open window showing contents of project directory >------

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            
            string[] args = Environment.GetCommandLineArgs();
            Path = Directory.GetCurrentDirectory();
            string logpath = GetAncestorPath(1, Path);

            Path = GetAncestorPath(2, Path);
            LoadNavTab(Path);
            Clientname = "GUI_client";
            Author = "Junhao Chen";
          
            // start Comm
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = args[1];
            endPoint_.port = Convert.ToInt32(args[2]);
            translater = new Translater();
            translater.listen(endPoint_);

            // start processing messages
            ProcessMessages();
            LogPath_t.Text = System.IO.Path.Combine(logpath,"SaveFile");

            // load dispatcher
            LoadDispatcher();

            //automatically test
            if (args[3]=="Demo")
                Test1();

        }
        //----< find parent paths >--------------------------------------

        string GetAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }
        //----< file Find Libs tab with subdirectories and files >-------

        void LoadNavTab(string path)
        {
            Dirs.Items.Clear();
            CurrPath.Text = path;
            string[] dirs = Directory.GetDirectories(path);
            Dirs.Items.Add("..");
            foreach (string dir in dirs)
            {
                DirectoryInfo di = new DirectoryInfo(dir);
                string name = System.IO.Path.GetDirectoryName(dir);
                Dirs.Items.Add(di.Name);
            }
            Files.Items.Clear();
            string[] files = Directory.GetFiles(path);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Files.Items.Add(name);
            }
        }
        //----< handle selections in files listbox >---------------------

        private void Files_SelectionChanged(
          object sender, SelectionChangedEventArgs e
        )
        {
            if (unselecting)
            {
                unselecting = false;
                return;
            }
            if (Swin == null)
            {
                Swin = new SelectionWindow();
                Swin.SetMainWindow(this);
            }
            Swin.Show();

            if (e.AddedItems.Count == 0)
                return;
            string selStr = e.AddedItems[0].ToString();
            selStr = System.IO.Path.Combine(Path, selStr);
            if (!SelectedFiles.Contains(selStr))
            {
                SelectedFiles.Add(selStr);
                Swin.Add(selStr);
            }
        }
        //----< unselect files called by unloading SelectionWindow >-----

        public void UnselectFiles()
        {
            unselecting = true;  // needed to avoid using selection logic
                                 //selectedFiles.Clear();
            Files.UnselectAll();
        }
        //----< move into double-clicked directory, display contents >---

        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            string selectedDir = Dirs.SelectedItem.ToString();
            if (selectedDir == "..")
                Path = GetAncestorPath(1, Path);
            else
                Path = System.IO.Path.Combine(Path, selectedDir);
            LoadNavTab(Path);
        }
        //----< shut down the SelectionWindow if open >------------------

        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            Swin.Close();
        }
        //----< copy dlls from different folders to sendingfolder  >------------------

        private void CopyandSendFiles(List<string> files)
        {
            string path_ori = translater.getSendPath();
            //Console.Write(path_ori);
            List<string> dlls = new List<string>();
            foreach (string path_dll in files)
            {
                Console.Write(path_dll + "\n");
                string filename = System.IO.Path.GetFileName(path_dll);
                string targetPath = System.IO.Path.Combine(path_ori, filename);
                System.IO.File.Copy(path_dll, targetPath, true);
                dlls.Add(filename);
                SendingFiles(filename);
            }
        }
        //----< send files  >------------------

        private void SendingFiles(string filename)
        {
            //set serverEndPoint
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8090;

            //construct message for dll
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("sendingFile", filename);
            translater.postMessage(msg);
        }
        //----< send message to host >------------------

        public void SendMessage()
        {
            //copy files
            string tReq = CreateTestrequest();
            CopyandSendFiles(TestRequests[tReq]);

            //set serverEndPoint
            CsEndPoint serverEndPoint = new CsEndPoint();
            serverEndPoint.machineAddress = "localhost";
            serverEndPoint.port = 8090;

            //construct message for task
            CsMessage msg = new CsMessage();
            string tRname = Clientname.ToString() + TestRequests.Count().ToString();
            string lFname = tRname + "_log.txt";
            msg.add("name", tRname);
            msg.add("testLogFile", lFname);
            msg.add("to", CsEndPoint.toString(serverEndPoint));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("testRequest", tReq);
            msg.add("type", "client");
            msg.add("command", "ShowResult");
            msg.add("path", translater.getSavePath());
            translater.postMessage(msg);

        }
        //----< createTestrequest >------------------

        private string CreateTestrequest()
        {
            string dlls = "";
            List<string> files = new List<string>();
            foreach (string path_dll in SelectedFiles)
            {
                files.Add(path_dll);
                string filename = System.IO.Path.GetFileName(path_dll);
                dlls += filename + ", ";
            }
            dlls = dlls.Substring(0, dlls.LastIndexOf(","));
            string time = DateTime.Now.ToString("yyyy-MM-ddTHH:mm:sszzz");
            string testreq = Clientname + ", " + Author + ", " + time + ", " + dlls;
            TestRequests.Add(testreq, files);
            TestReq.Items.Add(testreq);
            return testreq;
        }
        //----< process incoming messages on child thread >----------------

        private void ProcessMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    if (msg.attributes.ContainsKey("command"))
                    {
                        string msgId = msg.value("command");
                        if (dispatcher_.ContainsKey(msgId))
                            dispatcher_[msgId].Invoke(msg);
                    }
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }
        //----< add client processing for message with key >---------------

        private void AddClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }
        //----< load ShowResult processing into dispatcher dictionary >-------

        private void DispatcherLoadShowResult()
        {
            Action<CsMessage> ShowResult = (CsMessage rcvMsg) =>
            {
                string res = "";
                res += rcvMsg.value("testRequest").ToString() + "\n";
                res += "Result:" + rcvMsg.value("TestResult").ToString() + "\n";
                res += "log file:" + rcvMsg.value("testLogFile").ToString() + "\n";
                if (rcvMsg.attributes.ContainsKey("sendingFile"))
                {
                    Console.Write("recieve file");
                }
                
                Action<string> doContent = ( string file) =>
                {
                    AddTestResult(file);
                };
                Dispatcher.Invoke(doContent, new Object[] { res });
            };
            AddClientProc("ShowResult", ShowResult);
        }
        //----< load all dispatcher processing >---------------------------

        private void LoadDispatcher()
        {
            DispatcherLoadShowResult();
        }
        //----< function dispatched by child thread to main thread >-------

        private void AddTestResult( string res)
        {
            TestResult.Items.Add(res);
        }
        //----< function clean selectedFiles >-------

        public void CleanselectedFiles()
        {
            SelectedFiles.Clear();
        }

        private void Test1()
        {
            string testdll_path = GetAncestorPath(1, Path)+ "\\TestDlls_clt";
            string filename1 = "Demo_Testreqs-1.dll";
            string filename2 = "Demo_Testreqs-2.dll";
            filename1 = System.IO.Path.Combine(testdll_path, filename1);
            filename2 = System.IO.Path.Combine(testdll_path, filename2);
            SelectedFiles.Add(filename1);
            SelectedFiles.Add(filename2);
            SendMessage();
            CleanselectedFiles();
        }
    }
}
