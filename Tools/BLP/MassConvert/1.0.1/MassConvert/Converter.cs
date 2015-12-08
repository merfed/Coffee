using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace Project1
{
    class Converter
    {

        static string ProgramVersion = "1.0.1";
        static string fileType = "";

        static void Main()
        {
            string[] args = Environment.GetCommandLineArgs();
            string dir = Directory.GetCurrentDirectory();
            string desktop = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            string[] files;
            int factor = 40;
            StartUp();
            files = Directory.GetFiles(@dir, "*." + fileType);
            if (files.Length != 0)
            {
                Console.WriteLine(string.Concat("There are ", (int)files.Length, " files in this directory"));
                StringBuilder builder = new StringBuilder();
                for (int w=0; w<files.Length/factor; w++) {
                    for (int i = w*factor; i < w * factor + factor - 1; i++)
                    {
                        builder.Append(string.Concat(files.GetValue(i), " "));
                    }
                    builder.Append(files.GetValue(w * factor + factor - 1));
                    Run(builder.ToString());
                    builder.Clear();
                }
                for (int i=(files.Length-(files.Length % factor)); i < files.Length - 1; i++) {
                    builder.Append(files.GetValue(i) + " ");
                }
                builder.Append(files.GetValue(files.Length - 1));
                Run(builder.ToString());
                builder.Clear();
            }
            else
            {
                Console.WriteLine("No Files found within current directory");
            }
            Exit(0);
        }

        static void StartUp()
        {
            string cv = GetCurrentVersion();
            if (!ProgramVersion.Equals(cv))
            {
                Console.WriteLine("MassConvert is out of date! Newest version: " + cv);
                Console.WriteLine("You can download at http://bit.ly/YhE3sR");
                Exit(0);
            }
            if (!Directory.Exists(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MassConverter"))
            {
                Directory.CreateDirectory(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MassConverter");
            }
            if (!File.Exists(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MassConverter\\BLPConverter.exe"))
            {
                Console.WriteLine("No Converter File supplied. Downloading a new converter.");
                GetNewConverter();
            }
            Console.WriteLine("Please input what files you are converting from");
            GetFileType();
        }

        static String GetCurrentVersion()
        {
            System.Net.WebClient client = new System.Net.WebClient();
            Stream stream = client.OpenRead("https://raw.github.com/1Rogue/MassConvert/master/version.txt");
            StreamReader reader = new StreamReader(stream);
            return reader.ReadToEnd();
        }

        static void GetNewConverter()
        {
            System.Net.WebClient client = new System.Net.WebClient();
            client.DownloadFile("https://github.com/1Rogue/MassConvert/blob/master/Resources/BLPConverter.exe?raw=true", @Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MassConverter\\BLPConverter.exe");
            Console.WriteLine("Converter Downloaded.");
        }

        static void Exit(int code)
        {
            Console.WriteLine("Press any key to continue...");
            Console.ReadKey(true);
            Environment.Exit(code);
        }

        static void Run(string fileargs)
        {
            var process = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData) + "\\MassConverter\\BLPConverter.exe",
                    Arguments = "-e -m " + fileargs
                }
            };
            process.Start();
            process.WaitForExit();
        }

        static void GetFileType()
        {
            string input = Console.ReadLine();
            switch (input.ToLower()) {
                case "png":
                    fileType = "png";
                    break;
                case "blp":
                    fileType = "blp";
                    break;
                default:
                    Console.WriteLine("Invalid input, Please type it again.");
                    Console.WriteLine("Previous input: " + input);
                    GetFileType();
                    break;
            }
        }
    }
}
