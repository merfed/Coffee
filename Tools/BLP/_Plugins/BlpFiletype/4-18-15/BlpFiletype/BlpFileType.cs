using PaintDotNet;
using PaintDotNet.Data;
using SereniaBLPLib;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace PdnBlpFileType
{
    public class BlpFileType : FileType
    {
        public BlpFileType()
            : base("BLP image file",
                FileTypeFlags.SupportsLoading,
                new String[] { ".blp" })
        {
        }

        protected override Document OnLoad(Stream input)
        {
            BlpFile b = new BlpFile(input);
            return Document.FromImage(b.GetBitmap(0));
        }
    }

    public class BlpFileTypeFactory : IFileTypeFactory
    {
        public FileType[] GetFileTypeInstances()
        {
            return new FileType[] { new BlpFileType() };
        }
    }
}
