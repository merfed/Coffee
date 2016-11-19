using System;
using System.IO;
using Autodesk.Maya.OpenMaya;
using M2Export;
using M2Lib.m2;

[assembly: MPxFileTranslatorClass(typeof(M2Translator), "World of Warcraft M2", null, "M2ShowUIOptionsExport", "-expansion 2")]

namespace M2Export
{
    public class M2Translator : MPxFileTranslator
    {
        protected const string FExtension = "m2";
        public override string defaultExtension() => FExtension;
        public override bool haveReadMethod() => false;
        public override bool haveWriteMethod() => true;
        public override bool canBeOpened() => true;

        /// <summary>
        /// Maya calls this method to find out if this translator is capable of
        /// handling the given file.
        /// </summary>
        /// <param name="file"></param>
        /// <param name="buffer"></param>
        /// <param name="bufferLen"></param>
        /// <returns></returns>
        public override MFileKind identifyFile(MFileObject file, string buffer, short bufferLen)
        {
            //TODO Check magic number if bufferLen >= 4 and the first characters are M,D,2,0
            //Based on extension .m2
            var fileName = file.name;
            var fileNameLen = fileName.Length;
            var startOfExtension = fileName.IndexOf('.') + 1;

            if ((startOfExtension > 0)
            && (startOfExtension < fileNameLen)
            && (fileName.Substring(startOfExtension, fileNameLen) == FExtension))
            {
                return MFileKind.kIsMyFileType;
            }
            return MFileKind.kNotMyFileType;
        }

        /// <summary>
        /// Maya calls this method to have the translator write out a file.
        /// </summary>
        /// <param name="file"></param>
        /// <param name="options"></param>
        /// <param name="mode"></param>
        public override void writer(MFileObject file, string options, FileAccessMode mode)
        {
            if(mode == FileAccessMode.kExportActiveAccessMode)
                throw new NotImplementedException("Exporting only selection is not supported yet. Use \"Export All\" instead");
            var expansion = ParseOptions(options);

            MGlobal.displayInfo("Exporting to M2 "+expansion+"..");

            // Name is fileName without .m2 extension
            var wowModel = new M2 {Name = file.rawName.Substring(0, file.rawName.Length - 3)};

            MayaToM2.ExtractModel(wowModel);

            using (var writer = new BinaryWriter(new FileStream(file.expandedFullName, FileMode.Create, FileAccess.Write)))
            {
                wowModel.Save(writer, expansion);
            }
            MGlobal.displayInfo("Done.");
        }

        /// <summary>
        /// Parse options given to the translator.
        /// </summary>
        /// <param name="options">The option string given by Maya.</param>
        /// <returns>The desired WoW version.</returns>
        private static M2.Format ParseOptions(string options)
        {
            var expansion = M2.Format.LichKing; 
            var optionList = options.Split(new[]{' ', ';'}, StringSplitOptions.RemoveEmptyEntries);
            for(var i = 0; i < optionList.Length; i++)
            {
                var option = optionList[i];
                // ReSharper disable once InvertIf
                if (option == "-expansion")
                {
                    i++;
                    var parameter = optionList[i];
                    switch (parameter)
                    {
                        case "0":
                            expansion = M2.Format.Classic;
                            break;
                        case "1":
                            expansion = M2.Format.BurningCrusade;
                            break;
                        case "2":
                            expansion = M2.Format.LichKing;
                            break;
                        case "3":
                            expansion = M2.Format.Cataclysm;
                            break;
                        default:
                            expansion = M2.Format.LichKing;
                            break;
                    }
                }
            }
            return expansion;
        }
    }
}