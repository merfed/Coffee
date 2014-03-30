/*
 * Copyright (c) 2010 Marcus Schweda <marcus.schweda@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

using System;
using System.IO;
using System.IO.Pipes;
using System.Threading;

namespace MDS.cBlp2.test {

    /// <summary>
    /// Provides pipe streams for communication between multiple instances of the program.
    /// </summary>
    /// <remarks>
    /// We use this to realize a single instance behaviour. If there is already an instance
    /// running we can pass possible command line parameters to the listening main app and
    /// tell it to bring itself to front.
    /// </remarks>
    static internal class OpenFilePipe {

        // Unique pipe name
        private static string guid = "{25FADEF4-DA4B-4000-90E2-3671A682611F}";
        public static event EventHandler<ArgumentPassedArgs> ArgumentPassedEvent;

        /// <summary>
        /// Passes an argument through the pipe to a listening parent instance.
        /// </summary>
        /// <param name="arg">Argument to pass.</param>
        public static void PassArgument(string arg) {
            try {
                using (NamedPipeClientStream pipeStream = new NamedPipeClientStream(guid)) {
                    using (StreamWriter sw = new StreamWriter(pipeStream)) {
                        pipeStream.Connect(200);
                        sw.WriteLine(arg);
                    }
                }
            } catch (Exception) { }
        }

        /// <summary>
        /// Open a pipe for listening. Subsequent application instances can send data through the pipe.
        /// </summary>
        public static void CreatePipe() {
            ThreadPool.QueueUserWorkItem(new WaitCallback(Listen));
        }

        private static void Listen(object state = null) {
            try {
                using (NamedPipeServerStream pipeStream = new NamedPipeServerStream(guid)) {
                    using (StreamReader sr = new StreamReader(pipeStream)) {
                        pipeStream.WaitForConnection();
                        // Connection to another instance established. Read data.
                        string passedArg = sr.ReadLine();

                        // Raise event
                        ThreadPool.QueueUserWorkItem((delegate(object arg) {
                            ArgumentPassedEvent(typeof(OpenFilePipe), new ArgumentPassedArgs { Argument = (string)passedArg });
                        }), passedArg);
                    }
                }
            } catch (Exception) {
            } finally {
                // Listen again
                Listen();
            }
        }

    }

    public class ArgumentPassedArgs : EventArgs { public string Argument; }

}
