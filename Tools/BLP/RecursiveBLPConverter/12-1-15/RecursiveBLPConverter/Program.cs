/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

using System;
using System.Diagnostics;
using System.IO;

namespace RecursiveBLPConverter
{
    public static class DirExtension
    {
        public static void Empty(this DirectoryInfo dir)
        {
            foreach (FileInfo file in dir.GetFiles())
                file.Delete();
            foreach (DirectoryInfo subDirectory in dir.GetDirectories())
                subDirectory.Delete(true);
        }
    }

    class Program
    {
        const string APP_TITLE = "ArwicRecursiveBLP";
        const string NEW_DIR_EXT = "_out";

        static string blpConverter = "";
        static string topDir = "";

        static void ConvertBLP(string source)
        {
            ProcessStartInfo psi = new ProcessStartInfo();
            psi.CreateNoWindow = true;
            psi.UseShellExecute = false;
            psi.FileName = blpConverter;
            string dest = source.Replace(topDir, $"{topDir}{NEW_DIR_EXT}");
            if (dest.Length > 3)
            {
                dest = dest.Remove(dest.Length - 3, 3);
                dest = $"{dest}png";
            }
            Directory.CreateDirectory(Path.GetDirectoryName(dest));
            File.Create(dest).Close();
            psi.Arguments = $"\"{source}\" \"{dest}\"";

            int exitCode = 0;
            try
            {
                using (Process p = Process.Start(psi))
                {
                    p.WaitForExit();
                    exitCode = p.ExitCode;
                }
            }
            catch
            {
                Console.WriteLine($"{APP_TITLE}: Error: failed to convert file \n\t'{source}' \nblp converter exited with code \n\t'{exitCode}'");
            }
            if (exitCode != 0)
                Console.WriteLine($"{APP_TITLE}: Error: failed to convert file \n\t'{source}' \nblp converter exited with code \n\t'{exitCode}'");
        }

        static void ConvertAllInDir(string dir)
        {
            try
            {
                // Act on every file in the directory
                foreach (string d in Directory.GetDirectories(dir))
                {
                    foreach (string f in Directory.GetFiles(d))
                    {
                        ConvertBLP(f);
                    }
                    ConvertAllInDir(d);
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void Main(string[] args)
        {
            // check for args
            if (args.Length != 2)
            {
                Console.WriteLine($"{APP_TITLE}: Usage: <path to blp converter> <directory of blps>");
                Console.ReadKey();
                Environment.Exit(1);
            }

            // parse args
            blpConverter = args[0];
            topDir = args[1];

            // prepare output directory
            if (Directory.Exists($"{topDir}{NEW_DIR_EXT}"))
            {
                DirectoryInfo dir = new DirectoryInfo($"{topDir}{NEW_DIR_EXT}");
                dir.Empty();
                dir.Delete();
            }
            Directory.CreateDirectory($"{topDir}{NEW_DIR_EXT}");

            // convert files
            ConvertAllInDir(topDir);
            Console.WriteLine($"{APP_TITLE}: Done");
            Console.ReadKey();
        }
    }
}
