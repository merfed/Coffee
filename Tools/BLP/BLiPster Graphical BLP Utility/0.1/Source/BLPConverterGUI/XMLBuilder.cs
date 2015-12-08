using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace BLPConverterGUI
{
    class XMLBuilder
    {
        public static System.IO.Stream FileFormatXml()
        {
            string strFileFormats =
                "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" +
                "<Formats>" +
                "  <Format>" +
                "    <Name>[UNSPECIFIED]</Name>" +
                "    <Description>[INVALID]</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_PAL_A0</Name>" +
                "    <Description>Palettized (no alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_PAL_A1</Name>" +
                "    <Description>Palettized (1-bit alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_PAL_A4</Name>" +
                "    <Description>Palettized (4-bit alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_PAL_A8</Name>" +
                "    <Description>Palettized (full alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_DXT1_A0</Name>" +
                "    <Description>DXT1 (no alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_DXT1_A1</Name>" +
                "    <Description>DXT1 (alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_DXT3</Name>" +
                "    <Description>DXT3</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>BLP_DXT5</Name>" +
                "    <Description>DXT5</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>PNG_PAL</Name>" +
                "    <Description>Palettized (no alpha)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>PNG_PAL_MASK</Name>" +
                "    <Description>Palettized (with transparency)</Description>" +
                "  </Format>" +
                "  <Format>" +
                "    <Name>PNG_RGB</Name>" +
                "    <Description>RGB</Description></Format>" +
                "  <Format>" +
                "    <Name>PNG_RGBA</Name>" +
                "    <Description>RGBA</Description></Format>" +
                "</Formats>";

            byte[] byteArray = Encoding.ASCII.GetBytes(strFileFormats);
            System.IO.MemoryStream stream = new System.IO.MemoryStream(byteArray);
            return stream;
        }

        public static System.IO.Stream ConversionRuleXml()
        {
            string strConversions =
                "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" + 
                "<Conversions>" + 
                "  <Conversion>" + 
                "    <Source>BLP_PAL_A0</Source>" + 
                "    <Target>PNG_PAL</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_PAL_A1</Source>" + 
                "    <Target>PNG_PAL_MASK</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_PAL_A4</Source>" + 
                "    <Target>PNG_RGBA</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_PAL_A8</Source>" + 
                "    <Target>PNG_RGBA</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_DXT1_A0</Source>" + 
                "    <Target>PNG_RGB</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_DXT1_A1</Source>" + 
                "    <Target>PNG_RGBA</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_DXT3</Source>" + 
                "    <Target>PNG_RGBA</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>BLP_DXT5</Source>" + 
                "    <Target>PNG_RGBA</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>PNG_PAL</Source>" + 
                "    <Target>BLP_PAL_A0</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>PNG_PAL_MASK</Source>" + 
                "    <Target>BLP_PAL_A1</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>PNG_RGB</Source>" + 
                "    <Target>BLP_DXT1_A0</Target>" + 
                "  </Conversion>" + 
                "  <Conversion>" + 
                "    <Source>PNG_RGBA</Source>" + 
                "    <Target>BLP_DXT3</Target>" + 
                "  </Conversion>" + 
                "</Conversions>";

            byte[] byteArray = Encoding.ASCII.GetBytes(strConversions);
            System.IO.MemoryStream stream = new System.IO.MemoryStream(byteArray);
            return stream;
        }
    }
}
